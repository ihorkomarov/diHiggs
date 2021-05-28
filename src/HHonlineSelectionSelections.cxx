#include "UHH2/HHonlineSelection/include/HHonlineSelectionSelections.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/Particle.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;
using namespace std;

namespace {  
template<typename T>
bool passes_selection(const vector<T> & particles, int nmin, int nmax=-1, double min_pt=0, double max_pt=-1){

    int n_particles = particles.size();
    bool passes_count =  n_particles >= nmin && (nmax < 0 || n_particles <= nmax);

    if(n_particles == 0)
        return passes_count;
    
    Particle last_significant_particle = particles.at(n_particles-1);
    bool passes_pt = last_significant_particle.pt() > min_pt && (last_significant_particle.pt() < max_pt || max_pt == -1);

    return passes_count && passes_pt;
}
}




NL1MuonSelection::NL1MuonSelection(int nmin_, int nmax_,  double min_pt_, double max_pt_): nmin(nmin_), nmax(nmax_), min_pt(min_pt_), max_pt(max_pt_){}

bool NL1MuonSelection::passes(const Event & event){
    return passes_selection(*event.L1M_seeds, nmin, nmax, min_pt, max_pt);
}

NL1EGammaSelection::NL1EGammaSelection(int nmin_, int nmax_,  double min_pt_, double max_pt_): nmin(nmin_), nmax(nmax_), min_pt(min_pt_), max_pt(max_pt_){}

bool NL1EGammaSelection::passes(const Event & event){
    return passes_selection(*event.L1EG_seeds, nmin, nmax, min_pt, max_pt);    
}

NL1JetSelection::NL1JetSelection(int nmin_, int nmax_, double min_pt_, double max_pt_) :
    nmin(nmin_), nmax(nmax_), min_pt(min_pt_), max_pt(max_pt_){}

bool NL1JetSelection::passes(const Event & event){
    return passes_selection(*event.L1J_seeds, nmin, nmax, min_pt, max_pt);
    
}

