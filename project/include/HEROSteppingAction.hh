#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class HEROEventAction;

class HEROSteppingAction : public G4UserSteppingAction
{
  public:
    HEROSteppingAction(HEROEventAction* eventAction);
    ~HEROSteppingAction() override = default;

    // method from the base class
    void UserSteppingAction(const G4Step*) override;

  private:
    HEROEventAction* fEventAction = nullptr;
};

#endif
