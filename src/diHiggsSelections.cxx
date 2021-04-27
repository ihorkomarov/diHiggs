#include "UHH2/diHiggs/include/diHiggsSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;


DijetSelection::DijetSelection(float dphi_min_, float third_frac_max_): dphi_min(dphi_min_), third_frac_max(third_frac_max_){}
    
bool DijetSelection::passes(const Event & event){
    assert(event.jets); // if this fails, it probably means jets are not read in
    if(event.jets->size() < 2) return false;
    const auto & jet0 = event.jets->at(0);
    const auto & jet1 = event.jets->at(1);
    auto dphi = deltaPhi(jet0, jet1);
    if(dphi < dphi_min) return false;
    if(event.jets->size() == 2) return true;
    const auto & jet2 = event.jets->at(2);
    auto third_jet_frac = jet2.pt() / (0.5 * (jet0.pt() + jet1.pt()));
    return third_jet_frac < third_frac_max;
}

StSelection::StSelection(double st_min_, double st_max_):st_min(st_min_), st_max(st_max_) {}
bool StSelection::passes(const Event & event) {
  auto met = event.met->pt();

  bool pass = false;
  double st = 0.0;
  double st_jets = 0.0;
  double st_lep = 0.0;

  for(const auto & jet : *event.jets){
    st_jets += jet.pt();
  }
  for(const auto & electron : *event.electrons){
    st_lep += electron.pt();
  }
  for(const auto & muon : *event.muons){
    st_lep += muon.pt();
  }

  st = st_lep + st_jets + met;

  pass = st > st_min && (st_max < 0 || st < st_max);

  return pass;
}
