#ifndef HERODETECTORCONSTRUCTION_HH
#define HERODETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "HERODetector.hh"

class HERODetectorConstruction : public G4VUserDetectorConstruction
{
public:
    HERODetectorConstruction();
    ~HERODetectorConstruction();

    void SetSensDetector(HEROSensitiveDetector *det) { fSensDetector=det; }
    void SetR(G4int r) { fR=r; }

    virtual G4VPhysicalVolume *Construct();
private:
    G4LogicalVolume* fLogicalBorScin[250 + 1];
    virtual void ConstructSDandField();

private:
    HEROSensitiveDetector *fSensDetector=nullptr;

private:
    G4int fR=125; // detector radius in cm
};

#endif
