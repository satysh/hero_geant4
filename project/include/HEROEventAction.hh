#ifndef HEROEVENTACTION_HH
#define HEROEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <unordered_map>

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
    void SetIsPrimaryFirstStep(G4bool flag) { fIsPrimaryFirstStep = flag; }
    void AddMaxEdepXYZT(G4double x, G4double y, G4double z, G4double t) {
      fMaxEdepX = x;
      fMaxEdepY = y;
      fMaxEdepZ = z;
      fMaxEdepTime = t;
    }
    void AddParticleEdep(G4int pdg, G4double edep);

    G4double IsPrimaryFirstStep() { return fIsPrimaryFirstStep; }
    G4int GetEventID() { return fEventID; }

  private:
    HERORunAction* fRunAction = nullptr;
    G4double       fEdep = 0.;
    G4double       fWEdep = 0.;
    G4double       fMaxEdepX = 0.;
    G4double       fMaxEdepY = 0.;
    G4double       fMaxEdepZ = 0.;
    G4double       fMaxEdepTime = 0.;
    G4int          fNOpticalPhotons = 0;
    G4int          fEventID=0;
    G4bool         fIsPrimaryFirstStep=true;

    std::unordered_map<G4int, G4double> fPrticleEdepMap; // <pdg, edep>
};

#endif


