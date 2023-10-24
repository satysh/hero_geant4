#include <iostream>

#include "G4RunManager.hh"
#include <QGSP_BERT_HP.hh>

#include "G4String.hh"

#include "HERODetectorConstruction.hh"
#include "HERODetector.hh"
#include "HEROActionInitialization.hh"
#include "HEROPrimaryGenerator.hh"

G4int GetPdgCode(G4int Z, G4int A) {
    if (Z == 1 && A == 1) return 2212;
    return 1000000000 + Z*10000 + A*10;
}

G4int GetPdgCode(G4String name="") {
    if (name == "0") // electron
        return 11;
    else if (name == "1") // gamma
        return 22;
    else if (name == "2") // proton
        return GetPdgCode(1, 1);
    else if (name == "3") // He
        return GetPdgCode(2, 4);
    else if (name == "4") // Fe
        return GetPdgCode(26, 56);

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

int main(int argc, char** argv)
{
/*
    // Checking
    for (G4int i=0; i < 5; i++) {
        char n_char[1 + sizeof(char)];
        std::sprintf(n_char, "%d", i);
        G4cerr << GetPdgCode(n_char) << ", " << atoi("12345") << G4endl;
    }
*/

    G4String particle = argv[1];
    G4cerr << particle << G4endl;
    G4String energy = argv[2];
    G4cerr << energy << G4endl;
    G4String nevents = argv[3];
    G4cerr << nevents << G4endl;
    G4String outFileName = argv[4];
    G4cerr << outFileName << G4endl;

    G4double primaryE0 = (G4double)atoi(energy); // GeV

    G4int nEvents = atoi(nevents);

    G4cerr << "-------------------------------" << G4endl;
    G4cerr << GetPdgCode(particle) << ", " << primaryE0 << ", " << nEvents << G4endl;

    G4RunManager *runManager = new G4RunManager();

    HERODetectorConstruction *detectorConstruction = new HERODetectorConstruction();
    HEROSensitiveDetector *sensDetector = new HEROSensitiveDetector("SensitiveDetector");
    detectorConstruction->SetSensDetector(sensDetector);
    runManager->SetUserInitialization(detectorConstruction);
    runManager->SetUserInitialization(new QGSP_BERT_HP);
    HEROActionInitialization *actionInit = new HEROActionInitialization();
    HEROPrimaryGenerator *primeGen = new HEROPrimaryGenerator();
    primeGen->SetPrimaryParticle(GetPdgCode(particle)); // 2212 proton
    primeGen->SetParticleEnergy(primaryE0);
    primeGen->SetParticleFixedStartTime(0); // nanosec
    actionInit->SetPrimaryGenerator(primeGen);
    actionInit->SetOutFileName(outFileName);
    runManager->SetUserInitialization(actionInit);
    runManager->Initialize();

    runManager->SetPrintProgress(1);
    runManager->BeamOn(nEvents);

    return 0;
}