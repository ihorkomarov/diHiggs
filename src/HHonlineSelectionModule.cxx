#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/diHiggs/include/HHonlineSelectionSelections.h"
#include "UHH2/diHiggs/include/DiHiggsHists.h"

//IDs
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/MuonIds.h"


using namespace std;
using namespace uhh2;

namespace uhh2examples {

/** \brief Basic analysis example of an AnalysisModule (formerly 'cycle') in UHH2
 * 
 * This is the central class which calls other AnalysisModules, Hists or Selection classes.
 * This AnalysisModule, in turn, is called (via AnalysisModuleRunner) by SFrame.
 */
class HHonlineSelectionModule: public AnalysisModule {
public:
    
    explicit HHonlineSelectionModule(Context & ctx);
    virtual bool process(Event & event) override;

private:
    
    std::unique_ptr<CommonModules> common;
    
   
    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor,
    // to avoid memory leaks.
    std::unique_ptr<Selection> eGamma_sel, l1muon_sel, noEGamma_sel, noL1muon_sel;
    std::unique_ptr<Selection> n3L1jet_sel, n4L1jet_sel;
    
    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> noCuts, n1L1Muon;
    std::unique_ptr<Hists> n3L1jet_singleEGamma, n3L1jet_singleL1M, n4L1jet_singleEGamma, n4L1jet_singleL1M;

    ElectronId Ele_Id;
    MuonId Mu_Id;
    // TauId Tau_Id;
    JetId Jet_Id;
};


HHonlineSelectionModule::HHonlineSelectionModule(Context & ctx){

    cout << "Hello World from HHonlineSelectionModule!" << endl;
    
    // If needed, access the configuration of the module here, e.g.:
    string testvalue = ctx.get("TestKey", "<not set>");
    cout << "TestKey in the configuration was: " << testvalue << endl;
    
    for(auto & kv : ctx.get_all()){
        cout << " " << kv.first << " = " << kv.second << endl;
    }
     
    double ptCutMuon = 24.0;
    double ptCutJet = 30;

    double etaCut = 2.4;
    
    
    JetId jet_pfid = JetPFID(JetPFID::WP_TIGHT_CHS);
    Ele_Id = AndId<Electron>(ElectronID_Fall17_tight, PtEtaCut(ptCutMuon, etaCut));
    Mu_Id = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(ptCutMuon, etaCut), MuonID(Muon::PFIsoTight));
    Jet_Id = AndId<Jet>(jet_pfid, PtEtaCut(ptCutMuon, etaCut));

    common.reset(new CommonModules());
    common->set_electron_id(Ele_Id);
    common->set_muon_id(Mu_Id);
    common->set_jet_id(Jet_Id);
    common->switch_jetPtSorter();
    common->init(ctx);



    //DefineSelections 
    eGamma_sel.reset(new NL1EGammaSelection(1,1,ptCutMuon));
    l1muon_sel.reset(new NL1MuonSelection(1,-1,ptCutMuon));

    noEGamma_sel.reset(new NL1EGammaSelection(0,0,ptCutMuon));
    noL1muon_sel.reset(new NL1MuonSelection(0,0,ptCutMuon));

    n3L1jet_sel.reset(new NL1JetSelection(3, -1,ptCutJet));
    n4L1jet_sel.reset(new NL1JetSelection(4, -1,ptCutJet));

    // 3. Set up Hists classes:
    noCuts.reset(new DiHiggsHists(ctx, "NoCuts"));

    n1L1Muon.reset(new DiHiggsHists(ctx, "1L1Muon"));

    n3L1jet_singleL1M.reset(new DiHiggsHists(ctx, "3Jet_L1M"));

    n4L1jet_singleL1M.reset(new DiHiggsHists(ctx, "4Jet_L1M"));
}

bool HHonlineSelectionModule::process(Event & event) {
    // This is the main procedure, called for each event. Typically,
    // do some pre-processing by calling the modules' process method
    // of the modules constructed in the constructor (1).
    // Then, test whether the event passes some selection and -- if yes --
    // use it to fill the histograms (2).
    // Finally, decide whether or not to keep the event in the output (3);
    // this is controlled by the return value of this method: If it
    // returns true, the event is kept; if it returns false, the event
    // is thrown away.

    
    // 1. run all modules other modules.
    // Note that it returns a bool, that may be False
    // (e.g. Golden JSON, MET filters), and therefore user should return early
    bool commonResult = common->process(event);
    if (!commonResult) return false;

    noCuts->fill(event);

    bool l1MuonCategory = l1muon_sel->passes(event);
    
    if(!l1MuonCategory) return false;

        n1L1Muon->fill(event);
    
    if(!n3L1jet_sel->passes(event)) return false;

        n3L1jet_singleL1M->fill(event);
    
    if(!n4L1jet_sel->passes(event)) return false;

        n4L1jet_singleL1M->fill(event);

    return true;
}

// as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
// make sure the HHonlineSelectionModule is found by class name. This is ensured by this macro:
UHH2_REGISTER_ANALYSIS_MODULE(HHonlineSelectionModule)

}
