void numberNoise(){
    
    ifstream inputFileName;
    std::vector<double> Event;
    std::vector<double> Total;
    std::vector<double> Noise;
    std::vector<double> Signal;
    
    double temp1, temp2, temp3, temp4;
    inputFileName.open("numberNoise_Signal2.txt", ios::in);
    if (inputFileName.good())
    {
        inputFileName.seekg(0L, ios::beg);
        while (! inputFileName.eof())
        {
            inputFileName >> temp1 >> temp2 >> temp3 >> temp4;
            Event.push_back(temp1);
            Total.push_back(temp2);
            Noise.push_back(temp3);
            Signal.push_back(temp4);
        }
    }
    
    TCanvas * Canvas1 = new TCanvas();
    TH2F * window1 = new TH2F("window1","Plot of Noise and Signal Hits for Correlated Data", 33, 0, 33, 1500, 0, 1500);
    window1->GetXaxis()->SetTitle("Event Number");
    window1->GetYaxis()->SetTitle("Number of Hits");
    window1->SetStats(0);
    window1->Draw();
    TGraphErrors *gr1 = new TGraphErrors();
    gr1->SetName("gr1");
    gr1->SetMarkerColor(6);
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerSize(0.4);
    
    for(int i=0;i<Event.size();i++){
        gr1->SetPoint(i,Event[i],Noise[i]);
    }
    gr1->Draw("LP");
    
    
    TGraphErrors *gr2 = new TGraphErrors();
    gr2->SetName("gr2");
    gr2->SetMarkerColor(4);
    gr2->SetMarkerStyle(20);
    gr2->SetMarkerSize(0.4);
    
    for(int i=0;i<Event.size();i++){
        gr2->SetPoint(i,Event[i],Signal[i]);
        
    }
    gr2->Draw("LP same");
    
    TLegend * legend = new TLegend(0.9,0.8,0.7,0.9);
    legend->SetHeader("");
    legend->AddEntry("gr1","Noise Hits","p");
    legend->AddEntry("gr2","Signal Hits","p");
    legend->Draw("same");

}

