#include "FFTtools.h"
#include <cassert>
#include <cmath>
#include <list>

//change values of cut as necessary

void drawingEvents(){

    // Set some defaults
    gROOT->SetStyle("Plain");
    gStyle->SetCanvasBorderMode(0);
    //gStyle->SetOptStat(0);
    
    // Open the file and read contents
    TFile* f = new TFile("cor_6326.root");
    f->ls();
    TTree* waveTree = (TTree*) f->Get("waveTree");
    
    TGraph *grRaw[24][1000];
    TGraph *gr[24][1000];
    TGraph *grFFT[24][1000];
    TGraph *grCor[24][1000];

    TH2F * cut2DRaw[1525]; //change back to 1525 for out_6326
    TH1F * numberHitsHist = new TH1F("numberHitsHist", "Number of Hits per Wire", 600, 0, 600);
    numberHitsHist->GetXaxis()->SetTitle("Number of Hits");
    TH1F * numberClusters = new TH1F("numberClusters", "Number of Clusters per Wire", 15, 0, 15);
    numberClusters->GetXaxis()->SetTitle("Number of Clusters");
    TH1F * numberClustersCount = new TH1F("numberClustersCount", "Number of Hits per Cluster", 200, 0, 200);
    numberClustersCount->GetXaxis()->SetTitle("Number of Hits");
    TH2F * noClusters_andHits = new TH2F("noClusters_andHits", "Number of Clusters and Size", 15, 0, 15, 600, 0, 600);
    noClusters_andHits->GetXaxis()->SetTitle("Number of Clusters");
    noClusters_andHits->GetYaxis()->SetTitle("Number of Hits");
    int numberTotalClusters[1525];
    int numberTotalHits[1525];
    int n[1525];
    
    char histName[180];
    char grName[180];
    char plotArg[180];
    char plotCond[180];
    sprintf(plotArg,"samples:Iteration$");
    for (int k = 1; k <= 1525; k++){ //change back to 1 to 1525
        sprintf(histName, "cut2DRaw_%d", k);
        cout << histName << endl;
        cut2DRaw[k] = new TH2F(histName, histName, 240, 240, 480, 3072, 0, 3072);
        sprintf(plotCond, "samples*(eventNum==%d && samples>14)", k); //samples>6
        waveTree->Project(histName, "Iteration$:channel", plotCond, "", 480, 480*k);
        
    //new TCanvas();
    //cut2DRaw[k]->Draw("colz");
    //} //comment out
    
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
            
            /*if (wire_contiguous && wire_hits > 0 ){
                for(int x = 1; x <= numberClusters->GetNbinsX(); x++){
                    for(int y = 1; y <= numberClusters->GetNbinsY(); y++){
                        noClusters_andHits->SetBinContent(x, y, wire_hits);
                    }
                }
            }*/
            
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
                    if(wire_contiguousHits<=21.398450){
                    numberClustersCount->Fill(wire_contiguousHits); //size of clusters
                    }
                    wire_contiguousHits = 0;
                }
            }
        }
        
        
        //to find total number of hits across all wires
        /*int total_hits = 0;
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
        n[k-1]=k;*/

    }
    
        /*new TCanvas();
        numberHitsHist->Draw();
        TF1*fitNoise1 = new TF1("fitNoise1", "expo", 0.9, 10);
        numberHitsHist->Fit(fitNoise1, "R");
        fitNoise1->SetLineColor(kBlue);
        fitNoise1->Draw("same");
        TF1* fitSignal1 = new TF1("fitSignal1", "gaus", 55, 78);
        numberHitsHist->Fit(fitSignal1, "R");
        fitSignal1->SetLineColor(kRed);
        fitSignal1->Draw("same");
    
        new TCanvas();
        numberClusters->Draw();*/
    
        new TCanvas();
        numberClustersCount->Draw();
        TF1*fitNoise2 = new TF1("fitNoise2", "expo", 0.9, 20);
        numberClustersCount->Fit(fitNoise2, "R");
        fitNoise2->SetLineColor(kBlue);
        fitNoise2->Draw("same");
        TF1*fitSignal2 = new TF1("fitSignal2", "gaus", 34, 58);
        numberClustersCount->Fit(fitSignal2, "R");
        fitSignal2->SetLineColor(kRed);
        fitSignal2->Draw("same"); 
    
        //new TCanvas();
        //noClusters_andHits->Draw();
    
        /*TGraph * numberTotalHitsGraph = new TGraph(1525, n, numberTotalHits);
        new TCanvas();
        numberTotalHitsGraph->SetTitle("Total Number of Hits");
        numberTotalHitsGraph->GetXaxis()->SetTitle("Event Number");
        numberTotalHitsGraph->GetYaxis()->SetTitle("Number of Hits");
        numberTotalHitsGraph->Draw("A*");
    
        TGraph * numberTotalClustersGraph = new TGraph(1525, n, numberTotalClusters);
        new TCanvas();
        numberTotalClustersGraph->SetTitle("Total Number of Clusters");
        numberTotalClustersGraph->GetXaxis()->SetTitle("Event Number");
        numberTotalClustersGraph->GetYaxis()->SetTitle("Number of Clusters");
        numberTotalClustersGraph->Draw("A*");*/
    
}
