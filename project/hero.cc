#include <iostream>
#include <sstream>
#include <string.h>

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
    //G4int year = 2010;
    TString bopt = "";
    G4int nowR = 125; // cm
    G4double primaryE0 = -1.; // GeV

    //G4double maxStartTime = 100000.; // nanoseconds
    G4double currFixedStartTime = 0.; // nanoseconds
    G4int nEvents = -1;

    G4int seed = 1;
    G4int seed_begin = 1;
    if (argc > 5) {
        TString inputSeedBegin(argv[5]);
        seed_begin = inputSeedBegin.Atoi();
    }

    if (argc > 1) {
        TString inputSeed(argv[1]);
        seed = inputSeed.Atoi() + seed_begin;
    }
    G4Random::setTheSeed(seed);

    if (argc > 2) {
        TString inputE(argv[2]);
        primaryE0 = (G4double)inputE.Atoi(); // GeV
    }

    if (argc > 3) {
        TString inputNEvents(argv[3]);
        nEvents = inputNEvents.Atoi();
    }

    if (argc > 4) {
        bopt = argv[4];
    }


    G4RunManager *runManager = new G4RunManager();
    //runManager->SetVerboseLevel(3);
    TString outFileNameTmp;
    if (bopt == "wb")
        outFileNameTmp.Form("seed_%d_fixed_E_%d_GeV_C_46_H_54_nevents_%d_r_%d.root", seed, G4int(primaryE0), nEvents, nowR);
    else
        outFileNameTmp.Form("seed_%d_fixed_E_%d_GeV_nevents_%d_r_%d.root", seed, G4int(primaryE0), nEvents, nowR);
    G4String outFileName(outFileNameTmp);
    HERODetectorConstruction *detectorConstruction = new HERODetectorConstruction();
    HEROSensitiveDetector *sensDetector = new HEROSensitiveDetector("SensitiveDetector");
    detectorConstruction->SetSensDetector(sensDetector);
    detectorConstruction->SetR(nowR);
    detectorConstruction->SetBopt(bopt);    
    runManager->SetUserInitialization(detectorConstruction);
    runManager->SetUserInitialization(new QGSP_BERT_HP);
    HEROActionInitialization *actionInit = new HEROActionInitialization();
    HEROPrimaryGenerator *primeGen = new HEROPrimaryGenerator();
    primeGen->SetPrimaryParticle(2212); // 2212 proton
    primeGen->SetParticleEnergy(primaryE0);
    primeGen->SetParticleFixedStartTime(currFixedStartTime); // nanosec
    primeGen->SetR(nowR);
    TString cum_func_name;
    //cum_func_name.Form("cumulative_func_%d.txt", year);
    //primeGen->SetInputFluxFileName(cum_func_name.Data());
    //primeGen->ReadFluxTXT();
    //primeGen->SetParticleEnergy(primaryE0, primaryE1); // GeV
    //primeGen->SetParticleMaxStartTime(maxStartTime); // nanosec
    actionInit->SetPrimaryGenerator(primeGen);
    actionInit->SetOutFileName(outFileName);
    runManager->SetUserInitialization(actionInit);
    runManager->Initialize();

    runManager->SetPrintProgress(1);
    runManager->BeamOn(nEvents);
 
    return 0;
}
