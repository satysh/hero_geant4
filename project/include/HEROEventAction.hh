#ifndef HEROEventAction_h
#define HEROEventAction_h 1

#include "G4UserEventAction.hh"

namespace HERO
{

/// Event action

class HEROEventAction : public G4UserEventAction
{
public:
    HEROEventAction();
    ~HEROEventAction() override;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
