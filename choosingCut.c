
#include "FFTtools.h"
#include <cassert>
#include <cmath>
#include <list>

//change values of cut as necessary

void choosingCut(int eventNum=2){
    
    // Set some defaults
    gROOT->SetStyle("Plain");
    gStyle->SetCanvasBorderMode(0);
    
    // Open the file and read contents
    TFile* f = new TFile("waveTree.root");
    f->ls();
    TTree* waveTree = (TTree*) f->Get("waveTree");
    
    TGraph *grRaw[10][1000];
    TGraph *gr[10][1000];
    TGraph *grFFT[10][1000];
    TGraph *grCor[10][1000];
    
    const char *names[11];
    for (int i = 0; i <=11; i++){
        char name[11];
        name = strcat("cut2DRaw_", sprintf(str, "%d", (i*2 +10)));
        names[i] = name;
        TH2F * cut2DRaw_20 = new TH2F(names[i], "2D Histogram with cut data", 240, 240, 480, 3000, 0, 3000);

    char grName[180];
    char plotArg[180];
    char plotCond[180];
    sprintf(plotArg,"samples:Iteration$");
    int maxIndex=0;
    
    for (int k = 0; k <=11; k++){
    TCanvas *corrCanvas = new TCanvas(); //canvas for corr plot
    waveTree->Draw(strcat("Iteration$:channel>>",names[i]), "samples*(eventNum==2 && samples>30)","colz");
    }
    //TH1F * numNeighboursHist = new TH1F("numNeighboursHist","Number of Neighbours", 10, 0, 10);
    TH1F * numberHitsHist[11] = new TH1F("numberHitsHist", "Number of Hits per Wire", 3000, 0, 3000);
    TH1F * numberClusters[11] = new TH1F("numberClusters", "Number of Clusters per Wire", 10, 0, 10);
    TH1F * numberClustersCount[11] = new TH1F("numberClustersCount", "Number of Hits per Cluster", 100, 0, 100); //total number of hits divided by 3 (on average)
    
    
    
    // to make graphs for total number of hits and clusters per wire
        
    for (int k = 0; k <=11; k++){
        for(int l = 1; l <= names[k]->GetNbinsX() ; l++){
            int wire_hits = 0;
            int wire_contiguous = 0;
            int wire_lasthit = 0;
            for(int time = 1; time <= names[k]->GetNbinsY(); time++){
                if(names[k]->GetBinContent(l, time) > 0) {
                    wire_hits++;
                    if(!wire_lasthit) //if last wire was not hit (i.e. new cluster)
                        wire_contiguous++; //number of clusters
                    wire_lasthit = 1;
                } else wire_lasthit = 0;
            }
            numberClusters[k]->Fill(wire_contiguous); // number of clusters per wire
            numberHitsHist[k]->Fill(wire_hits); // number of hits per wire
        
            //to find size of each hit "cluster" i.e. number of time slices
            int wire_contiguousCount[10]; // max is 6 clusters for cut >30
            int index = 0;
            int wire_contiguousHits = 0;
            for(int time = 1; time <= names[k]->GetNbinsY(); time++){
                if(names[k]->GetBinContent(l, time) > 0) {
                    wire_contiguousHits++;
                }
                if(names[k]->GetBinContent(l, time) == 0 && names[k]->GetBinContent(l, time-1) > 0){
                    wire_contiguousCount[index] == wire_contiguousHits;
                    index ++;
                    numberClustersCount[k]->Fill(wire_contiguousHits); //size of clusters
                    wire_contiguousHits = 0;
                }
            }
        }
    }

    for (int k = 0; k <=11; k++){
    //to find total numbr of hits across all wires
    int total_hits = 0;
    for(int wire = 1; wire<=names[i]->GetNbinsX(); wire++){
        for(int time = 1; time <= names[k]->GetNbinsY(); time++){
            if(names[k]->GetBinContent(wire, time) > 0) total_hits++;
        }
    }
    //cout << "total number of hits across all wires : " << total_hits << endl;
    }
        
    new TCanvas();
    numberHitsHist->Draw();
    new TCanvas();
    numberClusters->Draw();
    new TCanvas();
    numberClustersCount->Draw();
        
    }
}
