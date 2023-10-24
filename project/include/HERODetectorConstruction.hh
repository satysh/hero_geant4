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

    virtual G4VPhysicalVolume *Construct();
private:
    G4LogicalVolume* fLogicalBorScin[125];
    virtual void ConstructSDandField();

private:
    HEROSensitiveDetector *fSensDetector=nullptr;
};

#endif
