// To plot 2x2D plots of induction and collection planes...
void plot_2D(int eventNum=2);

void plot_2D(int eventNum){
    
    // Set some defaults
    gROOT->SetStyle("Plain");
    gStyle->SetCanvasBorderMode(0); // turn off canvas borders
    //gStyle->SetOptStat(0);
    
    // Open the file and read contents
    TFile* f = new TFile("corWaveTree.root");
    f->ls();
    TTree* wavetree = (TTree*) f->Get("waveTree");

    // Create a canvas, divide into two windows,
    TCanvas *c1 = new TCanvas("c1", "2D Plots of ADC Values", 800, 10, 600, 700);
    c1->Divide(1,2);
    
    // Plot induction plane
    c1->cd(1);
    char condition1[180];
    sprintf(condition1, "samples*(eventNum==%d)", eventNum);
    TH2F*h1=new TH2F("h1", "Induction Plane", 240, 0, 240, 3072, 0, 3072);
    h1->GetXaxis()->SetTitle("Wire Number");
    h1->GetYaxis()->SetTitle("Time");
    wavetree->Draw("Iteration$:channel>>h1", condition1,"colz");
    
    // Plot collection plane
    c1->cd(2);
    char condition2[180];
    sprintf(condition2, "samples*(eventNum==%d)", eventNum);
    TH2F*h2=new TH2F("h2", "Collection Plane", 240, 240, 480, 3072, 0, 3072);
    h2->GetXaxis()->SetTitle("Wire Number");
    h2->GetYaxis()->SetTitle("Time");
    wavetree->Draw("Iteration$:channel>>h2", condition2,"colz");
}
