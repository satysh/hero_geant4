#ifndef HERODETECTORCONSTRUCTION_HH
#define HERODETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "TString.h"
#include "HERODetector.hh"

class HERODetectorConstruction : public G4VUserDetectorConstruction
{
public:
    HERODetectorConstruction();
    ~HERODetectorConstruction();

    void SetSensDetector(HEROSensitiveDetector *det) { fSensDetector=det; }
    void SetR(G4int r) { fR=r; }
    void SetBopt(TString bopt) { fBopt=bopt; }


    virtual G4VPhysicalVolume *Construct();
private:
    G4LogicalVolume* fLogicalBorScin[2000 + 1];
    virtual void ConstructSDandField();

private:
    HEROSensitiveDetector *fSensDetector=nullptr;

private:
    G4int fR=125; // detector radius in cm
    TString fBopt="b";
};

#endif
