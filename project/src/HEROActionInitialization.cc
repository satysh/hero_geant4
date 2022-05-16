#include "HEROActionInitialization.hh"

HEROActionInitialization::HEROActionInitialization()
{}

HEROActionInitialization::~HEROActionInitialization()
{}

// TODO why do we need to build runAction here?
void HEROActionInitialization::BuildForMaster() const
{
    SetUserAction(fRunAction);
}

void HEROActionInitialization::Build() const
{
    SetUserAction(fPrimaryGenerator);
    SetUserAction(fRunAction);
}
