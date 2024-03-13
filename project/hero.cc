#include <iostream>

#include "G4RunManager.hh"
//#include "HEROPhysicsList.hh"
#include <QGSP_BERT_HP.hh>
//#include <QGSP_INCLXX_HP>

#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "tools/colors"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4String.hh"

#include "HERODetectorConstruction.hh"
#include "HERODetector.hh"
#include "HEROActionInitialization.hh"
#include "HEROPrimaryGenerator.hh"

int atoi(char*);
G4int getnEvents(G4double start_time);

int main(int argc, char** argv)
{
    //G4double primaryE0 = 1.; // GeV
    //G4double primaryE1 = 0.;

    G4double maxStartTime = 1000000000.; // nanoseconds
    //G4double currFixedStartTime = 0.; // nanoseconds
    G4int nEvents = 10;
    G4RunManager *runManager = new G4RunManager();
    //runManager->SetVerboseLevel(2);

    G4String outFileName = "hero.root";
    HERODetectorConstruction *detectorConstruction = new HERODetectorConstruction();
    HEROSensitiveDetector *sensDetector = new HEROSensitiveDetector("SensitiveDetector");
    //sensDetector->SetStartEventId((seed-1)*nEvents); // because of multi-thread
    detectorConstruction->SetSensDetector(sensDetector);
    runManager->SetUserInitialization(detectorConstruction);
    runManager->SetUserInitialization(new QGSP_BERT_HP);
    HEROActionInitialization *actionInit = new HEROActionInitialization();
    HEROPrimaryGenerator *primeGen = new HEROPrimaryGenerator();
    primeGen->SetPrimaryParticle(2212); // 2212 proton
    //primeGen->SetParticleEnergy(primaryE0);
    //primeGen->SetParticleFixedStartTime(currFixedStartTime); // nanosec
    //primeGen->SetParticleEnergy(primaryE0, primaryE1); // GeV
    primeGen->SetParticleMaxStartTime(maxStartTime); // nanosec
    actionInit->SetPrimaryGenerator(primeGen);
    actionInit->SetOutFileName(outFileName);
    runManager->SetUserInitialization(actionInit);
    runManager->Initialize();

    runManager->SetPrintProgress(1);
    runManager->BeamOn(nEvents);

    return 0;
}