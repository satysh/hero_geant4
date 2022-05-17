#include "HEROActionInitialization.hh"
#include "HERORunAction.hh"

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
}
