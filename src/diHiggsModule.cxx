#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/diHiggs/include/diHiggsSelections.h"
#include "UHH2/diHiggs/include/muonSeedsHists.h"
#include "UHH2/diHiggs/include/L1MuonSeedsHists.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {

/** \brief Basic analysis example of an AnalysisModule (formerly 'cycle') in UHH2
 * 
 * This is the central class which calls other AnalysisModules, Hists or Selection classes.
 * This AnalysisModule, in turn, is called (via AnalysisModuleRunner) by SFrame.
 */
class diHiggsModule: public AnalysisModule {
public:
    
    explicit diHiggsModule(Context & ctx);
    virtual bool process(Event & event) override;

private:
    
    std::unique_ptr<CommonModules> common;
    
    // std::unique_ptr<JetCleaner> jetcleaner;
   
    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor,
    // to avoid memory leaks.
    
    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> muon_seeds;
    std::unique_ptr<Hists> muon_L1;
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
    
    // If running in SFrame, the keys "dataset_version", "dataset_type", "dataset_lumi",
    // and "target_lumi" are set to the according values in the xml file. For CMSSW, these are
    // not set automatically, but can be set in the python config file.
    for(auto & kv : ctx.get_all()){
        cout << " " << kv.first << " = " << kv.second << endl;
    }
    
    // 1. setup other modules. CommonModules and the JetCleaner:
    common.reset(new CommonModules());
    // TODO: configure common here, e.g. by 
    // calling common->set_*_id or common->disable_*
    common->init(ctx);

    
    // 2. set up selections
    // njet_sel.reset(new NJetSelection(2)); // see common/include/NSelections.h
    // dijet_sel.reset(new DijetSelection()); // see diHiggsSelections

    // 3. Set up Hists classes:
    muon_seeds.reset(new muonSeedsHists(ctx, "MuonSeeds"));
    muon_L1.reset(new muonSeedsHists(ctx, "L1MuonSeedsHists"));
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
    
    cout << "diHiggsModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    
    // 1. run all modules other modules.
    // Note that it returns a bool, that may be False
    // (e.g. Golden JSON, MET filters), and therefore user should return early
    bool commonResult = common->process(event);
    if (!commonResult) return false;

    // 2. test selections and fill histograms

    muon_seeds->fill(event);
    muon_L1->fill(event);
    
    // currently we want to collect all event thus always true 
    return true;
}

// as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
// make sure the diHiggsModule is found by class name. This is ensured by this macro:
UHH2_REGISTER_ANALYSIS_MODULE(diHiggsModule)

}
