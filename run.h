#ifndef RUN_H
#define RUN_H

#include "G4UserRunAction.hh"
#include "G4Run.hh"

#include "analysis.h"

class DamsaRunAction : public G4UserRunAction {
public:
    DamsaRunAction() {}
    virtual ~DamsaRunAction() {}

    virtual void BeginOfRunAction(const G4Run*) {}
    virtual void EndOfRunAction(const G4Run*);
};

void DamsaRunAction::EndOfRunAction(const G4Run*) {
    // Write analysis histograms (particle spectra at scoring planes)
    DamsaAnalysis::Instance()->WriteROOTHistograms("particle_spectra.root");
}

#endif
