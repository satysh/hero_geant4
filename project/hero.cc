#include <iostream>
#include <fstream>

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

#include "TString.h"

#include "HERODetectorConstruction.hh"
#include "HERODetector.hh"
#include "HEROActionInitialization.hh"
#include "HEROPrimaryGenerator.hh"

int atoi(char *str);
int *getParams(int intervalId); // return array [primaryE0, primaryE1, nEvents]

int main(int argc, char** argv)
{   

    G4int intervalId = 0;
    if (argc > 1) {
        intervalId = atoi(argv[1]);
    }
    else {
        G4cerr << "######################## ";
        G4cerr << "Interval Id is not set!";
        G4cerr << " ########################" << G4endl;
        return -1;
    }

    int *params = getParams(intervalId);

    G4double primaryE0 = G4double(params[0]); // GeV
    G4double primaryE1 = G4double(params[1]); // GeV
    G4int nEvents = params[2];

    G4RunManager *runManager = new G4RunManager();
    //runManager->SetVerboseLevel(3);

    TString outFileName;
    outFileName.Form("hero_E0_%d_E1_%d_nevents_%d.root", params[0], params[1], params[2]);
    delete params;

    HERODetectorConstruction *detectorConstruction = new HERODetectorConstruction();
    HEROSensitiveDetector *sensDetector = new HEROSensitiveDetector("SensitiveDetector");
    detectorConstruction->SetSensDetector(sensDetector);
    runManager->SetUserInitialization(detectorConstruction);
    runManager->SetUserInitialization(new QGSP_BERT_HP);
    HEROActionInitialization *actionInit = new HEROActionInitialization();
    HEROPrimaryGenerator *primeGen = new HEROPrimaryGenerator();
    primeGen->SetPrimaryParticle(2212); // 2212 proton
    //primeGen->SetParticleEnergy(primaryE0);
    primeGen->SetParticleFixedStartTime(0.); // nanosec
    primeGen->SetParticleEnergy(primaryE0, primaryE1); // GeV
    actionInit->SetPrimaryGenerator(primeGen);
    actionInit->SetOutFileName(G4String(outFileName.Data()));
    runManager->SetUserInitialization(actionInit);
    runManager->Initialize();

    runManager->SetPrintProgress(1);
    runManager->BeamOn(nEvents);

    return 0;
}

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

int *getParams(int intervalId) {
    int *result = new int[3];
    result[0] = 0;
    result[1] = 0;
    result[2] = 0;

    std::fstream fin("../project/input/one_year_protons_number_7e-8%.txt");
    if (!fin.is_open()) {
        G4cerr << "Can't read input/one_year_protons_number_7e-8%.txt" << G4endl;
        return result;
    }

    
    double E0, E1, ni, Ni;
    for (int i=0; i<=intervalId; i++) {
        fin >> E0 >> E1 >> ni >> Ni;
        //G4cerr << E0 << ", " << E1 << ", " << ni << ", " << Ni << G4endl; // DEBUG
    }
    
    result[0] = E0;
    result[1] = E1;
    result[2] = Ni;

    fin.close();
    return result;
}   