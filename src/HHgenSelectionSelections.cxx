#include "UHH2/HHgenSelection/include/HHgenSelectionSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;

DiHiggsGenMuonSelection::DiHiggsGenMuonSelection(int nmin_ ): nmin(nmin_){}

bool DiHiggsGenMuonSelection::passes(const Event & event){

bool mounChannel = false;

std::vector <GenParticle> muons ;
std::vector <GenParticle>* allParticles = event.genparticles;
for(const GenParticle & gp : *allParticles){
    if(gp.pdgId() == 13 || gp.pdgId() == -13){
       // std::cout << "moun" << std::endl;
        muons.push_back(gp);
    }
}

std::vector <GenParticle> wBosons;
for(const GenParticle & m : muons){

    GenParticle mother = allParticles->at(m.mother1());

    if(mother.pdgId() == -24 || mother.pdgId() == 24 ){
        wBosons.push_back(mother);
    }
}

if (wBosons.size() == 1){
    GenParticle wMother = allParticles->at(wBosons.at(0).mother1());
    if(wMother.pdgId() == 25){
    mounChannel = true;
    }
}

return mounChannel;

}


