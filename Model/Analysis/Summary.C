#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TLegend.h>
#include <TStyle.h>

void Summary() {

    gStyle->SetOptStat(0);
    gStyle->SetTitleFontSize(0.045);

    const int N = 20;

    double x[N] = {
        25,75,125,175,225,275,325,375,425,475,
        525,575,625,675,725,775,825,875,925,975
    };

    double nonInteract[N] = {
        0.00164999, 0.00764127, 0.01902810, 0.07596560, 0.19066500, 
        0.30067500, 0.38254600, 0.44349700, 0.48846700, 0.52304700, 
        0.55008000, 0.57347600, 0.59134100, 0.60758700, 0.62110800, 
        0.63402600, 0.64545800, 0.65584700, 0.66376800, 0.67298600
    };

    double phot[N] = {
        0.6706240, 0.9243320, 0.9030740, 0.7797960, 0.5896260,
        0.4251760, 0.3110580, 0.2326170, 0.1797660, 0.1423230,
        0.1158740, 0.0956100, 0.0807487, 0.0688219, 0.0592529,
        0.0520621, 0.0457471, 0.0407283, 0.0367337, 0.0328901
    };

    double avgCompt[N] = {
        0.00352703, 0.05198600, 0.07619700, 0.16089300, 0.29238700,
        0.43529800, 0.56375700, 0.67325800, 0.76047900, 0.83173400,
        0.88920800, 0.93613800, 0.97608200, 1.00800000, 1.04034000,
        1.06347000, 1.08585000, 1.10356000, 1.12184000, 1.13949000
    };
    
    double avgHits[N] = {
        0.575280, 0.860933, 1.017290, 1.014460, 0.948283,
        0.864794, 0.793652, 0.736502, 0.689056, 0.651002,
        0.619652, 0.591486, 0.569617, 0.548863, 0.533337,
        0.515893, 0.501273, 0.487479, 0.477466, 0.465583
    };

    // Build graphs with style
    TGraph *gNon = new TGraph(N, x, nonInteract);
    TGraph *gPhot = new TGraph(N, x, phot);
    TGraph *gCompt = new TGraph(N, x, avgCompt);
    TGraph *gHits = new TGraph(N, x, avgHits);

    auto styleGraph = [](TGraph* g, Color_t c, int m) {
        g->SetMarkerStyle(m);
        g->SetMarkerColor(c);
        g->SetMarkerSize(1.4);
        g->SetLineColor(c);
        g->SetLineWidth(2);
    };

    styleGraph(gNon, kRed+1, 20);
    styleGraph(gPhot, kBlue+1, 21);
    styleGraph(gCompt, kGreen+2, 22);
    styleGraph(gHits, kMagenta+2, 23);

    // Canvas
    TCanvas *c = new TCanvas("c", "Summary Plots", 1400, 1000);
    c->Divide(2, 2);

    // Panel 1
    c->cd(1); 
    gPad->SetGrid();
    gNon->SetTitle("Probability of No Interaction;Energy (keV);Prob");
    gNon->Draw("APL");

    // Panel 2
    c->cd(2); 
    gPad->SetGrid();
    gPhot->SetTitle("Single Hit Deposition Fraction;Energy (keV);Fraction");
    gPhot->Draw("APL");

    // Panel 3
    c->cd(3); 
    gPad->SetGrid();
    gCompt->SetTitle("Average Number of Compton Events;Energy (keV);<N_{Compton}>");
    gCompt->Draw("APL");

    // Panel 4
    c->cd(4); 
    gPad->SetGrid();
    gHits->SetTitle("Average Number of Hits;Energy (keV);<N>");
    gHits->Draw("APL");
}
