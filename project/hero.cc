#include <iostream>

#include "G4MTRunManager.hh"
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
//#include "G4Random.hh"

#include "HERODetectorConstruction.hh"
#include "HEROActionInitialization.hh"
#include "HEROPrimaryGenerator.hh"

int main(int argc, char** argv)
{
    for (int i=0; i<argc; i++) {
        G4cout << argv[i] << G4endl;
    }
    G4int seed = 2;
    CLHEP::HepRandom::setTheSeed(seed); G4Random::setTheSeed(seed);
    G4MTRunManager* runManager = new G4MTRunManager();
    //G4RunManager *runManager = new G4RunManager();
    //runManager->SetVerboseLevel(2);

    runManager->SetUserInitialization(new HERODetectorConstruction());
    runManager->SetUserInitialization(new QGSP_BERT_HP);
    HEROActionInitialization *actionInit = new HEROActionInitialization();
    HEROPrimaryGenerator *primeGen = new HEROPrimaryGenerator();
    primeGen->SetParticleEnergy(15.); // GeV
    actionInit->SetPrimaryGenerator(primeGen);
    runManager->SetUserInitialization(actionInit);
    runManager->Initialize();

    runManager->SetNumberOfThreads(3);
    //runManager->SetPrintProgress(1);
    runManager->BeamOn(10000);
/*
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();



    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/vis/open OGL");
    UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
    UImanager->ApplyCommand("/vis/drawVolume");
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");


    ui->SessionStart();
    UImanager->ApplyCommand("/run/BeamOn 100");
*/
    return 0;
}
