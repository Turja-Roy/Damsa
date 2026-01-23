#ifndef ACTION_H
#define ACTION_H

#include "G4VUserActionInitialization.hh"

#include "generator.h"
#include "run.h"
#include "event.h"
#include "stepping.h"

class DamsaActionInitialization : public G4VUserActionInitialization {
public:
    DamsaActionInitialization();
    virtual ~DamsaActionInitialization();

    virtual void Build() const;
};

DamsaActionInitialization::DamsaActionInitialization () {}
DamsaActionInitialization::~DamsaActionInitialization () {}

void DamsaActionInitialization::Build () const {
    DamsaPrimaryGenerator* generator = new DamsaPrimaryGenerator();
    SetUserAction(generator);

    DamsaRunAction* runAction = new DamsaRunAction();
    SetUserAction(runAction);

    DamsaEventAction* eventAction = new DamsaEventAction();
    SetUserAction(eventAction);
    
    DamsaSteppingAction* steppingAction = new DamsaSteppingAction();
    SetUserAction(steppingAction);
}

#endif
