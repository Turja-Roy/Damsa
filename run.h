#ifndef RUN_H
#define RUN_H

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"

class DamsaRunAction : public G4UserRunAction {
public:
    DamsaRunAction();
    virtual ~DamsaRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
};

DamsaRunAction::DamsaRunAction() {
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(0);
}
DamsaRunAction::~DamsaRunAction() {}

void DamsaRunAction::BeginOfRunAction(const G4Run* run) {
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    man->OpenFile("output" + strRunID.str() + ".root");
}

void DamsaRunAction::EndOfRunAction(const G4Run*) {
    G4AnalysisManager* man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();
}

#endif
