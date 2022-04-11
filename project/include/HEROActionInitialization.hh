#ifndef HEROActionInitialization_h
#define HEROActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

namespace HERO
{

/// Action initialization class.

class HEROActionInitialization : public G4VUserActionInitialization
{
  public:
    HEROActionInitialization();
    ~HEROActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
