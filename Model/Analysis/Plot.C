#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TMath.h>
#include <iostream>
#include <set>
#include <TTreeIndex.h>
using namespace std;

void Plot(double eLow = 0.0, double eUp = 1000.0) 
{
    // --------------------------------------------------------
    // Load file and tree
    // --------------------------------------------------------
    TString dir = "../Data/";
    //TString datafile = Form("data_%g_%g.root", eLow, eUp);
    TString datafile = "data_20m_0000_1000_keV.root";
    TFile *f = new TFile(dir + datafile, "READ");

    if (!f || f->IsZombie()) { 
        cerr << "Error opening file!" << endl; 
        return; 
    }

    TTree *t = (TTree*)f->Get("T");
    if (!t) { 
        cerr << "Tree T not found!" << endl; 
        return; 
    }

    // --------------------------------------------------------
    // Branch setup
    // --------------------------------------------------------
    int eventID, copyNo, initCryst;
    double eDep, eTrue, eBE, eComptLoss;
    double eLeak_Top, eLeak_Left, eLeak_Bottom, eLeak_Right, eLeak_Front, eLeak_Back;
    double xHit, yHit, eAbsorb;

    t->SetBranchAddress("eventID", &eventID);
    t->SetBranchAddress("copyNo", &copyNo);
    t->SetBranchAddress("totalEdep", &eDep);
    t->SetBranchAddress("absorbEdep", &eAbsorb);
    t->SetBranchAddress("P_BindingEnergy", &eBE);
    t->SetBranchAddress("EComptLoss", &eComptLoss);
    t->SetBranchAddress("InitCrystalID", &initCryst);
    t->SetBranchAddress("xHit", &xHit);
    t->SetBranchAddress("yHit", &yHit);
    t->SetBranchAddress("ETrue", &eTrue);
    t->SetBranchAddress("ELeak_Top", &eLeak_Top);
    t->SetBranchAddress("ELeak_Left", &eLeak_Left);
    t->SetBranchAddress("ELeak_Bottom", &eLeak_Bottom);
    t->SetBranchAddress("ELeak_Right", &eLeak_Right);
    t->SetBranchAddress("ELeak_Front", &eLeak_Front);
    t->SetBranchAddress("ELeak_Back", &eLeak_Back);

    Long64_t nEntries = t->GetEntries();

    // --------------------------------------------------------
    // Histograms - Canvas 1
    // --------------------------------------------------------
    TH1F *hEmax   = new TH1F("hEmax", "Maximum Energy Deposited per Event;E_{max} (keV);Events", 300, 0, 1200);
    TH1F *hNhits  = new TH1F("hNhits", "Number of Crystals Fired per Event;N_{hits};Events", 15, 0, 15);
    TH1F *hElost  = new TH1F("hElost", "Energy Lost per Event (excluding Back);E_{lost} (keV);Events", 300, 0, 1200);
    TH2F *hHitPattern = new TH2F("hHitPattern", "Crystal Hit Map;x_{loc};y_{loc}", 8, 0, 8, 8, 0, 8);

    // --------------------------------------------------------
    // Histograms - Canvas 2
    // --------------------------------------------------------
    TH1F *hETrue    = new TH1F("hETrue",    "True Photon Energy per Event;E_{true} (keV);Events", 200, 0, 1200);
    TH1F *hEDepNorm = new TH1F("hEDepNorm", "Total Deposited Energy / True Energy;#Sigma E_{dep}/E_{true};Events", 150, 0, 1.5);
    TH1F *hEAbsNorm = new TH1F("hEAbsNorm", "Absorbed Energy / True Energy;#Sigma E_{abs}/E_{true};Events", 150, 0, 1.5);
    TH1F *hELeakFrac = new TH1F("hELeakFrac","Leakage Fraction per Event;#Sigma E_{leak}/E_{true};Events", 150, 0, 1.5);

    // --------------------------------------------------------
    // Histograms - Canvas 3
    // --------------------------------------------------------
    //TH2F *hEHits = new TH2F("hEHits", "Hit Energy;E_{true} (keV);Number of Hits", 22, 0, 1100, 10, 0, 10);
    TProfile *hEHits = new TProfile("hEHits", "Number of Hits;E_{true} (keV);N_{hits}",20, 0, 1000);

    // --------------------------------------------------------
    // Setup event loop
    // --------------------------------------------------------
    t->BuildIndex("eventID");
    TTreeIndex *index = (TTreeIndex*)t->GetTreeIndex();

    double emax = 0.0;
    double sumEdep = 0.0;
    double sumLoss = 0.0;
    double sumAbs = 0.0;
    double sumLeak = 0.0;
    double elost   = 0.0;
    double eTrue_evt = 0.0;

    int currentEvent = -1;
    int numMiss = 0;
    int numPhot = 0;
    int numCompt = 0;
    int totEvt = 0;

    double avgNumCompt = 0.;
    double tol = 1e-10;

    set<int> hitRegThr;
    set<int> hitReg;

    // --------------------------------------------------------
    // Main event loop
    // --------------------------------------------------------
    for (Long64_t i = 0; i < nEntries; i++) 
    {
        Long64_t local = t->LoadTree(index->GetIndex()[i]);
        t->GetEntry(local);
        // ----------------------------------------------------
        // Event boundary: finalize previous event
        // ----------------------------------------------------
        if (eventID != currentEvent && currentEvent != -1 && eTrue_evt > 0) 
        {
            int nHitThr = hitRegThr.size();
            int nHit = hitReg.size();
            if (emax > 0) hEmax->Fill(emax);

            hElost->Fill(elost);
            hETrue->Fill(eTrue_evt);
            hEDepNorm->Fill(sumEdep / eTrue_evt);
            hEAbsNorm->Fill(sumAbs / eTrue_evt);
            hELeakFrac->Fill(sumLeak / eTrue_evt);
            hNhits->Fill(nHitThr);
            hEHits->Fill(eTrue_evt, nHitThr);

            // Count missed events
            if ((eTrue_evt - sumLeak) < tol && nHit == 0)
                numMiss++;

            // ------------------------------------------------
            // Photoelectric / Compton classification
            // ------------------------------------------------
            if (sumLeak < tol) 
            {
                if (nHit == 1) 
                {
                    numPhot++;
                    numCompt++;
                    avgNumCompt = avgNumCompt * (numCompt - 1) / (numCompt);
                }
                else if (nHit > 1) 
                {
                    int numComptEvt = (nHit - 1);
                    numCompt++;
                    avgNumCompt = (avgNumCompt * (numCompt - 1) + numComptEvt) / (numCompt);
                }
            } 
            else 
            {
                if (nHit != 0) 
                {
                    int numComptEvt = nHit;
                    numCompt++;
                    avgNumCompt = (avgNumCompt * (numCompt - 1) + numComptEvt) / (numCompt);
                }
            }

            totEvt++;

            // Reset accumulators
            emax = 0.0;
            sumEdep = 0.0;
            sumAbs = 0.0;
            sumLeak = 0.0;
            sumLoss = 0.0;
            elost = 0.0;
            hitReg.clear();
            hitRegThr.clear();
        }

        // ----------------------------------------------------
        // Process current entry
        // ----------------------------------------------------
        eTrue_evt = eTrue;

        if (eTrue_evt > eLow && eTrue_evt <= eUp) 
        {
            // Update per-event values
            sumAbs = eAbsorb;
            sumLeak = (eLeak_Top + eLeak_Left + eLeak_Bottom + eLeak_Right + eLeak_Front + eLeak_Back);
            elost = (eLeak_Top + eLeak_Left + eLeak_Bottom + eLeak_Right + eLeak_Front);

            if (eDep > emax) emax = eDep;
            if (eDep > 10.0) hitRegThr.insert(copyNo);
            if (eDep > 0.0) hitReg.insert(copyNo);

            sumEdep += eDep;
            sumLoss += (eComptLoss + eBE);

            // Hit map
            int xLoc = int(copyNo / 8);
            int yLoc = int(copyNo % 8);
            hHitPattern->Fill(xLoc, yLoc);
        } 
        else {
            eTrue_evt = -1.0;
        }

        currentEvent = eventID;
    }

    // --------------------------------------------------------
    // Final event (post-loop)
    // --------------------------------------------------------
    if (currentEvent != -1 && eTrue_evt > 0) 
    {
        int nHitThr = hitRegThr.size();
        int nHit = hitReg.size();
        if (emax > 0) hEmax->Fill(emax);

        hElost->Fill(elost);
        hETrue->Fill(eTrue_evt);
        hEDepNorm->Fill(sumEdep / eTrue_evt);
        hEAbsNorm->Fill(sumAbs / eTrue_evt);
        hELeakFrac->Fill(sumLeak / eTrue_evt);
        hNhits->Fill(nHitThr);
        hEHits->Fill(eTrue_evt, nHitThr);

        // Count missed events
        if ((eTrue_evt - sumLeak) < tol && nHit == 0)
            numMiss++;

        // ------------------------------------------------
        // Photoelectric / Compton classification
        // ------------------------------------------------
        if (sumLeak < tol) 
        {
            if (nHit == 1) 
            {
                numPhot++;
                numCompt++;
                avgNumCompt = avgNumCompt * (numCompt - 1) / (numCompt);
            }
            else if (nHit > 1) 
            {
                int numComptEvt = (nHit - 1);
                numCompt++;
                avgNumCompt = (avgNumCompt * (numCompt - 1) + numComptEvt) / (numCompt);
            }
        } 
        else 
        {
            if (nHit != 0) 
            {
                int numComptEvt = nHit;
                numCompt++;
                avgNumCompt = (avgNumCompt * (numCompt - 1) + numComptEvt) / (numCompt);
            }
        }

        totEvt++;

        emax = 0.0;
        sumEdep = 0.0;
        sumAbs = 0.0;
        sumLeak = 0.0;
        sumLoss = 0.0;
        elost = 0.0;
        hitReg.clear();
        hitRegThr.clear();
    }

    // --------------------------------------------------------
    // Drawing and printing results
    // --------------------------------------------------------
    cout << "Entries in hETrue = " << hETrue->GetEntries() << endl;

    // Canvas 1
    TCanvas *c1 = new TCanvas("c1", "Detector Energy Summary", 1600, 1000);
    c1->Divide(2, 2);
    c1->cd(1); hEmax->SetLineColor(kBlue); hEmax->Draw();
    c1->cd(2); hNhits->SetLineColor(kRed); hNhits->Draw();
    c1->cd(3); hElost->SetLineColor(kGreen+2); hElost->Draw();
    c1->cd(4); gStyle->SetOptStat(0); hHitPattern->Draw("COLZ");

    // Canvas 2
    TCanvas *c2 = new TCanvas("c2", "Energy Balance per Event", 1600, 1000);
    c2->Divide(2, 2);
    c2->cd(1); hETrue->SetLineColor(kMagenta); hETrue->Draw();
    c2->cd(2); hEDepNorm->SetLineColor(kBlue+1); hEDepNorm->Draw();
    c2->cd(3); hEAbsNorm->SetLineColor(kGreen+2); hEAbsNorm->Draw();
    c2->cd(4); hELeakFrac->SetLineColor(kOrange+1); hELeakFrac->Draw();

    for (int i = 1; i <= 4; i++) {
        if (i != 4) { c1->cd(i); gPad->SetLogy(); }
        c2->cd(i); gPad->SetLogy();
    }

    // Canvas 3
    TCanvas *c3 = new TCanvas("c3", "Hit Energy vs Number of Hits", 800, 600);
    gStyle->SetOptStat(0);
    hEHits->Draw();

    c1->Update();
    c2->Update();
    c3->Update();

    c1->SaveAs("c1.root");
    c2->SaveAs("c2.root");
    c3->SaveAs("c3.root");

    // --------------------------------------------------------
    // Summary print
    // --------------------------------------------------------
    cout << "Total number of events (" << eLow << " < Etrue <= " << eUp << " keV): " << totEvt << endl; 
    cout << endl;

    cout << "Number of missed events: " << numMiss << endl;
    cout << "Fraction of missed events: " << double(numMiss)/double(totEvt) << endl;
    cout << endl;

    cout << "Number of Photoelectric events: " << numPhot << endl;
    cout << "Fraction of Photoelectric events: " << double(numPhot)/double(totEvt) << endl;
    cout << endl;

    cout << "Average number of Compton events: " << avgNumCompt << endl;
    cout << endl;

    double avgHits = hNhits->GetMean();
    cout << "Average number of hits per event = " << avgHits << endl;
}
