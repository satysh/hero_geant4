#include "HERODetectorConstruction.hh"

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "tools/colors"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "HERODetector.hh"

HERODetectorConstruction::HERODetectorConstruction()
{}

HERODetectorConstruction::~HERODetectorConstruction()
{}

G4VPhysicalVolume *HERODetectorConstruction::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    //  МАТЕРИАЛЫ        

    G4Material *Polyvinyltoluene = new G4Material("Polyvinyltoluene", 1.023*g/cm3, 2); // ПЛАСТИКОВЫЙ СЦИНТИЛЛЯТОР
    Polyvinyltoluene->AddElement(nist->FindOrBuildElement("C"), 9);
    Polyvinyltoluene->AddElement(nist->FindOrBuildElement("H"), 10);

    G4Material *Wmat = new G4Material("Wmat", 19.3*g/cm3, 3);
    Wmat->AddElement(nist->FindOrBuildElement("W"), 92.8 * perCent);
    Wmat->AddElement(nist->FindOrBuildElement("Ni"), 4.5 * perCent);
    Wmat->AddElement(nist->FindOrBuildElement("Cu"), 2.7 * perCent);

    G4Material *BorScinmat = new G4Material("BorScinmat", 1.032 * g / cm3, 3);
    BorScinmat->AddElement(nist->FindOrBuildElement("C"), 46 * perCent);
    BorScinmat->AddElement(nist->FindOrBuildElement("H"), 51 * perCent);
    BorScinmat->AddElement(nist->FindOrBuildElement("B"), 3 * perCent);

    //G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *worldMat = new G4Material("worldMat", 1.205e-14 * g / cm3, 3);
    worldMat->AddElement(nist->FindOrBuildElement("N"), 75.5 * perCent);
    worldMat->AddElement(nist->FindOrBuildElement("O"), 23.1 * perCent);
    worldMat->AddElement(nist->FindOrBuildElement("Ar"), 1.4 * perCent);
   


    // МИР

    G4Box *solidWorld = new G4Box("solidWorld", 180*cm, 180*cm, 300*cm);

    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    // ПЛАСТИКОВЫЙ СЦИНТИЛЛЯТОР

    G4Box *solidPlasticScin = new G4Box("solidPlasticScin", 60*mm, 60*mm, 0.75*mm);
    G4LogicalVolume *logicPlasticScin = new G4LogicalVolume(solidPlasticScin, Polyvinyltoluene, "logicPlasticScin");

    G4VisAttributes * calTubeVisAtt = new G4VisAttributes(G4Colour(0.,1.,1.)); // Instantiation of a set of visualization attributes with cyan colour
    //calTubeVisAtt->SetForceWireframe(true); // Set the forced wireframe style
    logicPlasticScin->SetVisAttributes(calTubeVisAtt);

    G4VPhysicalVolume *physPlasticScin[109];

    // ВОЛЬФРАМОВЫЙ ПРЕОБРАЗОВАТЕЛЬ
    
    G4Box *solidWolfram = new G4Box("solidWolfram", 60*mm, 60*mm, 0.4*mm);
    G4LogicalVolume *logicWolfram = new G4LogicalVolume(solidWolfram, Wmat, "logicWolfram");
    G4VisAttributes * calTubeVisAtt2 = new G4VisAttributes(G4Colour(0.5,0.5,0.5)); // Instantiation of a set of visualization attributes with cyan colour
    //calTubeVisAtt2->SetForceWireframe(true); // Set the forced wireframe style
    logicWolfram->SetVisAttributes(calTubeVisAtt2);

    G4VPhysicalVolume *physWolfram[109];


    for (int i = 0; i < 109; i++) {
        physPlasticScin[i] = new G4PVPlacement(0, G4ThreeVector(0., 0., (-125.+0.75+2.3*i)*mm), logicPlasticScin, "physPlasticScin" + std::to_string(i), logicWorld, false, i, true);
        physWolfram[i] = new G4PVPlacement(0, G4ThreeVector(0., 0., (-123.5+0.4+2.3*i)*mm), logicWolfram, "physWolfram" + std::to_string(i), logicWorld, false, i, true);
    }

// 10 ММ ПЛАСТИК
    G4Box *solid10Plastic = new G4Box("solid10Plastic", 60*mm, 5*mm, 50*mm);
    G4LogicalVolume *logic10Plastic = new G4LogicalVolume(solid10Plastic, Polyvinyltoluene, "logic10Plastic");

    G4VisAttributes * calTubeVisAtt3 = new G4VisAttributes(G4Colour(1.,1.,0.)); // Instantiation of a set of visualization attributes with cyan colour
    //calTubeVisAtt3->SetForceWireframe(true); // Set the forced wireframe style
    logic10Plastic->SetVisAttributes(calTubeVisAtt3);

    G4Box *solidBorScin = new G4Box("solidBorScin", 60.*mm, 2.5*mm, 50.*mm);
    fLogicalBorScin = new G4LogicalVolume(solidBorScin, BorScinmat, "fLogicalBorScin");

    G4VisAttributes * calTubeVisAtt4 = new G4VisAttributes(G4Colour(0.,1.,0.)); // Instantiation of a set of visualization attributes with cyan colour
    //calTubeVisAtt3->SetForceWireframe(true); // Set the forced wireframe style
    fLogicalBorScin->SetVisAttributes(calTubeVisAtt4);
    


    G4VPhysicalVolume *phys10Plastic[9];
    G4VPhysicalVolume *physBorScin[9];

    phys10Plastic[1] = new G4PVPlacement(0, G4ThreeVector(0., 65.*mm, -75.*mm), logic10Plastic, "phys10Plastic0", logicWorld, false, 1, true);
    physBorScin[1] = new G4PVPlacement(0, G4ThreeVector(0., 72.5 * mm, -75. * mm), fLogicalBorScin, "physBorScin0", logicWorld, false, 1, true);
    
    phys10Plastic[5] = new G4PVPlacement(0, G4ThreeVector(0., 65.*mm, 75.7*mm), logic10Plastic, "phys10Plastic1", logicWorld, false, 5, true);
    physBorScin[5] = new G4PVPlacement(0, G4ThreeVector(0., 72.5 *mm, 75.7*mm), fLogicalBorScin, "physBorScin1", logicWorld, false, 5, true);


    phys10Plastic[3] = new G4PVPlacement(0, G4ThreeVector(0., -65.*mm, -75.*mm), logic10Plastic, "phys10Plastic6", logicWorld, false, 3, true);
    physBorScin[3] = new G4PVPlacement(0, G4ThreeVector(0., -72.5 * mm, -75. * mm), fLogicalBorScin, "physBorScin6", logicWorld, false, 3, true);
    
    phys10Plastic[7] = new G4PVPlacement(0, G4ThreeVector(0., -65.*mm, 75.7*mm), logic10Plastic, "phys10Plastic7", logicWorld, false, 7, true);
    physBorScin[7] = new G4PVPlacement(0, G4ThreeVector(0., -72.5 *mm, 75.7*mm), fLogicalBorScin, "physBorScin7", logicWorld, false, 7, true);


    G4RotationMatrix * RotMat = new G4RotationMatrix();
    RotMat -> rotate(90.0 *deg, G4ThreeVector(0., 0., 1.));
    RotMat -> invert();

    phys10Plastic[2] = new G4PVPlacement(RotMat, G4ThreeVector(65. *mm, 0., -75*mm), logic10Plastic, "phys10Plastic2", logicWorld, false, 2, true);
    physBorScin[2] = new G4PVPlacement(RotMat, G4ThreeVector(72.5 * mm, 0., -75 * mm), fLogicalBorScin, "physBorScin2", logicWorld, false, 2, true);
    
    phys10Plastic[6] = new G4PVPlacement(RotMat, G4ThreeVector(65. *mm, 0., 75.7*mm), logic10Plastic, "phys10Plastic3", logicWorld, false, 6, true);
    physBorScin[6] = new G4PVPlacement(RotMat, G4ThreeVector(72.5 *mm, 0., 75.7*mm), fLogicalBorScin, "physBorScin3", logicWorld, false, 6, true);

    phys10Plastic[4] = new G4PVPlacement(RotMat, G4ThreeVector(-65. *mm, 0., -75*mm), logic10Plastic, "phys10Plastic4", logicWorld, false, 4, true);
    physBorScin[4] = new G4PVPlacement(RotMat, G4ThreeVector(-72.5 * mm, 0., -75 * mm), fLogicalBorScin, "physBorScin4", logicWorld, false, 4, true);
    
    phys10Plastic[8] = new G4PVPlacement(RotMat, G4ThreeVector(-65. *mm, 0., 75.7*mm), logic10Plastic, "phys10Plastic5", logicWorld, false, 8, true);
    physBorScin[8] = new G4PVPlacement(RotMat, G4ThreeVector(-72.5 *mm, 0., 75.7*mm), fLogicalBorScin, "physBorScin5", logicWorld, false, 8, true);

    G4RotationMatrix* RotMatface = new G4RotationMatrix();
    RotMatface->rotate(90.0 * deg, G4ThreeVector(1., 0., 0.));
    RotMatface->invert();
    phys10Plastic[0] = new G4PVPlacement(RotMatface, G4ThreeVector(0., 0., -130* mm), logic10Plastic, "phys10Plastic9", logicWorld, false, 0, true);
    physBorScin[0] = new G4PVPlacement(RotMatface, G4ThreeVector(0., 0., -137.5 * mm), fLogicalBorScin, "physBorScin9", logicWorld, false, 0, true);



    return physWorld;
}

void HERODetectorConstruction::ConstructSDandField()
{
    HEROSensitiveDetector* sensDet = new HEROSensitiveDetector("SensitiveDetector");
    fLogicalBorScin->SetSensitiveDetector(sensDet);
}


