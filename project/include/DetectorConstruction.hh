#ifndef B5DetectorConstruction_h
#define B5DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"

#include <vector>

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;

namespace B5
{

class MagneticField;

/// Detector construction

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    void SetArmAngle(G4double val);
    G4double GetArmAngle() { return fArmAngle; }

    void ConstructMaterials();

  private:
    void DefineCommands();

    G4GenericMessenger* fMessenger;

    static G4ThreadLocal MagneticField* fMagneticField;
    static G4ThreadLocal G4FieldManager* fFieldMgr;

    G4LogicalVolume* fHodoscope1Logical = nullptr;
    G4LogicalVolume* fHodoscope2Logical = nullptr;
    G4LogicalVolume* fWirePlane1Logical = nullptr;
    G4LogicalVolume* fWirePlane2Logical = nullptr;
    G4LogicalVolume* fCellLogical = nullptr;
    G4LogicalVolume* fHadCalScintiLogical = nullptr;
    G4LogicalVolume* fMagneticLogical = nullptr;

    std::vector<G4VisAttributes*> fVisAttributes;

    G4double fArmAngle = 30.*CLHEP::deg;
    G4RotationMatrix* fArmRotation = nullptr;
    G4VPhysicalVolume* fSecondArmPhys = nullptr;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
