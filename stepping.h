#ifndef STEPPING_H
#define STEPPING_H

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "analysis.h"

class DamsaSteppingAction : public G4UserSteppingAction
{
public:
    DamsaSteppingAction();
    virtual ~DamsaSteppingAction();
    
    virtual void UserSteppingAction(const G4Step* step);
};

DamsaSteppingAction::DamsaSteppingAction()
: G4UserSteppingAction()
{}

DamsaSteppingAction::~DamsaSteppingAction()
{}

void DamsaSteppingAction::UserSteppingAction(const G4Step* step)
{
    G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    
    if(volume->GetName() == "physScoringVolumeDetector") {
        G4Track* track = step->GetTrack();
        G4String particleName = track->GetDefinition()->GetParticleName();
        G4double energy = track->GetKineticEnergy();
        
        G4ThreeVector momentum = track->GetMomentumDirection();
        G4double angle = momentum.angle(G4ThreeVector(0, 0, 1));
        
        DamsaAnalysis::Instance()->RecordParticle(particleName, energy, "DetectorEntrance", angle);
    }
    else if(volume->GetName() == "physScoringVolumeTarget") {
        G4Track* track = step->GetTrack();
        G4String particleName = track->GetDefinition()->GetParticleName();
        G4double energy = track->GetKineticEnergy();
        
        G4ThreeVector momentum = track->GetMomentumDirection();
        G4double angle = momentum.angle(G4ThreeVector(0, 0, 1));
        
        DamsaAnalysis::Instance()->RecordParticle(particleName, energy, "TargetExit", angle);
    }
}

#endif
