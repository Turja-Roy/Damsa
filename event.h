#ifndef EVENT_H
#define EVENT_H

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4RunManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "analysis.h"

class DamsaEventAction : public G4UserEventAction
{
public:
    DamsaEventAction();
    virtual ~DamsaEventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
};

DamsaEventAction::DamsaEventAction()
{}

DamsaEventAction::~DamsaEventAction()
{}

void DamsaEventAction::BeginOfEventAction(const G4Event*)
{
    // Reset per-event track ID sets to prevent cross-event contamination
    DamsaAnalysis::Instance()->ResetEventTracking();
}

void DamsaEventAction::EndOfEventAction(const G4Event* event)
{
    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce) return;
    
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    G4int trackerID = sdManager->GetCollectionID("TrackerSD/EnergyDeposit");
    G4int calorimeterID = sdManager->GetCollectionID("CalorimeterSD/EnergyDeposit");
    
    if (trackerID < 0 || calorimeterID < 0) return;
    
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    G4int eventID = event->GetEventID();
    
    G4THitsMap<G4double>* trackerMap = (G4THitsMap<G4double>*)(hce->GetHC(trackerID));
    if (trackerMap) {
        for (auto itr = trackerMap->GetMap()->begin(); itr != trackerMap->GetMap()->end(); ++itr) {
            G4int copyNo = itr->first;
            G4double edep = *(itr->second);
            
            if (edep > 0.) {
                G4VPhysicalVolume* physVol = G4PhysicalVolumeStore::GetInstance()->GetVolume("physSiTracker", false, copyNo);
                if (physVol) {
                    G4ThreeVector pos = physVol->GetTranslation();
                    man->FillNtupleIColumn(0, eventID);
                    man->FillNtupleDColumn(1, pos.x());
                    man->FillNtupleDColumn(2, pos.y());
                    man->FillNtupleDColumn(3, pos.z());
                    man->AddNtupleRow(0);
                }
            }
        }
    }
    
    G4THitsMap<G4double>* calorimeterMap = (G4THitsMap<G4double>*)(hce->GetHC(calorimeterID));
    if (calorimeterMap) {
        for (auto itr = calorimeterMap->GetMap()->begin(); itr != calorimeterMap->GetMap()->end(); ++itr) {
            G4int copyNo = itr->first;
            G4double edep = *(itr->second);
            
            if (edep > 0.) {
                G4VPhysicalVolume* physVol = G4PhysicalVolumeStore::GetInstance()->GetVolume("physCalorimeter", false, copyNo);
                if (physVol) {
                    G4ThreeVector pos = physVol->GetTranslation();
                    man->FillNtupleIColumn(0, eventID);
                    man->FillNtupleDColumn(1, pos.x());
                    man->FillNtupleDColumn(2, pos.y());
                    man->FillNtupleDColumn(3, pos.z());
                    man->AddNtupleRow(0);
                }
            }
        }
    }
}

#endif
