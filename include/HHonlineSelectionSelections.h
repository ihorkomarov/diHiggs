#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/Selection.h"

namespace uhh2examples {
    
/* Select events with at least two jets in which the leading two jets have deltaphi > 2.7 and the third jet pt is
 * below 20% of the average of the leading two jets, where the minimum deltaphi and
 * maximum third jet pt fraction can be changed in the constructor.
 * The jets are assumed to be sorted in pt.
 */


// class ParticleSelection: public uhh2::Selection {
// public:
//     explicit ParticleSelection(const std::string& type, int nmin, int nmax = -1, double min_pt=0, double max_pt=-1);
//     virtual bool passes(const uhh2::Event & event);
// private:
//     int nmin, nmax, min_pt, max_pt, type;
// };

class NL1MuonSelection: public uhh2::Selection {
public:
    explicit NL1MuonSelection(int nmin, int nmax = -1, double min_pt=0, double max_pt=-1);
    virtual bool passes(const uhh2::Event & event);
private:
    int nmin, nmax, min_pt, max_pt;
};


class NL1EGammaSelection: public uhh2::Selection {
public:
    explicit NL1EGammaSelection(int nmin, int nmax = -1,  double min_pt=0, double max_pt=-1);
    virtual bool passes(const uhh2::Event & event);
private:
    int nmin, nmax, min_pt, max_pt;
};

class NL1JetSelection: public uhh2::Selection {
public:
    explicit NL1JetSelection(int nmin, int nmax = -1, double min_pt=0, double max_pt=-1);
    virtual bool passes(const uhh2::Event & event);
private:
    int nmin, nmax, min_pt, max_pt;
};
}
