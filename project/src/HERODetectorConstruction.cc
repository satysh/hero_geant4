#include "HERODetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

namespace HERO
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HERODetectorConstruction::HERODetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HERODetectorConstruction::~HERODetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* HERODetectorConstruction::Construct()
{
  // Construct materials
  auto nistManager = G4NistManager::Instance();

  // Air
  nistManager->FindOrBuildMaterial("G4_AIR");

  // Lead
  nistManager->FindOrBuildMaterial("G4_Pb");

  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  auto air = G4Material::GetMaterial("G4_AIR");
  auto scintillator = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  auto lead = G4Material::GetMaterial("G4_Pb");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  // geometries --------------------------------------------------------------
  // experimental hall (world volume)
  auto worldSolid
    = new G4Box("worldBox",300.*cm,300.*cm,300.*cm); // name,x,y,z
  auto worldLogical
    = new G4LogicalVolume(worldSolid,air,"worldLogical"); // VSolid, material, name
  auto worldPhysical
    = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,
                        false,0,checkOverlaps); // rotMatrix, Pos3Vector, vLogic, name, vMother,

  // lead sphere
  auto sphereSolid
    = new G4Sphere("Sphere",150.*cm,150.*cm,150.*cm);
  auto sphereLogical
    = new G4LogicalVolume(sphereSolid,lead,"sphereLogical");
  new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),sphereLogical,
                    "spherePhysical",worldLogical,
                    false,0,checkOverlaps);

  // return the world physical volume ----------------------------------------

  return worldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
