#include <iostream>

#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
//#include "HEROPhysicsList.hh"
#include <QGSP_BERT_HP.hh>

/*
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "tools/colors"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
*/

#include "HERODetectorConstruction.hh"
#include "HEROActionInitialization.hh"

int main(int argc, char** argv)
{
    for (int i=0; i<argc; i++) {
        G4cout << argv[i] << G4endl;
    }
    G4MTRunManager* runManager = new G4MTRunManager();
    //G4RunManager *runManager = new G4RunManager();

    runManager->SetUserInitialization(new HERODetectorConstruction());
    runManager->SetUserInitialization(new QGSP_BERT_HP);
    runManager->SetUserInitialization(new HEROActionInitialization());

    runManager->SetNumberOfThreads(2);
    runManager->Initialize();
    runManager->SetPrintProgress(1);
    runManager->BeamOn(10);

    /*G4UIExecutive* ui = new G4UIExecutive(argc, argv);

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();



    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/vis/open OGL");
    UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
    UImanager->ApplyCommand("/vis/drawVolume");
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");


    ui->SessionStart();
    //UImanager->ApplyCommand(""
    //UImanager->ApplyCommand("/run/BeamOn 100");*/

    return 0;
}
