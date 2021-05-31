#pragma once

#include "UHH2/core/include/Hists.h"

namespace uhh2examples {

/**  \brief Example class for booking and filling histograms
 * 
 * NOTE: This class uses the 'hist' method to retrieve histograms.
 * This requires a string lookup and is therefore slow if you have
 * many histograms. Therefore, it is recommended to use histogram
 * pointers as member data instead, like in 'common/include/ElectronHists.h'.
 */
class DiHiggsHists: public uhh2::Hists {
public:
    // use the same constructor arguments as Hists for forwarding:
    DiHiggsHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;
    virtual ~DiHiggsHists();
protected:
    TH1F *n_L1mu, *pt_L1mu, *eta_L1mu, *phi_L1mu;
    TH1F *n_Mu,*pt_Mu, *eta_Mu, *phi_Mu;  

    TH1F *eta_L1jet1, *pt_L1jet1, *phi_L1jet1, *eta_L1jet2, *eta_L1jet3, *eta_L1jet4, *n_L1Jets;
    TH1F *eta_jet1, *pt_jet1, *phi_jet1, *eta_jet2, *eta_jet3, *eta_jet4, *n_Jets;

    TH1F *leading_L1Mu_pt, *leading_Mu_pt;
    TH1F *relative_error_Muon, *relative_error_Jet;
    TH2D *relative_error_Jet_2d, *relative_error_Muon_2d;
};

}
