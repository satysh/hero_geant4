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

int atoi(char*);
char* int_to_char(G4int number);

int main(int argc, char** argv)
{
    G4int pdg = 2212;  // 2212 proton
    G4double primaryE0 = atoi(argv[1]); // GeV

    //G4double primaryE1 = 0.;
    G4double currFixedStartTime = 0.; // nanoseconds
    G4int nEvents = G4int(100000. / primaryE0);

    //G4double maxStartTime = 128000.; // nanoseconds

    G4RunManager *runManager = new G4RunManager();

    G4String outFileName = "proton_pE_" + G4String(argv[1]) + "_nEv_" + G4String(int_to_char(nEvents)) + ".root";
    HERODetectorConstruction *detectorConstruction = new HERODetectorConstruction();
    HEROSensitiveDetector *sensDetector = new HEROSensitiveDetector("SensitiveDetector");
    detectorConstruction->SetSensDetector(sensDetector);
    runManager->SetUserInitialization(detectorConstruction);
    runManager->SetUserInitialization(new QGSP_BERT_HP);
    HEROActionInitialization *actionInit = new HEROActionInitialization();
    HEROPrimaryGenerator *primeGen = new HEROPrimaryGenerator();
    primeGen->SetPrimaryParticle(pdg);
    primeGen->SetParticleEnergy(primaryE0);
    primeGen->SetParticleFixedStartTime(currFixedStartTime); // nanosec
    //primeGen->SetParticleEnergy(primaryE0, primaryE1); // GeV
    //primeGen->SetParticleMaxStartTime(maxStartTime); // nanosec
    actionInit->SetPrimaryGenerator(primeGen);
    actionInit->SetOutFileName(outFileName);
    runManager->SetUserInitialization(actionInit);
    runManager->Initialize();

    runManager->SetPrintProgress(1);
    nEvents=10; // DEBUG
    runManager->BeamOn(nEvents);

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

char* int_to_char(G4int number) {
    std::stringstream strs;
    strs << number;
    std::string temp_str = strs.str();
    char* char_type = (char*) temp_str.c_str();

    return char_type;
}
