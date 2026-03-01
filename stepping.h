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
    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    
    // Safety check for valid post-step point
    if(!postStepPoint) return;
    
    G4VPhysicalVolume* volume = postStepPoint->GetTouchableHandle()->GetVolume();
    if(!volume) return;
    
    G4StepStatus stepStatus = postStepPoint->GetStepStatus();
    
    // Only record particles when they cross a geometry boundary
    if(stepStatus != fGeomBoundary) return;
    
    // Get particle properties
    G4String particleName = track->GetDefinition()->GetParticleName();
    G4double energy = track->GetKineticEnergy();
    G4int trackID = track->GetTrackID();
    G4bool isPrimary = (track->GetParentID() == 0);  // Primary particle has parentID = 0
    
    // Check momentum direction - only count forward-moving particles (positive Z momentum)
    G4ThreeVector momentum = track->GetMomentumDirection();
    G4double cosTheta = momentum.z();  // Dot product with beam axis (0,0,1)
    
    // Skip backward-moving particles (momentum in -Z direction)
    if(cosTheta < 0) return;
    
    G4double angle = momentum.angle(G4ThreeVector(0, 0, 1));  // Angle from beam axis
    
    G4String volumeName = volume->GetName();
    
    // Target exit scoring plane
    if(volumeName == "physScoringVolumeTarget") {
        // Check if this track was already recorded at this location
        if(!DamsaAnalysis::Instance()->WasTrackRecorded(trackID, "TargetExit")) {
            DamsaAnalysis::Instance()->RecordParticle(particleName, energy, "TargetExit", angle, trackID, isPrimary);
        }
    }
    // Magnet entrance scoring plane
    else if(volumeName == "physScoringMagnetEntrance") {
        if(!DamsaAnalysis::Instance()->WasTrackRecorded(trackID, "MagnetEntrance")) {
            DamsaAnalysis::Instance()->RecordParticle(particleName, energy, "MagnetEntrance", angle, trackID, isPrimary);
        }
    }
    // Calorimeter entrance scoring plane
    else if(volumeName == "physScoringCaloEntrance") {
        if(!DamsaAnalysis::Instance()->WasTrackRecorded(trackID, "CaloEntrance")) {
            DamsaAnalysis::Instance()->RecordParticle(particleName, energy, "CaloEntrance", angle, trackID, isPrimary);
        }
    }
    // Calorimeter exit scoring plane
    else if(volumeName == "physScoringCaloExit") {
        if(!DamsaAnalysis::Instance()->WasTrackRecorded(trackID, "CaloExit")) {
            DamsaAnalysis::Instance()->RecordParticle(particleName, energy, "CaloExit", angle, trackID, isPrimary);
        }
    }
}

#endif
