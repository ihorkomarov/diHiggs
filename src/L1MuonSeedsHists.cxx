#include "UHH2/diHiggs/include/L1MuonSeedsHists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

L1MuonSeedsHists::L1MuonSeedsHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here

  // leptons
  book<TH1F>("N_L1mu", "N^{#mu}", 10, 0, 10);
  book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV/c]", 40, 0, 200);
  book<TH1F>("eta_mu", "#eta^{#mu}", 40, -2.1, 2.1);
}


void L1MuonSeedsHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;

  int NL1muons = event.L1M_seeds->size();
  hist("N_L1mu")->Fill(NL1muons, weight);
  
  for (const L1Muon & thismuL1 : *event.L1M_seeds){
      hist("pt_mu")->Fill(thismuL1.pt(), weight);
      hist("eta_mu")->Fill(thismuL1.eta(), weight);
  }  
}

L1MuonSeedsHists::~L1MuonSeedsHists(){}
