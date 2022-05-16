#ifndef HERODETECTORCONSTRUCTION_HH
#define HERODETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

class HERODetectorConstruction : public G4VUserDetectorConstruction
{
public:
    HERODetectorConstruction();
    ~HERODetectorConstruction();

    virtual G4VPhysicalVolume *Construct();
private:
    G4LogicalVolume* fLogicalBorScin;
    virtual void ConstructSDandField();
};

#endif
