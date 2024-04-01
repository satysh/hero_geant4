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

#include "TString.h"

int main(int argc, char** argv)
{   
    G4Random::setTheSeed(1);
    G4int nowR = 125; // cm
    //G4double primaryE0 = 10.; // GeV
    //G4double primaryE1 = 0.;

    if (argc > 1) {
        TString strR(argv[1]);
        nowR = strR.Atoi();
    }

    G4double maxStartTime = 100000.; // nanoseconds
    //G4double currFixedStartTime = 0.; // nanoseconds
    G4int nEvents = 10000;
    G4RunManager *runManager = new G4RunManager();
    //runManager->SetVerboseLevel(3);

    TString outFileNameTmp;
    outFileNameTmp.Form("2010_C_48_H_52_background_%d_usec_nevents_%d_r_%d.root", G4int(maxStartTime / 1000), nEvents, nowR);
    G4String outFileName(outFileNameTmp);
    HERODetectorConstruction *detectorConstruction = new HERODetectorConstruction();
    HEROSensitiveDetector *sensDetector = new HEROSensitiveDetector("SensitiveDetector");
    //sensDetector->SetStartEventId((seed-1)*nEvents); // because of multi-thread
    detectorConstruction->SetSensDetector(sensDetector);
    detectorConstruction->SetR(nowR);
    runManager->SetUserInitialization(detectorConstruction);
    runManager->SetUserInitialization(new QGSP_BERT_HP);
    HEROActionInitialization *actionInit = new HEROActionInitialization();
    HEROPrimaryGenerator *primeGen = new HEROPrimaryGenerator();
    primeGen->SetPrimaryParticle(2212); // 2212 proton
    //primeGen->SetParticleEnergy(primaryE0);
    //primeGen->SetParticleFixedStartTime(currFixedStartTime); // nanosec
    primeGen->SetR(nowR);
    primeGen->SetInputFluxFileName("cumulative_func_2010.txt");
    primeGen->ReadFluxTXT();
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