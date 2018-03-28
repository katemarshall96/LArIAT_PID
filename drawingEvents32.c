

#include "FFTtools.h"
#include <cassert>
#include <cmath>
#include <list>

//change values of cut as necessary

void drawingEvents32(){
    
    // Set some defaults
    gROOT->SetStyle("Plain");
    gStyle->SetCanvasBorderMode(0);
    //gStyle->SetOptStat(0);
    
    // Open the file and read contents
    TFile* f = new TFile("corWaveTree.root");
    f->ls();
    TTree* waveTree = (TTree*) f->Get("waveTree");
    
    TGraph *grRaw[24][1000];
    TGraph *gr[24][1000];
    TGraph *grFFT[24][1000];
    TGraph *grCor[24][1000];
    
    TH2F * cut2DRaw[32];     TH1F * numberHitsHist = new TH1F("numberHitsHist", "Number of Hits per Wire: Correlated Data", 1525, 0, 1525);
    numberHitsHist->GetXaxis()->SetTitle("Number of Hits");
    TH1F * numberClusters = new TH1F("numberClusters", "Number of Clusters per Wire: Correlated Data", 18, 0, 18);
    numberClusters->GetXaxis()->SetTitle("Number of Clusters");
    TH1F * numberClustersCount = new TH1F("numberClustersCount", "Number of Hits per Cluster: Correlated Data", 500, 0, 500);
    numberClustersCount->GetXaxis()->SetTitle("Number of Hits");
    int numberTotalClusters[32];
    int numberTotalHits[32];
    int n[32];
    
    char histName[180];
    char grName[180];
    char plotArg[180];
    char plotCond[180];
    sprintf(plotArg,"samples:Iteration$");
    for (int k = 1; k <= 32; k++){ //change back to 1 to 1525
        sprintf(histName, "cut2DRaw_%d", k);
        cout << histName << endl;
        cut2DRaw[k] = new TH2F(histName, histName, 240, 240, 480, 3072, 0, 3072);
        sprintf(plotCond, "samples*(eventNum==%d && samples>21)", k); //try 16
        waveTree->Project(histName, "Iteration$:channel", plotCond);
        
        //new TCanvas();
        //cut2DRaw[k]->Draw("colz");

        // to make graphs for total number of hits and clusters per wire
        for(int i = 1; i <= cut2DRaw[k]->GetNbinsX() ; i++){
            int wire_hits = 0;
            int wire_contiguous = 0;
            int wire_lasthit = 0;
            for(int time = 1; time <= cut2DRaw[k]->GetNbinsY(); time++){
                if(cut2DRaw[k]->GetBinContent(i, time) > 0) {
                    wire_hits++;
                    if(!wire_lasthit) //if last wire was not hit (i.e. new cluster)
                        wire_contiguous++; //number of clusters
                    wire_lasthit = 1;
                } else wire_lasthit = 0;
            }
            
            if (wire_contiguous > 0){
                numberClusters->Fill(wire_contiguous); // number of clusters per wire
            }
            
            if (wire_hits > 0){
                numberHitsHist->Fill(wire_hits); // number of hits per wire
            }
            
            //to find size of each hit "cluster" i.e. number of time slices
            int wire_contiguousCount[15];
            int index = 0;
            int wire_contiguousHits = 0;
            for(int time = 1; time <= cut2DRaw[k]->GetNbinsY(); time++){
                if(cut2DRaw[k]->GetBinContent(i, time) > 0) {
                    wire_contiguousHits++;
                }
                if(cut2DRaw[k]->GetBinContent(i, time) == 0 && cut2DRaw[k]->GetBinContent(i, time-1) > 0){
                    wire_contiguousCount[index] == wire_contiguousHits;
                    index ++;
                    //if(wire_contiguousHits<=49.253075){
                    numberClustersCount->Fill(wire_contiguousHits); //size of clusters
                    //}
                    wire_contiguousHits = 0;
                }
            }
        }
        
        
        //to find total number of hits across all wires
        int total_hits = 0;
        int cluster_hits = 0;
        int last_hit = 0;
        for(int wire = 1; wire<=cut2DRaw[k]->GetNbinsX(); wire++){
            for(int time = 1; time <= cut2DRaw[k]->GetNbinsY(); time++){
                if(cut2DRaw[k]->GetBinContent(wire, time) > 0) { total_hits++;
                    if(!last_hit) //if last wire was not hit (i.e. new cluster)
                        cluster_hits++; //number of clusters
                    last_hit = 1;
                } else last_hit = 0;
            }
        }
        
        numberTotalClusters[k-1]=cluster_hits;
        numberTotalHits[k-1]=total_hits;
        n[k-1]=k;
        
       // cout << "total clusters : " << cluster_hits << endl;
       // cout << "total hits : " << total_hits << endl;

        
    }
    
    // drawing
    new TCanvas();
    numberHitsHist->Draw();
    new TCanvas();
    numberClusters->Draw();
    new TCanvas();
    numberClustersCount->Draw();
    
    TGraph * numberTotalHitsGraph = new TGraph(32, n, numberTotalHits);
    new TCanvas();
    numberTotalHitsGraph->SetTitle("Total Number of Hits");
    numberTotalHitsGraph->GetXaxis()->SetTitle("Event Number");
    numberTotalHitsGraph->GetYaxis()->SetTitle("Number of Hits");
    numberTotalHitsGraph->Draw("A*");
    
    TGraph * numberTotalClustersGraph = new TGraph(32, n, numberTotalClusters);
    new TCanvas();
    numberTotalClustersGraph->SetTitle("Total Number of Clusters");
    numberTotalClustersGraph->GetXaxis()->SetTitle("Event Number");
    numberTotalClustersGraph->GetYaxis()->SetTitle("Number of Clusters");
    numberTotalClustersGraph->Draw("A*");
    
     /*TF1* fitClusterSize = new TF1("fitFunction", "gaus", 45, 130);
     numberClustersCount->Fit(fitClusterSize, "R");
     fitClusterSize->SetLineColor(kRed);
     fitClusterSize->Draw("same");*/
}
