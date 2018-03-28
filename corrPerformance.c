#include "FFTtools.h"
#include <cassert>
#include <cmath>
#include <list>

//change values of cut as necessary

void corrPerfromance(int eventNum=2){
    
    Int_t firstWire=1;
    Int_t lastWire=240;
    
    // Set some defaults
    gROOT->SetStyle("Plain");
    gStyle->SetCanvasBorderMode(0);
    
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
    int maxValIndex[24][1000];
    int maxCorIndex[24][1000];
    TH1F * maxCorrHist = new TH1F("maxCorHist", "max Corr histogram with cut", 4500, 0, 4500);
    TH1F * maxValHist = new TH1F("maxValHist", "max unCorr histogram with cut", 1000, 0, 1000);

    char grName[180];
    char plotArg[180];
    char plotCond[180];
    sprintf(plotArg,"samples:Iteration$");
    int maxIndex=0;
    TCanvas *corrCanvas = new TCanvas(); //canvas for corr plot
    
    //int binarySignalIndicator[11][1001] = {0};
    for(int j=0;j<24;j++){
        cerr<<"*";
        int  currentIndex = 128*j;

            //loop for getting uncorr (raw) values from wavetree and filling uncorr histogram
        for(int wire=firstWire;wire<=lastWire;wire++) {
            int wireIndex=wire-firstWire;
            sprintf(plotCond,"eventNum==%d && channel==%d",eventNum,wire);
            corrCanvas->cd(); //always changes to drawing in a new canvas
            waveTree->Draw(plotArg,plotCond);
            //waveTree->Draw("Iteration$:channel>>(240,0,240,24,0,3072)","samples","colz",240,240*2);

            Int_t numPoints=waveTree->GetSelectedRows();
            Double_t *sampleNum=waveTree->GetV2();
            Double_t *adcNum=waveTree->GetV1();
            gr[j][wireIndex]= new TGraph(128,&sampleNum[currentIndex],&adcNum[currentIndex]);
            gr[j][wireIndex]->SetLineColor((wireIndex));
            sprintf(grName,"grWire%d",wire);
            gr[j][wireIndex]->SetName(grName);
            maxValues[j][wireIndex]=FFTtools::getPeakVal(gr[j][wireIndex],&maxCorIndex[j][wireIndex]);
            if (maxValues[j][wireIndex]<30){
                maxValHist->Fill(maxValues[j][wireIndex]);
                //cut2DRaw->Fill(wireIndex, currentIndex, maxValues[j][wireIndex-1]); //why -1 ?
                //binarySignalIndicator[j][wireIndex] = 1;
            }
            //printf("%d", binarySignalIndicator[j][wireIndex]);
            maxIndex=wireIndex;
        }
        
        //loop for getting correlated values and filling corr histogram
        for(int i=1;i<=maxIndex;i++) {
            
            grCor[j][i-1]=FFTtools::getCorrelationGraph(gr[j][i],gr[j][i-1]);
            grCor[j][i-1]->SetLineColor((i));
            maxCorValues[j][i-1]=FFTtools::getPeakVal(grCor[j][i-1],&maxCorIndex[j][i-1]);
            if (maxCorValues[j][i-1]<90){
                maxCorrHist->Fill(maxCorValues[j][i-1]);
                //cut2DCorr->Fill(i, currentIndex, maxCorValues[j][i-1]);
            }
        }
        
    }
    
    maxCorrHist->GetXaxis()->SetTitle("Sample Charge");
    maxCorrHist->GetYaxis()->SetTitle("Samples");
    maxValHist->GetXaxis()->SetTitle("Sample Charge");
    maxValHist->GetYaxis()->SetTitle("Samples");
    new TCanvas();
    maxCorrHist->SetLineColor(kBlue);
    maxCorrHist->Draw();
    new TCanvas();
    maxValHist->SetLineColor(kRed);
    maxValHist->Draw();

}

