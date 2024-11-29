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

    // world mat + rindex
    /*
    G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
    G4double rindexWorld[2] = {1.000, 1.000}; 

    G4MaterialPropertiesTable *mptworld = new G4MaterialPropertiesTable();
    mptworld->AddProperty("RINDEX", energy, rindexWorld, 2);
    worldMat->SetMaterialPropertiesTable(mptworld);
    */

    G4Material *BorScinMat;
    
    if (fBopt == "b") {
        G4cerr << "[HERO] b opt is set!" << G4endl;
        BorScinMat = new G4Material("BorScinMat", 1.032 * g / cm3, 3);
        G4double constRatio = 0.46 / 0.51; // (cPerCent / hPerCent) for 3% boron 
        G4double hPerCent = (1. - fBoronPerCent) / (1. + constRatio);
        G4double cPerCent = constRatio * hPerCent;
        BorScinMat->AddElement(nist->FindOrBuildElement("C"), cPerCent);
        BorScinMat->AddElement(nist->FindOrBuildElement("H"), hPerCent);
        BorScinMat->AddElement(nist->FindOrBuildElement("B"), fBoronPerCent);    
        G4cerr << cPerCent << ", " << hPerCent << ", " << fBoronPerCent << ", " << cPerCent + hPerCent + fBoronPerCent << G4endl;
    }
    else if (fBopt == "-b") {
        G4cerr << "[HERO] -b opt is set!" << G4endl;
        BorScinMat = new G4Material("BorScinMat", 1.032 * g / cm3, 2);
        BorScinMat->AddElement(nist->FindOrBuildElement("C"), 46 * perCent);
        BorScinMat->AddElement(nist->FindOrBuildElement("H"), 54 * perCent);
    }

    G4double energyScin[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
    G4double rindexScin[2] = {1.5, 1.5};
    G4double fraction[2] = {1.0, 1.0};

    G4MaterialPropertiesTable *mptScin = new G4MaterialPropertiesTable();
    mptScin->AddProperty("SCINTILLATIONCOMPONENT1", energyScin, fraction, 2);
    mptScin->AddProperty("RINDEX", energyScin, rindexScin, 2);
    mptScin->AddConstProperty("SCINTILLATIONYIELD", 10./keV);
    mptScin->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptScin->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 250.*ns);
    mptScin->AddConstProperty("SCINTILLATIONYIELD1", 1.);
    BorScinMat->SetMaterialPropertiesTable(mptScin);

    G4Material *WolframMat = new G4Material("WolframMat", 19.3*g/cm3, 1);
    WolframMat->AddElement(nist->FindOrBuildElement("W"), 100. * perCent);
    //WolframMat->AddElement(nist->FindOrBuildElement("Ni"), 4.5 * perCent);
    //WolframMat->AddElement(nist->FindOrBuildElement("Cu"), 2.7 * perCent);

    /*
    G4double energyW[] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
    G4double rindexW[] = {2.25, 2.25};

    G4MaterialPropertiesTable *mptW = new G4MaterialPropertiesTable();
    mptW->AddProperty("RINDEX", energyW, rindexW, 2);
    WolframMat->SetMaterialPropertiesTable(mptW);
    */

    // World
    G4int R = fR;
    G4Sphere *solidWorld = new G4Sphere("solidWorld", 0.*cm, G4double(R + 1)*cm, 0.*deg, 360.*deg, 0.*deg, 180.*deg);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    fLogicalBorScin[0] = logicWorld;
    G4VisAttributes * calTubeVisAtt = new G4VisAttributes(G4Colour(1.,1.,0.));
    fLogicalBorScin[0]->SetVisAttributes(calTubeVisAtt);

    // R layers
    // Scint
    G4int sensBorScinId = 1;
    for (G4int i=0; i<R; i++) {
        G4Sphere *solidBorScin = new G4Sphere("solidBorScin_"+std::to_string(i), G4double(i)*cm, (G4double(i)+0.9)*cm,
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
    for (G4int i=0; i<R; i++) {
        G4Sphere *solidWolfram = new G4Sphere("solidWolfram_"+std::to_string(i), (G4double(i)+0.9)*cm, (G4double(i)+1.)*cm,
                                              0.*deg, 360.*deg, 0.*deg, 180.*deg
                                             );
        G4LogicalVolume *logicWolfram = new G4LogicalVolume(solidWolfram, WolframMat, "logicWolfram_"+std::to_string(i));
        G4VisAttributes * calTubeVisAtt = new G4VisAttributes(G4Colour(0.,1.,0.));
        logicWolfram->SetVisAttributes(calTubeVisAtt);
        fLogicalBorScin[sensBorScinId] = logicWolfram;
        new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWolfram,
                          "physWolfram_"+std::to_string(i), logicWorld, false, 0, true
                         );
        sensBorScinId++;
    }

    return physWorld;
}

void HERODetectorConstruction::ConstructSDandField()
{
}


