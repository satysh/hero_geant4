#ifndef HEROHERODetectorConstruction_h
#define HEROHERODetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;

namespace HERO
{
/// Detector construction

class HERODetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    HERODetectorConstruction();
    ~HERODetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    void ConstructMaterials();
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
