#ifndef PHYSICS_H
#define PHYSICS_H

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

class DamsaPhysicsList : public G4VModularPhysicsList {
public:
    DamsaPhysicsList();
    virtual ~DamsaPhysicsList();
};

DamsaPhysicsList::DamsaPhysicsList() {
    RegisterPhysics (new G4EmStandardPhysics());
    RegisterPhysics (new G4OpticalPhysics());
}

DamsaPhysicsList::~DamsaPhysicsList() {}

#endif
