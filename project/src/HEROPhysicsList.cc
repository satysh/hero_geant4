#include "HEROPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"

HEROPhysicsList::HEROPhysicsList()
{
    RegisterPhysics (new G4EmStandardPhysics());
    RegisterPhysics(new G4OpticalPhysics());
}

HEROPhysicsList::~HEROPhysicsList()
{}
