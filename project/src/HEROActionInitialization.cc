#include "HEROActionInitialization.hh"
#include "HERORunAction.hh"
#include "HEROEventAction.hh"
#include "HEROSteppingAction.hh"

HEROActionInitialization::HEROActionInitialization()
{}

HEROActionInitialization::~HEROActionInitialization()
{}

void HEROActionInitialization::BuildForMaster() const
{
    HERORunAction *runAction = new HERORunAction();
    runAction->SetOutFileName(fOutFileName);
    SetUserAction(runAction);
}

void HEROActionInitialization::Build() const
{
    SetUserAction(fPrimaryGenerator);
    
    HERORunAction *runAction = new HERORunAction();
    runAction->SetOutFileName(fOutFileName);
    SetUserAction(runAction);

    HEROEventAction *eventAction = new HEROEventAction(runAction);
    SetUserAction(eventAction);

    HEROSteppingAction *steppingAction = new HEROSteppingAction(eventAction);
    SetUserAction(steppingAction);
}
