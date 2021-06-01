#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
//hists
#include "UHH2/diHiggs/include/DiHiggsHists.h"

//Selections
#include "UHH2/common/include/NSelections.h"
#include "UHH2/diHiggs/include/diHiggsSelections.h"

//IDs
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/MuonIds.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {

class diHiggsModule: public AnalysisModule {
public:
    
    explicit diHiggsModule(Context & ctx);
    virtual bool process(Event & event) override;

private:
    
    std::unique_ptr<CommonModules> common;
    std::unique_ptr<Hists> noCuts;
    std::unique_ptr<Hists> n1Muon;
    std::unique_ptr<Hists> n3jet_singleM, n4jet_singleM;

    // Define Selections
    std::unique_ptr<Selection> muon_sel;
    std::unique_ptr<Selection> n3jet_sel, n4jet_sel;

    ElectronId Ele_Id;
    MuonId Mu_Id;
    // TauId Tau_Id;
    JetId Jet_Id;
};


diHiggsModule::diHiggsModule(Context & ctx){
    // In the constructor, the typical tasks are to initialize the
    // member variables, in particular the AnalysisModules such as
    // CommonModules or some cleaner module, Selections and Hists.
    // But you can do more and e.g. access the configuration, as shown below.
    
    cout << "Hello World from diHiggsModule!" << endl;
    
    // If needed, access the configuration of the module here, e.g.:
    string testvalue = ctx.get("TestKey", "<not set>");
    cout << "TestKey in the configuration was: " << testvalue << endl;
    
    for(auto & kv : ctx.get_all()){
        cout << " " << kv.first << " = " << kv.second << endl;
    }
    
    /// DEFINE Min Pt
    int ptCut_Mu = 10;
    int ptCut_Jet = 10;
    int ptCut_El = 0;
    double etaCut = 2.4;
    
    //Year year = extract_year(ctx);

    // Object IDs
    //cout << "Year: " << year << endl;
    JetId jet_pfid = JetPFID(JetPFID::WP_TIGHT_CHS);
    Ele_Id = AndId<Electron>(ElectronID_Fall17_tight, PtEtaCut(ptCut_El, etaCut));
    Mu_Id = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(ptCut_Mu, etaCut), MuonID(Muon::PFIsoTight));
    Jet_Id = AndId<Jet>(jet_pfid, PtEtaCut(ptCut_Jet, etaCut));


    // set common
    common.reset(new CommonModules());
    common->set_electron_id(Ele_Id);
    common->set_muon_id(Mu_Id);
    common->set_jet_id(Jet_Id);
    common->switch_jetPtSorter();
    common->init(ctx);
    
    // 2. set up selections
    //lepton selections
    muon_sel.reset(new NMuonSelection(1, -1));
    // jet selections
    n3jet_sel.reset(new NJetSelection(3, -1));
    n4jet_sel.reset(new NJetSelection(4, -1));
  


    // 3. Set up Hists classes:
    noCuts.reset(new DiHiggsHists(ctx, "NoCuts"));

    n1Muon.reset(new DiHiggsHists(ctx, "1Muon"));

    n3jet_singleM.reset(new DiHiggsHists(ctx, "3Jet_1M"));

    n4jet_singleM.reset(new DiHiggsHists(ctx, "4Jet_1M"));

}


bool diHiggsModule::process(Event & event) {
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

    // 2. test selections and fill histograms

    noCuts->fill(event);

    // event ID ausgeben
    bool muonCategory = muon_sel->passes(event);

    if(!muonCategory) return false; 
        
        n1Muon->fill(event);

    if(!n3jet_sel->passes(event)) return false;

        n3jet_singleM->fill(event);

    if(!n4jet_sel->passes(event)) return false;

        n4jet_singleM->fill(event);

    return true;
}

// as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
// make sure the diHiggsModule is found by class name. This is ensured by this macro:
UHH2_REGISTER_ANALYSIS_MODULE(diHiggsModule)

}
