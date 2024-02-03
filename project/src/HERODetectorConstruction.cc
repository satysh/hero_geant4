#include "HERODetectorConstruction.hh"

#include "G4Sphere.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "tools/colors"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

HERODetectorConstruction::HERODetectorConstruction()
{}

HERODetectorConstruction::~HERODetectorConstruction()
{}

G4VPhysicalVolume *HERODetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    /*  Materials */
    // ----------------------------------------------------------------------------------------------------------------
    //G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *worldMat = new G4Material("worldMat", 1.205e-14 * g / cm3, 3);
    worldMat->AddElement(nist->FindOrBuildElement("N"), 75.5 * perCent);
    worldMat->AddElement(nist->FindOrBuildElement("O"), 23.1 * perCent);
    worldMat->AddElement(nist->FindOrBuildElement("Ar"), 1.4 * perCent);

    G4Material *BorScinMat = new G4Material("BorScinMat", 1.032 * g / cm3, 3);
    BorScinMat->AddElement(nist->FindOrBuildElement("C"), 46 * perCent);
    BorScinMat->AddElement(nist->FindOrBuildElement("H"), 51 * perCent);
    BorScinMat->AddElement(nist->FindOrBuildElement("B"), 3 * perCent);

    G4int R = 125; // DetSize
    // World
    G4Sphere *solidWorld = new G4Sphere("solidWorld", 0.*cm, G4double(R)*cm, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    // Scint
    G4Sphere *solidBorScin = new G4Sphere("solidBorScin", 0., G4double(R)*cm, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    fLogicalBorScin = new G4LogicalVolume(solidBorScin, BorScinMat, "logicBorScint");
    G4VisAttributes * calTubeVisAtt = new G4VisAttributes(G4Colour(1.,1.,0.));
    fLogicalBorScin->SetVisAttributes(calTubeVisAtt);
    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), fLogicalBorScin, "physBorScint", logicWorld, false, 0, true);
    
    return physWorld;
}

void HERODetectorConstruction::ConstructSDandField()
{
    if (!fSensDetector) {
        HEROSensitiveDetector* sensDet = new HEROSensitiveDetector("SensitiveDetector");
        fSensDetector=sensDet;
    }
    fLogicalBorScin->SetSensitiveDetector(fSensDetector);
}


