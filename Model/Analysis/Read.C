#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TMath.h>
#include <iostream>
#include <set>
#include <TTreeIndex.h>
using namespace std;

void Read() {
    // -----------------------------
    // Load file and tree
    // -----------------------------
    TString dir = "../Data/";
    TString datafile = "data_20m_0000_1000_keV.root";
    TFile *f = new TFile(dir + datafile, "READ");
    if(!f || f->IsZombie()) { cerr << "Error opening file!" << endl; return; }

    TTree *t = (TTree*)f->Get("T");
    if(!t) { cerr << "Tree T not found!" << endl; return; }

    // -----------------------------
    // Branch setup
    // -----------------------------
    int eventID, copyNo, initCryst;
    double eDep, eTrue, eBE, eComptLoss;
    double eLeak_Top, eLeak_Left, eLeak_Bottom, eLeak_Right, eLeak_Front, eLeak_Back;
    double xHit, yHit, eAbsorb;

    double emax = 0.0;
    double sumEdep = 0.0;
    double sumAbs = 0.0;
    double sumLeak = 0.0;
    double sumBE = 0.0;
    double sumEComptLoss = 0.0;
    double elost = 0.0;
    double eTrue_evt = 0.0;
    int currentEvent = -1;
    int numMiss = 0 , numPhot = 0, numCompt = 0;
    double avgNumCompt = 0.;
    int totEvt = 0;
    set<int> crystals;
    vector<int> cID;

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
    t->BuildIndex("eventID");
    TTreeIndex *index = (TTreeIndex*)t->GetTreeIndex();
    double efront=0;
    for(Long64_t i = 0; i < nEntries; i++) {
        Long64_t local = t->LoadTree(index->GetIndex()[i]);
        t->GetEntry(local);
        if(eventID != currentEvent && currentEvent != -1 && eTrue_evt > 0) {
            //cout<<"Processing event: "<<currentEvent<<endl;
            double eTot = sumEdep + sumLeak + sumBE + sumAbs + sumEComptLoss;
            //cout<<"ETrue: "<<eTrue_evt<<"; Etot: "<<eTot<<endl;
            //cout<<eTrue_evt - eTot<<endl;
            //cout<<"========================================================="<<endl;
            if (abs(eTrue_evt - eTot) < 1e-6){
                cout<<"eTrue: "<<eTrue_evt<<"; eTot: "<<eTot<<"; eCrystDep: "<<sumEdep<<"; eLeak: "<<sumLeak<<"; eAbs: "<<sumAbs<<"; eBE: "<<sumBE<<"; ECLoss: "<<sumEComptLoss<<endl;
            }
            // if((eTrue_evt - eTot) < -1e-6){
            //     for(int k=0; k<cID.size(); k++){
            //         cout<<cID[k]<<endl;
            //     }
            //     cout<<"eTrue: "<<eTrue_evt<<"; eTot: "<<eTot<<"; eCrystDep: "<<sumEdep<<"; eLeak: "<<sumLeak<<"; eAbs: "<<sumAbs<<"; eBE: "<<sumBE<<"; ECLoss: "<<sumEComptLoss<<endl;
            // }

            //cout<<"eTrue: "<<eTrue_evt<<"; eTot: "<<eTot - sumLeak<<"; eCrystDep: "<<sumEdep<<"; eLeak: "<<sumLeak<<"; eAbs: "<<sumAbs<<"; eBE: "<<sumBE<<"; ECLoss: "<<sumEComptLoss<<endl;

            emax = 0.0;
            sumEdep = 0.0;
            sumAbs = 0.0;
            sumLeak = 0.0;
            elost = 0.0;
            sumBE = 0.0;
            sumEComptLoss = 0.0;
            crystals.clear();
            cID.clear();
        }
        //cout<<"eTrue: "<<eTrue<<"; CrystID: "<<copyNo<<"; eCrystDep: "<<eDep<<"; eLeak: "<<(eLeak_Top + eLeak_Left + eLeak_Bottom + eLeak_Right + eLeak_Front + eLeak_Back)<<"; eAbs: "<<eAbsorb<<"; eBE: "<<eBE<<"; ECLoss: "<<eComptLoss<<endl;
        eTrue_evt = eTrue;
        sumAbs = eAbsorb;
        sumBE += eBE;
        cID.push_back(copyNo);
        sumEComptLoss += eComptLoss;
        sumLeak = (eLeak_Top + eLeak_Left + eLeak_Bottom + eLeak_Right + eLeak_Front + eLeak_Back);
        elost = (eLeak_Top + eLeak_Left + eLeak_Bottom + eLeak_Right + eLeak_Front);
        efront = eLeak_Front;

        if(eDep > 0.) crystals.insert(copyNo);
        sumEdep += eDep;
            
        currentEvent = eventID;
    }
}
