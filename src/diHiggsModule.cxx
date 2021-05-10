#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
//hists
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/diHiggs/include/offlineObjectsHists.h"
#include "UHH2/diHiggs/include/L1MuonSeedsHists.h"
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
    std::unique_ptr<Hists> n1Electron, n1Muon;
    std::unique_ptr<Hists> n3jet_singleE,n3jet_singleM, n4jet_singleE, n4jet_singleM;
    std::unique_ptr<Hists> n1B_singleE, n1B_singleM, n2B_singleE, n2B_singleM;

    // Define Selections
    std::unique_ptr<Selection> electron_sel, muon_sel, noElectron_sel, noMuon_sel;
    std::unique_ptr<Selection> n3jet_sel, n4jet_sel;
    std::unique_ptr<Selection> n1B_sel, n2B_sel;

    ElectronId Ele_Id;
    MuonId Mu_Id;
    // TauId Tau_Id;
    JetId Jet_Id;

    BTag::algo btag_algo;
    BTag::wp wp_btag_medium;
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
    
    double ptCut = 10;
    double etaCut = 4;
    
    //Year year = extract_year(ctx);

    // Object IDs
    //cout << "Year: " << year << endl;
    JetId jet_pfid = JetPFID(JetPFID::WP_TIGHT_CHS);
    Ele_Id = AndId<Electron>(ElectronID_Fall17_tight, PtEtaCut(ptCut, etaCut));
    Mu_Id = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(ptCut, etaCut), MuonID(Muon::PFIsoTight));
    Jet_Id = AndId<Jet>(jet_pfid, PtEtaCut(ptCut, etaCut));

    // BTagging
    btag_algo = BTag::DEEPJET;
    wp_btag_medium = BTag::WP_MEDIUM;

    JetId DeepjetMedium = BTag(btag_algo, wp_btag_medium);

    // set common
    common.reset(new CommonModules());
    common->set_electron_id(Ele_Id);
    common->set_muon_id(Mu_Id);
    common->set_jet_id(Jet_Id);
    common->switch_jetPtSorter();
    common->init(ctx);
    
    // 2. set up selections
    //lepton selections
    electron_sel.reset(new NElectronSelection(1, 1));
    muon_sel.reset(new NMuonSelection(1, 1));
    noMuon_sel.reset(new NMuonSelection(0, 0));
    noElectron_sel.reset(new NElectronSelection(0, 0));
    // jet selections
    n3jet_sel.reset(new NJetSelection(3, -1));
    n4jet_sel.reset(new NJetSelection(4, -1));
    n1B_sel.reset(new NJetSelection(1,-1, DeepjetMedium));
    n2B_sel.reset(new NJetSelection(2,-1, DeepjetMedium));
  


    // 3. Set up Hists classes:
    noCuts.reset(new offlineObjectsHists(ctx, "NoCuts"));

    n1Electron.reset(new offlineObjectsHists(ctx, "1Electron"));
    n1Muon.reset(new offlineObjectsHists(ctx, "1Muon"));

    n3jet_singleE.reset(new offlineObjectsHists(ctx, "3Jet_1E"));
    n3jet_singleM.reset(new offlineObjectsHists(ctx, "3Jet_1M"));

    n4jet_singleE.reset(new offlineObjectsHists(ctx, "4Jet_1E"));
    n4jet_singleM.reset(new offlineObjectsHists(ctx, "4Jet_1M"));

    n1B_singleE.reset(new offlineObjectsHists(ctx, "1B_1E"));
    n1B_singleM.reset(new offlineObjectsHists(ctx, "1B_1M"));

    n2B_singleE.reset(new offlineObjectsHists(ctx, "2B_1E"));
    n2B_singleM.reset(new offlineObjectsHists(ctx, "2B_1M"));
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
    bool electronCategory = electron_sel->passes(event) && noMuon_sel->passes(event); // N_e == 1, N_mu == 0
    bool muonCategory = muon_sel->passes(event) && noElectron_sel->passes(event); // N_e == 0, N_mu == 1
    bool singleLeptonCategory = electronCategory || muonCategory;

    if(!singleLeptonCategory) return false; 

    if(electronCategory)
        n1Electron->fill(event);
    else if(muonCategory)
        n1Muon->fill(event);

    if(!n3jet_sel->passes(event)) return false;

    if(electronCategory)
        n3jet_singleE->fill(event);
    else if(muonCategory)
        n3jet_singleM->fill(event);


    if(!n4jet_sel->passes(event)) return false;

    if(electronCategory)
        n4jet_singleE->fill(event);
    else if(muonCategory)
        n4jet_singleM->fill(event);  

    if(!n1B_sel->passes(event)) return false;

    if(electronCategory)
        n1B_singleE->fill(event);
    else if(muonCategory)
        n1B_singleM->fill(event);  
    
    if(!n2B_sel->passes(event)) return false;

    if(electronCategory)
        n2B_singleE->fill(event);
    else if(muonCategory)
        n2B_singleM->fill(event);  

    return true;
}

// as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
// make sure the diHiggsModule is found by class name. This is ensured by this macro:
UHH2_REGISTER_ANALYSIS_MODULE(diHiggsModule)

}
