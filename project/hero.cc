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

    G4double primaryE0 = 100.; // GeV
    //G4double primaryE1 = 0.;

    //G4double maxStartTime = 62500000.; // nanoseconds
    G4double currFixedStartTime = 40000000.; // nanoseconds
    G4int nEvents = 1;
/*
    G4int seed;
    if (argc == 1)      seed = 1; // default
    else if (argc == 2) seed = atoi(argv[1]);
    else { // Read IntPam2009.txt and set E and nEvents
        seed = atoi(argv[1]);
        std::fstream fin("../project/input/IntPam2009.txt");
        if (!fin.is_open()) {
            G4cerr << "Can't open IntPam2009.txt!" << G4endl;
            return -1;
        }
        G4int id = atoi(argv[2]);
        G4String info="";
        for (G4int i=0; i<7; i++) { fin >> info; }
        G4double pi = 3.141592653;
        G4double detectorR = 125.*0.01; // m
        G4double time = maxStartTime*1e-9; // to sec
        G4double detectorS = 4.*pi*detectorR*detectorR; // m2
        G4int it=1;
        while (!fin.eof()) {
            G4double curE; // GeV
            G4double curFlux; // particles/m^2 sr s
            fin >> curE >> curFlux;
            G4double curNevents = curFlux*detectorS*4.*pi*time;
            if (it == id) {
                primaryE0=curE; nEvents=(G4int)curNevents;
                fin >> primaryE1;
                break;
            }
            it++;
        }
        fin.close();
        G4int NTHREADS = atoi(argv[3]);
        nEvents = 1 + (G4int)nEvents/NTHREADS;
    }

    G4Random::setTheSeed(seed);
    std::stringstream seedStraem;
    seedStraem << seed;
    G4String outFileName = "output_" + seedStraem.str() + ".root";
    G4cout << "Sim is starts: seed = " << seed << ", ";
    G4cout << "primaryE0=" << primaryE0 << " [GeV], "
           << "primaryE1=" << primaryE1 << " [GeV], "
           << "maxStartTime=" << maxStartTime << " [nanosec], "
           << "nEvents=" << nEvents << G4endl;
*/

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
    primeGen->SetPrimaryParticle(1000020040);
    primeGen->SetParticleEnergy(primaryE0);
    primeGen->SetParticleFixedStartTime(currFixedStartTime); // nanosec
    //primeGen->SetParticleEnergy(primaryE0, primaryE1); // GeV
    //primeGen->SetParticleMaxStartTime(maxStartTime); // nanosec
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

G4int getnEvents(G4double st) {
    return 0;
}
