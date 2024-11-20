#ifndef HEROEVENTACTION_HH
#define HEROEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

class HERORunAction;

class HEROEventAction : public G4UserEventAction
{
  public:
    HEROEventAction(HERORunAction* runAction);
    ~HEROEventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void AddEdep(G4double edep) { fEdep += edep; }

  private:
    HERORunAction* fRunAction = nullptr;
    G4double       fEdep = 0.;
};

#endif


