#include "HEROActionInitialization.hh"
#include "HERORunAction.hh"
#include "HEROPrimaryGenerator.hh"

HEROActionInitialization::HEROActionInitialization()
{}

HEROActionInitialization::~HEROActionInitialization()
{}

// TODO why do we need to build runAction here?
void HEROActionInitialization::BuildForMaster() const
{
    HERORunAction* runAction = new HERORunAction();
    SetUserAction(runAction);
}

void HEROActionInitialization::Build() const
{
    HEROPrimaryGenerator* generator = new HEROPrimaryGenerator();
    SetUserAction(generator);

    HERORunAction* runAction = new HERORunAction();
    SetUserAction(runAction);
}
