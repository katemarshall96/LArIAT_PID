#include "FFTtools.h"

void plotPeakCorrelations() {
    //Will make these arguments soon
    Int_t eventNum=2;
    Int_t firstWire=1;
    Int_t lastWire=240;
    
    TFile *fp = TFile::Open("waveTree.root","OPEN");
    TTree *waveTree = (TTree*) fp->Get("waveTree");
    
    TGraph *grRaw[10][1000];
    TGraph *gr[10][1000];
    TGraph *grFFT[10][1000];
    TGraph *grCor[10][1000];
    double maxCorValues[10][1000];
    int maxCorIndex[10][1000];
    TH2F * maxCor = new TH2F("maxCor", "Peak Corr Values", 240, 0, 240, 10, 0, 1280) ;
    //10, 0, 2560 is sample no. and 100, 0, 100 is wire number (swapped here to flip axes)
    char grName[180];
    char plotArg[180];
    char plotCond[180];
    sprintf(plotArg,"samples:Iteration$");
    int maxIndex=0;
    
    TCanvas *corrCanvas = new TCanvas(); //canvas for correlation plot
    for(int j=0;j<10;j++){
        int  currentIndex = 128*j;
        for(int wire=firstWire;wire<=lastWire;wire++) {
            int wireIndex=wire-firstWire;
            sprintf(plotCond,"eventNum==%d && channel==%d",eventNum,wire);
            corrCanvas->cd(); //always changes to drawing in a new canvas
            waveTree->Draw(plotArg,plotCond);
            Int_t numPoints=waveTree->GetSelectedRows();
            Double_t *sampleNum=waveTree->GetV2();
            Double_t *adcNum=waveTree->GetV1();
            //gr[wireIndex]= new TGraph(numPoints,&sampleNum[0],&adcNum[0]);
            gr[j][wireIndex]= new TGraph(128,&sampleNum[currentIndex],&adcNum[currentIndex]);
            gr[j][wireIndex]->SetLineColor((wireIndex));
            sprintf(grName,"grWire%d",wire);
            gr[j][wireIndex]->SetName(grName);
            grFFT[j][wireIndex]=FFTtools::makePowerSpectrum(gr[j][wireIndex]);
            grFFT[j][wireIndex]->SetLineColor((wireIndex));
            maxIndex=wireIndex;
        }
        
    for(int i=1;i<=maxIndex;i++) {
        
        grCor[j][i-1]=FFTtools::getCorrelationGraph(gr[j][i],gr[j][i-1]);
        //loop with two square brackets is "row" and "column" of matrix
        grCor[j][i-1]->SetLineColor((i));
        maxCorValues[j][i-1]=FFTtools::getPeakVal(grCor[j][i-1],&maxCorIndex[j][i-1]);
        maxCor->Fill(i, currentIndex, maxCorValues[j][i-1]);
        //currentIndex is wire we are currently in
        //j index is "time"
        //i index is "wire"

    }
        cout << "j iteration = " << j << endl;
    }
    
    new TCanvas();
    maxCor->Draw("colz");

}
