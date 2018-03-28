#include "FFTtools.h"

void plotCorrelations() {
  //Will make these arguments soon
    Int_t eventNum=2;
    Int_t firstWire=400;
    Int_t lastWire=405;
  
  TFile *fp = TFile::Open("waveTree.root","OPEN");
  TTree *waveTree = (TTree*) fp->Get("waveTree");

  
  TGraph *grRaw[1000];
  TGraph *gr[1000];
  TGraph *grFFT[1000];
  TGraph *grCor[1000];
  char grName[180];
  char plotArg[180];
  char plotCond[180];
  sprintf(plotArg,"samples:Iteration$");
  int maxIndex=0;
  for(int wire=firstWire;wire<=lastWire;wire++) {  
    int index=wire-firstWire;
    sprintf(plotCond,"eventNum==%d && channel==%d",eventNum,wire);
    waveTree->Draw(plotArg,plotCond);
    Int_t numPoints=waveTree->GetSelectedRows();
    Double_t *sampleNum=waveTree->GetV2();
    Double_t *adcNum=waveTree->GetV1();
    //gr[index]= new TGraph(numPoints,&sampleNum[0],&adcNum[0]); //address of element n
    gr[index]= new TGraph(256,&sampleNum[512],&adcNum[512]);
    //  grRaw[index]->SetLineColor((index));
    //  gr[index]=FFTtools::simplePassBandFilter(grRaw[index],2,20);
    gr[index]->SetLineColor((index));
    sprintf(grName,"grWire%d",wire);
    gr[index]->SetName(grName);
    grFFT[index]=FFTtools::makePowerSpectrum(gr[index]); //taken ft, what is the magnitude
    grFFT[index]->SetLineColor((index));
    maxIndex=index;
  }



  new TCanvas(); //c1_n2
  TMultiGraph *mg = new TMultiGraph(); //to plot on top of each other
    for(int i=0;i<=maxIndex;i++){
        mg->Add(gr[i],"lp");}
    mg->Draw("alp");
    mg->GetXaxis()->SetTitle("Time");
    mg->GetYaxis()->SetTitle("ADC Counts");
  
  new TCanvas(); //c1_n3
  TMultiGraph *mgFFT = new TMultiGraph();
  for(int i=0;i<=maxIndex;i++)
    mgFFT->Add(grFFT[i],"lp");
  mgFFT->Draw("alp");
  

  new TCanvas(); //c1_n4
  TMultiGraph *mgCor = new TMultiGraph();
  for(int i=1;i<=maxIndex;i++) {
    grCor[i-1]=FFTtools::getCorrelationGraph(gr[i],gr[i-1]);
    grCor[i-1]->SetLineColor((i));
    mgCor->Add(grCor[i-1],"lp");
  }
  mgCor->Draw("alp");
  mgCor->GetXaxis()->SetTitle("Time");
  mgCor->GetYaxis()->SetTitle("Amplified ADC Counts");
    

  

  



}
