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
    void AddWEdep(G4double edep) { fWEdep += edep; }
    void AddOpticalPhoton() { fNOpticalPhotons++; }
    void AddMaxEdep(G4double edep) { fMaxEdep = edep; }
    void AddMaxEdepXYZT(G4double x, G4double y, G4double z, G4double t) {
      fMaxEdepX = x;
      fMaxEdepY = y;
      fMaxEdepZ = z;
      fMaxEdepTime = t;
    }

    G4double GetMaxEdep() { return fMaxEdep; }

  private:
    HERORunAction* fRunAction = nullptr;
    G4double       fEdep = 0.;
    G4double       fWEdep = 0.;
    G4double       fMaxEdep = 0.;
    G4double       fMaxEdepX = 0.;
    G4double       fMaxEdepY = 0.;
    G4double       fMaxEdepZ = 0.;
    G4double       fMaxEdepTime = 0.;
    G4int          fNOpticalPhotons = 0;
};

#endif


