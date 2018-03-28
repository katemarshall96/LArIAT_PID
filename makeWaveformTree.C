#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TAxis.h"

using namespace art;
using namespace std;
using namespace std::chrono;

void
makeWaveformTree(std::string const& filename, std::string const& outfile,
         int nevents=2)
{
  InputTag daq_tag{ "daq" };
  // Create a vector of length 1, containing the given filename.
  vector<string> filenames(1, filename);

  TFile *fpOut = TFile::Open(outfile.c_str(),"RECREATE");
  int eventNum;
  int channel;
  std::vector<int> samples;

  TTree *waveTree = new TTree("waveTree","Tree of Waveforms");
  waveTree->Branch("eventNum",&eventNum,"eventNum/I");
  waveTree->Branch("channel",&channel,"channel/I");
  waveTree->Branch("samples",&samples);

  int iev=0;
  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    if(iev>=nevents) break;
    std::cout << "Event " << iev << std::endl;
    // Look at the digits
    auto& digits =
      *ev.getValidHandle<vector<raw::RawDigit>>(daq_tag);
    for(auto&& digit: digits){
      eventNum=iev;
      channel=digit.Channel();
      samples.clear();
      for(auto&& sample: digit.ADCs()){
	samples.push_back(sample);
      }
      waveTree->Fill();
    } // end loop over digits (=?channels)
    ++iev;
  } // end loop over events
  waveTree->AutoSave();
}
