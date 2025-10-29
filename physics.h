#ifndef PHYSICS_H
#define PHYSICS_H

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"

class DamsaPhysicsList : public G4VModularPhysicsList {
public:
    DamsaPhysicsList();
    virtual ~DamsaPhysicsList();
};

DamsaPhysicsList::DamsaPhysicsList() {
    RegisterPhysics (new G4EmStandardPhysics());
    RegisterPhysics (new G4OpticalPhysics());
    RegisterPhysics (new G4DecayPhysics());
    RegisterPhysics (new G4HadronPhysicsFTFP_BERT());
}

DamsaPhysicsList::~DamsaPhysicsList() {}

#endif
