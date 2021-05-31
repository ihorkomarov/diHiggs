#include "UHH2/diHiggs/include/DiHiggsHists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include "TH2F.h"
#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

namespace {

  float inv_mass(const LorentzVector & p4)
        { 
          return p4.isTimelike() ? p4.mass() : -sqrt(-p4.mass2());
        }

}

DiHiggsHists::DiHiggsHists(Context & ctx, const string & dirname): Hists(ctx, dirname){

  int etaRange = 4;
  int ptMin = 0;
  int ptMax = 2000;
  n_L1Jets = book<TH1F>("N_L1jets", "N_{jets}", 20, 0, 20);  
  n_Jets = book<TH1F>("N_jets", "N_{jets}", 20, 0, 20); 

 
  eta_jet1 = book<TH1F>("eta_jet1", "#eta^{jet 1}", 40, -etaRange, etaRange);
  pt_jet1  = book<TH1F>("pt_jet1", "p_{T}^{jet 1}", 40, ptMin, ptMax);
  eta_jet2 = book<TH1F>("eta_jet2", "#eta^{jet 2}", 40, -etaRange, etaRange);
  eta_jet3 = book<TH1F>("eta_jet3", "#eta^{jet 3}", 40, -etaRange, etaRange);
  eta_jet4 = book<TH1F>("eta_jet4", "#eta^{jet 4}", 40, -etaRange, etaRange);


  eta_L1jet1 = book<TH1F>("eta_L1jet1", "#eta^{jet 1}", 40, -etaRange, etaRange);
  pt_L1jet1  = book<TH1F>("pt_L1jet1", "p_{T}^{jet 1}", 40, ptMin, ptMax);
  eta_L1jet2 = book<TH1F>("eta_L1jet2", "#eta^{jet 2}", 40, -etaRange, etaRange);
  eta_L1jet3 = book<TH1F>("eta_L1jet3", "#eta^{jet 3}", 40, -etaRange, etaRange);
  eta_L1jet4 = book<TH1F>("eta_L1jet4", "#eta^{jet 4}", 40, -etaRange, etaRange);

  int errorRange = 100;

  relative_error_Muon = book<TH1F>("relative_error_mu", "#delta_{#mu}", 40, -errorRange, errorRange);
  relative_error_Jet = book<TH1F>("relative_error_jet", "#delta_{jet}", 40, -errorRange, errorRange);

  relative_error_Muon_2d = book<TH2D>("relative_error_mu_2d", "leading muon p_T", 40, 0, 300, 40, 0, 300);
  relative_error_Jet_2d = book<TH2D>("relative_error_jet_2d", "leading jet p_T", 40, 0, 300, 40, 0, 800);

  // leptons
  //reco_mu
  n_Mu = book<TH1F>("N_mu", "N^{#mu}", 10, 0, 10);
  pt_Mu = book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV/c]", 40, ptMin, ptMax);
  eta_Mu = book<TH1F>("eta_mu", "#eta^{#mu}", 40, -etaRange, etaRange);
  //L1_mu
  n_L1mu = book<TH1F>("N_L1mu", "N^{#mu}", 10, 0, 10);
  pt_L1mu = book<TH1F>("pt_L1mu", "p_{T}^{#mu} [GeV/c]", 40, ptMin, ptMax);
  eta_L1mu = book<TH1F>("eta_L1mu", "#eta^{#mu}", 40, -etaRange, etaRange);
  //book<TH1F>("reliso_mu", "#mu rel. Iso", 40, 0, 0.5);
  //reco_electrons

  leading_L1Mu_pt = book<TH1F>("leading_L1mu_pt", "p_{T}^{#L1mu} [GeV/c]", 40, ptMin, ptMax);
  leading_Mu_pt = book<TH1F>("leading_Mu_pt", "p_{T}^{#mu} [GeV/c]", 40, ptMin, ptMax);


  //mHH plots 

  m_HH = book<TH1F>("m_HH", "mHH", 40, 200, 800);
  m_HH_2d = book<TH2D>("mHH_leading_mu", "mHH to leading L1muon p_T", 40, 200, 800, 40, 0, 300);

  //book<TH1F>("reliso_e", "#e rel. Iso", 40, 0, 0.5);
  //ph
  // book<TH1F>("N_ph", "N^{#ph}", 10, 0, 10);
  // book<TH1F>("pt_ph", "p_{T}^{#ph} [GeV/c]", 40, 0, 200);
  // book<TH1F>("eta_ph", "#eta^{#ph}", 40, -2.1, 2.1);
  //book<TH1F>("reliso_ph", "#ph rel. Iso", 40, 0, 0.5);

  // primary vertices
  book<TH1F>("N_pv", "N^{PV}", 50, 0, 50);
}


void DiHiggsHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.



  std::vector <GenParticle> nHiggs;
  std::vector <GenParticle>* allParticles = event.genparticles;

  double weight = event.weight;
  
  std::vector<L1Jet>* l1jets = event.L1J_seeds;
  std::vector<L1Muon>* l1muons = event.L1M_seeds;

  std::vector<Jet>* jets = event.jets;
  std::vector<Muon>* muons = event.muons;

  //Gen Matching first 

  for(const GenParticle & gp : *allParticles){
      if(gp.pdgId() == 25){
          nHiggs.push_back(gp);
      }
  }

if (!nHiggs.empty()){
  float mHH = inv_mass(nHiggs.at(0).v4()+nHiggs.at(1).v4());

  if(nHiggs.size() == 2 && !l1muons->empty()){
    m_HH->Fill(mHH, weight);
    m_HH_2d->Fill(mHH,l1muons->at(0).pt());
  }
}
  if (!l1muons->empty() && !muons->empty()){

    double PtOfLeadingMuon = muons->at(0).pt();
    double PtOfLeadingL1Muon = l1muons->at(0).pt();

    double relativeErrorMuon = (PtOfLeadingMuon-PtOfLeadingL1Muon)*100/PtOfLeadingMuon;

    relative_error_Muon_2d->Fill(PtOfLeadingMuon,PtOfLeadingL1Muon);
    relative_error_Muon->Fill(relativeErrorMuon);
  }

  if (!l1jets->empty() && !jets->empty()){

    double PtOfLeadingJet = jets->at(0).pt();
    double PtOfLeadingL1Jet = l1jets->at(0).pt();

    double relativeErrorJet = (PtOfLeadingJet-PtOfLeadingL1Jet)*100/PtOfLeadingJet;

    relative_error_Jet_2d->Fill(PtOfLeadingJet,PtOfLeadingL1Jet);
    relative_error_Jet->Fill(relativeErrorJet);
  }


  //Fill leading Mu Pt_s
  if (!l1muons->empty()) 
    leading_L1Mu_pt->Fill(l1muons->at(0).pt(),weight);
  if (!muons->empty())
    leading_Mu_pt->Fill(muons->at(0).pt(),weight);

 //Fill L1 Jets
  int NL1jets = l1jets->size();
  n_L1Jets->Fill(NL1jets, weight);
  // if(!event.isRealData)  hist("N_PU")->Fill(event.genInfo->pileup_TrueNumInteractions(), weight);

  if(NL1jets>=1){
    eta_L1jet1->Fill(l1jets->at(0).eta(), weight);
    pt_L1jet1->Fill(l1jets->at(0).pt(), weight);
  }
  if(NL1jets>=2){
    eta_L1jet2->Fill(l1jets->at(1).eta(), weight);
  }
  if(NL1jets>=3){
    eta_L1jet3->Fill(l1jets->at(2).eta(), weight);
  }
  if(NL1jets>=4){
    eta_L1jet4->Fill(l1jets->at(3).eta(), weight);
  }

 // Fill Jet Collection
  int Njets = jets->size();
  n_Jets->Fill(Njets, weight);
  // if(!event.isRealData)  hist("N_PU")->Fill(event.genInfo->pileup_TrueNumInteractions(), weight);

  if(Njets>=1){
    eta_jet1->Fill(jets->at(0).eta(), weight);
    pt_jet1->Fill(jets->at(0).pt(), weight);
  }
  if(Njets>=2){
    eta_jet2->Fill(jets->at(1).eta(), weight);
  }
  if(Njets>=3){
    eta_jet3->Fill(jets->at(2).eta(), weight);
  }
  if(Njets>=4){
    eta_jet4->Fill(jets->at(3).eta(), weight);
  }

  //fill L1_mu

  int NL1Muons = l1muons->size();

  n_L1mu->Fill(NL1Muons, weight);
  for (const L1Muon & thisL1mu : *l1muons){
      pt_L1mu->Fill(thisL1mu.pt(), weight);
      eta_L1mu->Fill(thisL1mu.eta(), weight);
  }

  //fill Muon Collection

    int NMuons = muons->size();

    n_Mu->Fill(NMuons, weight);
    for (const Muon & thismu : *muons){
        pt_Mu->Fill(thismu.pt(), weight);
        eta_Mu->Fill(thismu.eta(), weight);
    }

 


  int Npvs = event.pvs->size();
  hist("N_pv")->Fill(Npvs, weight);
}

DiHiggsHists::~DiHiggsHists(){}
