void drawMeans(){
    
    //To read results from Mean_Results_Hits.txt file...
    ifstream inputFileName1;
    std::vector<double> SampleCut1;
    std::vector<double> Mean1;
    std::vector<double> Error1;
    
    double temp1, temp2, temp3;
    inputFileName1.open("Mean_Results_Hits.txt", ios::in);
    if (inputFileName1.good())
    {
        inputFileName1.seekg(0L, ios::beg);
        while (! inputFileName1.eof())
        {
            inputFileName1 >> temp1 >> temp2 >> temp3;
            SampleCut1.push_back(temp1);
            Mean1.push_back(temp2);
            Error1.push_back(temp3);
        }
    }
    
    //To read results from Mean_Results_Clusters.txt file...
    ifstream inputFileName2;
    std::vector<double> SampleCut2;
    std::vector<double> Mean2;
    std::vector<double> Error2;
    
    double temp4, temp5, temp6;
    inputFileName2.open("Mean_Results_Clusters.txt", ios::in);
    if (inputFileName2.good())
    {
        inputFileName2.seekg(0L, ios::beg);
        while (! inputFileName2.eof())
        {
            inputFileName2 >> temp4 >> temp5 >> temp6;
            SampleCut2.push_back(temp4);
            Mean2.push_back(temp5);
            Error2.push_back(temp6);
        }
    }
    
    //To read results from Mean_Results_Size.txt file...
    ifstream inputFileName3;
    std::vector<double> SampleCut3;
    std::vector<double> Mean3;
    std::vector<double> Error3;
    std::vector<double> Entries;
    
    double temp7, temp8, temp9, temp10;
    inputFileName3.open("Mean_Results_Size.txt", ios::in);
    if (inputFileName3.good())
    {
        inputFileName3.seekg(0L, ios::beg);
        while (! inputFileName3.eof())
        {
            inputFileName3 >> temp7 >> temp8 >> temp9 >> temp10;
            SampleCut3.push_back(temp7);
            Mean3.push_back(temp8);
            Error3.push_back(temp9);
            Entries.push_back(temp10);
        }
    }

    
    //To plot Means for Number of Hits per Wire...
    TCanvas * Canvas1 = new TCanvas();
    TH2F * window1 = new TH2F("window1","Plot of Means for Number of Hits per Wire", 42, -1, 41, 850, 50, 900);
    //set max y axis to 1500 to see in full
    window1->GetXaxis()->SetTitle("Sample Cut");
    window1->GetYaxis()->SetTitle("Mean");
    window1->SetStats(0);
    window1->Draw();
    TGraphErrors *gr1 = new TGraphErrors();
    gr1->SetMarkerColor(6);
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerSize(0.4);
    
    for(int i=0;i<SampleCut1.size();i++){
        gr1->SetPoint(i,SampleCut1[i],Mean1[i]);
        gr1->SetPointError(i,0,Error1[i]/sqrt(240));
    }
    
    gr1->Draw("P");
    
    
    //To plot Means for Number of Clusters per Wire...
    TCanvas * Canvas2 = new TCanvas();
    TH2F * window2 = new TH2F("window2","Plot of Means for Number of Clusters per Wire", 42, -1, 41, 5, 0, 5);
    window2->GetXaxis()->SetTitle("Sample Cut");
    window2->GetYaxis()->SetTitle("Mean");
    window2->SetStats(0);
    window2->Draw();
    TGraphErrors *gr2 = new TGraphErrors();
    gr2->SetMarkerColor(6);
    gr2->SetMarkerStyle(20);
    gr2->SetMarkerSize(0.4);
    
    for(int i=0;i<SampleCut2.size();i++){
        gr2->SetPoint(i,SampleCut2[i],Mean2[i]);
        gr2->SetPointError(i,0,Error2[i]/sqrt(240));
    }
    
    gr2->Draw("P");
    
    
    //To plot Means for Number of Hits per Cluster...
    TCanvas * Canvas3 = new TCanvas();
    TH2F * window3 = new TH2F("window3","Plot of Means for Number of Hits per Cluster", 42, -1, 41, 45, 5, 50);
    window3->GetXaxis()->SetTitle("Sample Cut");
    window3->GetYaxis()->SetTitle("Mean");
    window3->SetStats(0);
    window3->Draw();
    TGraphErrors *gr3 = new TGraphErrors();
    gr3->SetMarkerColor(6);
    gr3->SetMarkerStyle(20);
    gr3->SetMarkerSize(0.35);
    
    for(int i=0;i<SampleCut3.size();i++){
        gr3->SetPoint(i,SampleCut3[i],Mean3[i]);
        gr3->SetPointError(i,0,Error3[i]/sqrt(Entries[i]));
    }
    
    gr3->Draw("P");
    

    
}
