#include "UHH2/diHiggs/include/offlineObjectsHists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

offlineObjectsHists::offlineObjectsHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  int etaRange = 2.4;
  int ptMin = 0;
  int ptMax = 2000;
  book<TH1F>("N_jets", "N_{jets}", 20, 0, 20);  
  book<TH1F>("N_PU", "N_{PU}", 100, 0, 100);  
  eta_jet1 = book<TH1F>("eta_jet1", "#eta^{jet 1}", 40, -etaRange, etaRange);
  pt_jet1 = book<TH1F>("pt_jet1", "p_{T}^{jet 1}", 40, ptMin, ptMax);
  book<TH1F>("eta_jet2", "#eta^{jet 2}", 40, -etaRange, etaRange);
  book<TH1F>("eta_jet3", "#eta^{jet 3}", 40, -etaRange, etaRange);
  book<TH1F>("eta_jet4", "#eta^{jet 4}", 40, -etaRange, etaRange);


  // leptons
  //mu
  book<TH1F>("N_mu", "N^{#mu}", 10, 0, 10);
  book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV/c]", 40, ptMin, ptMax);
  book<TH1F>("eta_mu", "#eta^{#mu}", 40, -etaRange, etaRange);
  //book<TH1F>("reliso_mu", "#mu rel. Iso", 40, 0, 0.5);
  //e
  // book<TH1F>("N_e", "N^{#e}", 10, 0, 10);
  // book<TH1F>("pt_e", "p_{T}^{#e} [GeV/c]", 40, ptMin, ptMax);
  // book<TH1F>("eta_e", "#eta^{#e}", 40, -etaRange, etaRange);
  //book<TH1F>("reliso_e", "#e rel. Iso", 40, 0, 0.5);

  // primary vertices
  book<TH1F>("N_pv", "N^{PV}", 50, 0, 50);
}


void offlineObjectsHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;
  
  std::vector<Jet>* jets = event.jets;
  int Njets = jets->size();
  hist("N_jets")->Fill(Njets, weight);
  if(!event.isRealData)  hist("N_PU")->Fill(event.genInfo->pileup_TrueNumInteractions(), weight);

  if(Njets>=1){
    eta_jet1->Fill(jets->at(0).eta(), weight);
    pt_jet1->Fill(jets->at(0).pt(), weight);
  }
  if(Njets>=2){
    hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
  }
  if(Njets>=3){
    hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
  }
  if(Njets>=4){
    hist("eta_jet4")->Fill(jets->at(3).eta(), weight);
  }

  //fill mu
  int Nmuons = event.muons->size();
  hist("N_mu")->Fill(Nmuons, weight);
  for (const Muon & thismu : *event.muons){
      hist("pt_mu")->Fill(thismu.pt(), weight);
      hist("eta_mu")->Fill(thismu.eta(), weight);
      //hist("reliso_mu")->Fill(thismu.relIso(), weight);
  }

  // //fill e
  // int Nelectrons = event.electrons->size();
  // hist("N_e")->Fill(Nelectrons, weight);
  // for (const Electron & thisE : *event.electrons){
  //     hist("pt_e")->Fill(thisE.pt(), weight);
  //     hist("eta_e")->Fill(thisE.eta(), weight);
  //     //hist("reliso_e")->Fill(thisE.relIso(), weight);
  // }

  int Npvs = event.pvs->size();
  hist("N_pv")->Fill(Npvs, weight);
}

offlineObjectsHists::~offlineObjectsHists(){}
