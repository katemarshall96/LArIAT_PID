
#include "FFTtools.h"
#include <cassert>
#include <cmath>

//change values of cut as necessary

void drawCut(int eventNum=14){
    
    // Set some defaults
    gROOT->SetStyle("Plain");
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetOptStat(0);
    
    // Open the file and read contents
    TFile* f = new TFile("cor_6326.root");
    f->ls();
    TTree* waveTree = (TTree*) f->Get("waveTree");
    
    TGraph *grRaw[24][1000];
    TGraph *gr[24][1000];
    TGraph *grFFT[24][1000];
    TGraph *grCor[24][1000];

    TH2F * cut2DRaw = new TH2F("cut2DRaw", "", 240,240,480,3072,0,3072);
    cut2DRaw->GetXaxis()->SetTitle("Wire Number");
    cut2DRaw->GetYaxis()->SetTitle("Time");
    TH2F * signalOnly2DHist = new TH2F("signalOnly2DHist", "", 240,240,480,3072,0,3072);
    signalOnly2DHist->GetXaxis()->SetTitle("Wire Number");
    signalOnly2DHist->GetYaxis()->SetTitle("Time");
    
    char grName[180];
    char plotArg[180];
    char plotCond[180];
    sprintf(plotArg,"samples:Iteration$");
    int maxIndex=0;
    
    TCanvas *corrCanvas = new TCanvas(); //canvas for corr plot
    //waveTree->Draw(plotArg, plotCond);
    waveTree->Draw("Iteration$:channel>>cut2DRaw","samples*(samples>15)","colz",480,480*1500);
    TH1F * numNeighboursHist = new TH1F("numNeighboursHist","Number of Neighbours", 10, 0, 10);
    TH1F * numberHitsHist = new TH1F("numberHitsHist", "Number of Hits per Wire", 3000, 0, 3000);
    numberHitsHist->GetXaxis()->SetTitle("Number of Hits");
    TH1F * numberClusters = new TH1F("numberClusters", "Number of Clusters per Wire", 50, 0, 50);
    numberClusters->GetXaxis()->SetTitle("Number of Clusters");
    TH1F * numberClustersCount = new TH1F("numberClustersCount", "Number of Hits per Cluster (Raw Data): eventNum=5", 250, 0, 250); //total number of hits divided by 3 (on average)
    numberClustersCount->GetXaxis()->SetTitle("Number of Hits");
    TH1F * numberClustersCount2 = new TH1F("numberClustersCount2", "Number of Hits per Cluster (Correlated Data): eventNum=920", 250, 0, 250); //total number of hits divided by 3 (on average)
    numberClustersCount2->GetXaxis()->SetTitle("Number of Hits");

    // to make graphs for total number of hits and clusters per wire
    for(int i = 1; i <= cut2DRaw->GetNbinsX() ; i++){
    int wire_hits = 0;
    int wire_contiguous = 0;
    int wire_lasthit = 0;
    for(int time = 1; time <= cut2DRaw->GetNbinsY(); time++){
        if(cut2DRaw->GetBinContent(i, time) > 0) {
            wire_hits++;
            if(!wire_lasthit) //if last wire was not hit (i.e. new cluster)
                wire_contiguous++; //number of clusters
                wire_lasthit = 1;
        } else wire_lasthit = 0;
    }
        numberClusters->Fill(wire_contiguous); // number of clusters per wire
        numberHitsHist->Fill(wire_hits); // number of hits per wire
    
    //to find size of each hit "cluster" i.e. number of time slices
    int wire_contiguousCount[6]; // max is 6 clusters
    int index = 0;
    int wire_contiguousHits = 0;
    for(int time = 1; time <= cut2DRaw->GetNbinsY(); time++){
        if(cut2DRaw->GetBinContent(i, time) > 0) {
            wire_contiguousHits++;
        }
        if(cut2DRaw->GetBinContent(i, time) == 0 && cut2DRaw->GetBinContent(i, time-1) > 0){
            wire_contiguousCount[index] == wire_contiguousHits;
            index ++;
          // if(wire_contiguousHits<=34.827480){
            numberClustersCount->Fill(wire_contiguousHits); //size of clusters
           //}
            wire_contiguousHits = 0;
        }
    }
    }

    //to draw numberNeighboursHist and numberHitsHist
    for(int i = 1; i <= cut2DRaw->GetNbinsX() ; i++){
        for(int j = 1; j <= cut2DRaw->GetNbinsY(); j++){
                if(cut2DRaw->GetBinContent(i, j) > 0){
                int numberNeighbours = 0;
                if(cut2DRaw->GetBinContent(i-1,j-1) != 0) numberNeighbours++;
                if(cut2DRaw->GetBinContent(i  ,j-1) != 0) numberNeighbours++;
                if(cut2DRaw->GetBinContent(i+1,j-1) != 0) numberNeighbours++;
                if(cut2DRaw->GetBinContent(i-1,  j) != 0) numberNeighbours++;
                if(cut2DRaw->GetBinContent(i+1,  j) != 0) numberNeighbours++;
                if(cut2DRaw->GetBinContent(i-1,j+1) != 0) numberNeighbours++;
                if(cut2DRaw->GetBinContent(i  ,j+1) != 0) numberNeighbours++;
                if(cut2DRaw->GetBinContent(i+1,j+1) != 0) numberNeighbours++;
                numNeighboursHist->Fill(numberNeighbours);
                    if(numberNeighbours >=8) {
                        signalOnly2DHist->SetBinContent(i,j,cut2DRaw->GetBinContent(i, j));
                    }
                    
        }
    }
    }
    
    new TCanvas();
    signalOnly2DHist->Draw("colz");
    
    //to find size of each hit "cluster" i.e. number of time slices
    int wire_contiguousCount2[6]; // max is 6 clusters
    int index2 = 0;
    int wire_contiguousHits2 = 0;
    for(int i = 1; i <= signalOnly2DHist->GetNbinsX() ; i++){
        for(int time = 1; time <= signalOnly2DHist->GetNbinsY(); time++){
            if(signalOnly2DHist->GetBinContent(i, time) > 0) {
                wire_contiguousHits2++;
            }
            if(signalOnly2DHist->GetBinContent(i, time) == 0 && signalOnly2DHist->GetBinContent(i, time-1) > 0){
                wire_contiguousCount2[index2] == wire_contiguousHits2;
                index2 ++;
                //if(wire_contiguousHits2<=20){
                numberClustersCount2->Fill(wire_contiguousHits2); //size of clusters
                //}
                wire_contiguousHits2 = 0;
            }
        }
    }
    
    new TCanvas();
    numberClustersCount2->Draw();
    /*TF1* fitClusterSize2 = new TF1("fitFunction", "gaus", 22, 54);
    numberClustersCount2->Fit(fitClusterSize2, "R");
    fitClusterSize2->SetLineColor(kRed);
    fitClusterSize2->Draw("same");*/
    
    //to find total numbr of hits across all wires
    int total_hits = 0;
    for(int wire = 1; wire<=cut2DRaw->GetNbinsX(); wire++){
        for(int time = 1; time <= cut2DRaw->GetNbinsY(); time++){
            if(cut2DRaw->GetBinContent(wire, time) > 0) total_hits++;
        }
    }
    cout << "total number of hits across all wires : " << total_hits << endl;
    
    
    //drawing numberNeighboursHist an numberHitsHist
    new TCanvas();
    gPad->SetLogy();
    numNeighboursHist->Draw();
    new TCanvas();
    numberHitsHist->Draw();
    new TCanvas();
    numberClusters->Draw();
    new TCanvas();
    numberClustersCount->Draw();
    /*TF1* fitClusterSize = new TF1("fitFunction", "gaus",45, 105);
    numberClustersCount->Fit(fitClusterSize, "R");
    fitClusterSize->SetLineColor(kRed);
    fitClusterSize->Draw("same");*/
}
