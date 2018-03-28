//To plot 1D map of 2D correlation plot

#include "FFTtools.h"
#include <cassert>
#include <cmath>

void plot_1DCorr(int eventNum=2);
void plot_1DCorr(int eventNum){
    
    Int_t firstWire=1;
    Int_t lastWire=480;
    
    // Set some defaults
    gROOT->SetStyle("Plain");
    gStyle->SetCanvasBorderMode(0); // turn off canvas borders
    //gStyle->SetOptStat(0);
    
    // Open the file and read contents
    TFile* f = new TFile("waveTree.root");
    f->ls();
    TTree* waveTree = (TTree*) f->Get("waveTree");

    TGraph *grRaw[24][1000];
    TGraph *gr[24][1000];
    TGraph *grFFT[24][1000];
    TGraph *grCor[24][1000];
    double maxValues[24][1000];
    double maxCorValues[24][1000];
    int maxValIndex[24][1000]; // 10 is size of time chunks and 1000 is example number > no. wires
    int maxCorIndex[24][1000];
    //TH2F * maxCor = new TH2F("maxCor", "Peak Corr Values",  240, 0, 240, 24, 0, 3072);
    //TH2F * maxCor = new TH2F("maxCor", "Peak Corr Values",  240, 0, 240, 10, 0, 1280);
    //maxCor->GetXaxis()->SetTitle("Wire Number");
    //maxCor->GetYaxis()->SetTitle("Time");
    //TH2F * maxVal = new TH2F("maxVal", "Peak unCorr Values", 240, 0, 240, 24, 0, 3072);
    //TH2F * maxVal = new TH2F("maxVal", "Peak Corr Values",  240, 0, 240, 10, 0, 1280);
    //maxVal->GetXaxis()->SetTitle("Wire Number");
    //maxVal->GetYaxis()->SetTitle("Time");
    TH1F * maxCorHist = new TH1F("maxCorHist", "Peak Corr Values", 4000, 0, 4000);
    maxCorHist->GetXaxis()->SetTitle("Sample Charge");
    maxCorHist->GetYaxis()->SetTitle("Samples");
    TH1F * maxValHist = new TH1F("maxValHist", "Peak unCorr Values", 1000, 0, 1000);
    //TH1F * maxValHist = new TH1F("maxValHist", "Peak unCorr Values", 200, 0, 200);
    maxValHist->GetXaxis()->SetTitle("Sample Charge");
    maxValHist->GetYaxis()->SetTitle("Samples");
    char grName[180];
    char plotArg[180];
    char plotCond[180];
    sprintf(plotArg,"samples:Iteration$");
    int maxIndex=0;

    TCanvas *corrCanvas = new TCanvas(); //canvas for correlation plot
    for(int j=0;j<24;j++){
    //for(int j=0;j<10;j++){
        cerr<<"*";
        int  currentIndex = 128*j;
        for(int wire=firstWire;wire<=lastWire;wire++) {
            int wireIndex=wire-firstWire;
            sprintf(plotCond,"eventNum==%d && channel==%d",eventNum,wire);
            corrCanvas->cd(); //always changes to drawing in a new canvas
            waveTree->Draw(plotArg, plotCond);
            //waveTree->Draw("Iteration$:channel>>(240,0,240,24,0,3072)","samples","colz",240,240*2);
            Int_t numPoints=waveTree->GetSelectedRows();
            Double_t *sampleNum=waveTree->GetV2();
            Double_t *adcNum=waveTree->GetV1();
            //gr[wireIndex]= new TGraph(numPoints,&sampleNum[0],&adcNum[0]);
            gr[j][wireIndex]= new TGraph(128,&sampleNum[currentIndex],&adcNum[currentIndex]);
            gr[j][wireIndex]->SetLineColor((wireIndex));
            sprintf(grName,"grWire%d",wire);
            gr[j][wireIndex]->SetName(grName);
            maxValues[j][wireIndex]=FFTtools::getPeakVal(gr[j][wireIndex],&maxCorIndex[j][wireIndex]);
            //maxVal->Fill(wireIndex, currentIndex, maxValues[j][wireIndex]); //removed -1 ?
            maxValHist->Fill(maxValues[j][wireIndex]);
            //grFFT[j][wireIndex]=FFTtools::makePowerSpectrum(gr[j][wireIndex]);
            //grFFT[j][wireIndex]->SetLineColor((wireIndex));
            maxIndex=wireIndex;
    }
    
    for(int i=1;i<=maxIndex;i++) {
        
        grCor[j][i-1]=FFTtools::getCorrelationGraph(gr[j][i],gr[j][i-1]);
        grCor[j][i-1]->SetLineColor((i));
        maxCorValues[j][i-1]=FFTtools::getPeakVal(grCor[j][i-1],&maxCorIndex[j][i-1]);
        //maxCor->Fill(i, currentIndex, maxCorValues[j][i-1]);
        maxCorHist->Fill(maxCorValues[j][i-1]);
            
        }

}
    
    new TCanvas();
    maxCorHist->SetLineColor(kBlue);
    maxCorHist->Draw();
    new TCanvas();
    maxValHist->SetLineColor(kRed);
    maxValHist->Draw();
    
    /*new TCanvas();
    maxVal->Draw("colz");
    
    new TCanvas();
    maxCor->Draw("colz");
    
    TH1 * cumulativeHist = maxCorHist->GetCumulative();
    Double_t * integral = maxCorHist->GetIntegral();
    for(int i=1; i<=maxCorHist->GetNbinsX(); i++) {
        assert(std::abs(integral[i] * maxCorHist->GetEntries() - cumulativeHist->GetBinContent(i)));
    }
    
    TH1 * cumulativeHistRaw = maxValHist->GetCumulative();
    Double_t * integralRaw = maxValHist->GetIntegral();
    for(int i=1; i<=maxValHist->GetNbinsX(); i++) {
        assert(std::abs(integralRaw[i] * maxValHist->GetEntries() - cumulativeHistRaw->GetBinContent(i)));
    }
    
    new TCanvas();
    cumulativeHist->GetXaxis()->SetTitle("Sample Charge");
    cumulativeHist->GetYaxis()->SetTitle("Time");
    cumulativeHist->Draw();
    
    new TCanvas();
    cumulativeHistRaw->GetXaxis()->SetTitle("Sample Charge");
    cumulativeHistRaw->GetYaxis()->SetTitle("Time");
    cumulativeHistRaw->Draw();*/
    
}

//cuts applied in corrPerformance.c


