#include "HEROActionInitialization.hh"
#include "HERORunAction.hh"

HEROActionInitialization::HEROActionInitialization()
{}

HEROActionInitialization::~HEROActionInitialization()
{}

void HEROActionInitialization::BuildForMaster() const
{
    SetUserAction(new HERORunAction());
}

void HEROActionInitialization::Build() const
{
    SetUserAction(fPrimaryGenerator);
    SetUserAction(new HERORunAction());
}
