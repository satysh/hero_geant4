#include "HERODetectorConstruction.hh"
#include "HERODetector.hh"

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

    G4Material *WolframMat = new G4Material("WolframMat", 19.3*g/cm3, 3);
    WolframMat->AddElement(nist->FindOrBuildElement("W"), 92.8 * perCent);
    WolframMat->AddElement(nist->FindOrBuildElement("Ni"), 4.5 * perCent);
    WolframMat->AddElement(nist->FindOrBuildElement("Cu"), 2.7 * perCent);


    // World

    G4Sphere *solidWorld = new G4Sphere("solidWorld", 0.*cm, 125.*cm, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    // 125 layers
    // Scint
    G4int sensBorScinId = 0;
    for (G4int i=0; i<125; i+=2) {
        G4Sphere *solidBorScin = new G4Sphere("solidBorScin_"+std::to_string(i), G4double(i)*cm, (G4double(i)+1.)*cm,
                                              0.*deg, 360.*deg, 0.*deg, 180.*deg
                                             );
        fLogicalBorScin[sensBorScinId] = new G4LogicalVolume(solidBorScin, BorScinMat, "logicBorScint_"+std::to_string(i));
        G4VisAttributes * calTubeVisAtt = new G4VisAttributes(G4Colour(1.,1.,0.));
        fLogicalBorScin[sensBorScinId]->SetVisAttributes(calTubeVisAtt);
        new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), fLogicalBorScin[sensBorScinId],
                          "physBorScint_"+std::to_string(i), logicWorld, false, 0, true
                         );
        sensBorScinId++;
    }
    // Absorber
    for (G4int i=1; i<125; i+=2) {
        G4Sphere *solidWolfram = new G4Sphere("solidWolfram_"+std::to_string(i), G4double(i)*cm, (G4double(i)+1.)*cm,
                                              0.*deg, 360.*deg, 0.*deg, 180.*deg
                                             );
        G4LogicalVolume *logicWolfram = new G4LogicalVolume(solidWolfram, WolframMat, "logicWolfram_"+std::to_string(i));
        G4VisAttributes * calTubeVisAtt = new G4VisAttributes(G4Colour(0.,1.,0.));
        logicWolfram->SetVisAttributes(calTubeVisAtt);
        new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWolfram,
                          "physWolfram_"+std::to_string(i), logicWorld, false, 0, true
                         );
    }

    return physWorld;
}

void HERODetectorConstruction::ConstructSDandField()
{
    HEROSensitiveDetector* sensDet = new HEROSensitiveDetector("SensitiveDetector");
    for (G4int i=0; i<63; i++)
        fLogicalBorScin[i]->SetSensitiveDetector(sensDet);
}


