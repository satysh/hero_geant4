#ifndef G4OpticalPhysics_h
#define G4OpticalPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "G4OpticalParameters.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4OpticalPhysics : public G4VPhysicsConstructor
{
 public:
  G4OpticalPhysics(G4int verbose = 0, const G4String& name = "Optical");
  ~G4OpticalPhysics() override;
  void PrintStatistics() const;

  G4OpticalPhysics(const G4OpticalPhysics& right) = delete;
  G4OpticalPhysics& operator=(const G4OpticalPhysics& right) = delete;

 protected:
  // construct particle and physics
  void ConstructParticle() override;
  void ConstructProcess() override;

 private:
  void PrintWarning(G4ExceptionDescription&) const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif  // G4OpticalPhysics_h
