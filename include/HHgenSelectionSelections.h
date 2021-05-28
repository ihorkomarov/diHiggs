#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/Selection.h"

namespace uhh2examples {
    
/* Select events with at least two jets in which the leading two jets have deltaphi > 2.7 and the third jet pt is
 * below 20% of the average of the leading two jets, where the minimum deltaphi and
 * maximum third jet pt fraction can be changed in the constructor.
 * The jets are assumed to be sorted in pt.
 */
class DiHiggsGenMuonSelection: public uhh2::Selection {
public:
    explicit DiHiggsGenMuonSelection(int nmin);
    virtual bool passes(const uhh2::Event & event) override;
private: 
int nmin;
};


}
