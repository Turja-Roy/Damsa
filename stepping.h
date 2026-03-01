#ifndef STEPPING_H
#define STEPPING_H

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4StepStatus.hh"
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
    G4Track* track = step->GetTrack();
    G4VPhysicalVolume* volume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
    G4StepStatus stepStatus = step->GetPostStepPoint()->GetStepStatus();
    
    // Only record particles when they FIRST ENTER the scoring volume (boundary crossing)
    // PostStepPoint = where particle IS NOW (just entered the volume)
    if(stepStatus != fGeomBoundary) return;  // Not at a boundary, skip
    
    // Now record particles at boundaries
    if(volume->GetName() == "physScoringMagnetEntrance") {
        G4String particleName = track->GetDefinition()->GetParticleName();
        G4double energy = track->GetKineticEnergy();
        
        G4ThreeVector momentum = track->GetMomentumDirection();
        G4double angle = momentum.angle(G4ThreeVector(0, 0, 1));
        
        DamsaAnalysis::Instance()->RecordParticle(particleName, energy, "MagnetEntrance", angle);
    }
    else if(volume->GetName() == "physScoringCaloEntrance") {
        G4String particleName = track->GetDefinition()->GetParticleName();
        G4double energy = track->GetKineticEnergy();
        
        G4ThreeVector momentum = track->GetMomentumDirection();
        G4double angle = momentum.angle(G4ThreeVector(0, 0, 1));
        
        DamsaAnalysis::Instance()->RecordParticle(particleName, energy, "CaloEntrance", angle);
    }
    else if(volume->GetName() == "physScoringCaloExit") {
        G4String particleName = track->GetDefinition()->GetParticleName();
        G4double energy = track->GetKineticEnergy();
        
        G4ThreeVector momentum = track->GetMomentumDirection();
        G4double angle = momentum.angle(G4ThreeVector(0, 0, 1));
        
        DamsaAnalysis::Instance()->RecordParticle(particleName, energy, "CaloExit", angle);
    }
    else if(volume->GetName() == "physScoringVolumeTarget") {
        G4String particleName = track->GetDefinition()->GetParticleName();
        G4double energy = track->GetKineticEnergy();
        
        G4ThreeVector momentum = track->GetMomentumDirection();
        G4double angle = momentum.angle(G4ThreeVector(0, 0, 1));
        
        DamsaAnalysis::Instance()->RecordParticle(particleName, energy, "TargetExit", angle);
    }
}

#endif
