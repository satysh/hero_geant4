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

int main(int argc, char** argv)
{
    G4int nEvents = 10;

    G4int seed;
    if (argc == 1) seed = 1; // default
    else           seed = atoi(argv[1]);

    G4Random::setTheSeed(seed);
    std::stringstream seedStraem;
    seedStraem << seed;
    G4String outFileName = "output_" + seedStraem.str() + ".root";

    G4RunManager *runManager = new G4RunManager();
    //runManager->SetVerboseLevel(2);

    HERODetectorConstruction *detectorConstruction = new HERODetectorConstruction();
    HEROSensitiveDetector *sensDetector = new HEROSensitiveDetector("SensitiveDetector");
    sensDetector->SetStartEventId((seed-1)*nEvents); // because of multi-thread
    sensDetector->SetPrimaryStartTime(0.); // [nanoseconds] because of uniform start time
    detectorConstruction->SetSensDetector(sensDetector);
    runManager->SetUserInitialization(detectorConstruction);
    runManager->SetUserInitialization(new QGSP_BERT_HP);
    HEROActionInitialization *actionInit = new HEROActionInitialization();
    HEROPrimaryGenerator *primeGen = new HEROPrimaryGenerator();
    primeGen->SetParticleEnergy(15.); // GeV
    actionInit->SetPrimaryGenerator(primeGen);
    actionInit->SetOutFileName(outFileName);
    runManager->SetUserInitialization(actionInit);
    runManager->Initialize();

    runManager->SetPrintProgress(1);
    runManager->BeamOn(nEvents);
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

// -- implementation of atoi 
int atoi(char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
        if (len > 100) return -1;
    }
    int res = 0;
    int dec = 1;
    for (int i=len-1; i >= 0; i--) {
        res += dec*(int(str[i])-48);
        dec *= 10;
    }
    return res;
}
