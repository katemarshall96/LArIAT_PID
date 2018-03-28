//To plot 1D map of 2D (collection plane) plot

#include "FFTtools.h"

void plot_1D(int eventNum=190);

void plot_1D(int eventNum){

    // Set some defaults
    gROOT->SetStyle("Plain");
    gStyle->SetCanvasBorderMode(0); // turn off canvas borders
    
    // Open the file and read contents
    TFile* f = new TFile("out_6326.root");
    f->ls();
    TTree* wavetree = (TTree*) f->Get("waveTree");
    
    // Plot on new canvas
    
    TCanvas *canvas = new TCanvas();
    char condition[180];
    sprintf(condition, "samples*(eventNum==%d)", eventNum);
    TH2F*h1=new TH2F("h1", "Collection Plane", 240, 240, 480, 3072, 0, 3072);
    gPad->SetLogy(); //because much more noise than signal
    
    TH1F * chargehist = new TH1F("chargehist", "Peak Charge Values", 1000, 0, 1000);
    //can go to 3072 but cannot see much beyond 2000
    chargehist->GetXaxis()->SetTitle("Sample Charge");
    chargehist->GetYaxis()->SetTitle("Samples");
    wavetree->Draw("Max$(samples)>>chargehist", "channel>239");
    waveTree->Draw("Iteration$:channel>>cut2DRaw","samples","colz",480,480*920);

    
    //Fit signal peak with landau
    /*TF1*fit = new TF1("fitFunction", "landau", 70, 600);
    chargehist->Fit(fit, "R");
    fit->SetLineColor(kRed);
    fit->Draw("same");*/
    
}
