//#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4SystemOfUnits.hh"
#include <QGSP_BERT_HP.hh>
//#include <QGSP_INCLXX_HP>

#include "HERODetectorConstruction.hh"
#include "HEROActionInitialization.hh"
#include "HEROPrimaryGenerator.hh"

#include "OpticalPhysicsWithoutCherenkov.hh"

#include "TString.h" 

#include <ctime>

int main(int argc, char** argv)
{   
    clock_t start_time = clock();
    G4int nEvents = 1000;
    G4int pdg = 2212; // 2212 proton, 11 electron
    G4int detectorR = 125; 

    G4double primaryE = 1. * GeV;

    TString bopt = "b"; // or -b
    G4double boronPerCent = 5. * perCent;

    G4int nthr = 1;
    G4int random_state=654321;
    if (argc > 1) {
        TString energy(argv[1]), nthreds(argv[2]);
        primaryE = energy.Atof() * GeV;
        nthr = nthreds.Atoi();
    }
    else if (argc > 3) {
        TString seed(argv[3]);
        random_state = seed.Atoi();
    }

    G4Random::setTheSeed(random_state);

    TString outFileName;
    outFileName.Form("hero_nevents_%d_pdg_%d_R_%d_E_%d_bron_%s_percent_%d.root", nEvents, pdg, detectorR, G4int(primaryE), bopt.Data(),
                     G4int(boronPerCent / perCent));

    G4MTRunManager* runManager = new G4MTRunManager();     // Multithreaded mode
    runManager->SetNumberOfThreads(nthr);
    G4cout << G4endl << G4endl << "NUMBER THREADS: " << runManager->GetNumberOfThreads() << G4endl<< G4endl;

    //G4RunManager *runManager = new G4RunManager();
    G4VModularPhysicsList* physics = new QGSP_BERT_HP(0);
    //physics->RegisterPhysics(new G4OpticalPhysics());
    runManager->SetUserInitialization(physics);
    
    HERODetectorConstruction *detectorConstruction = new HERODetectorConstruction();    
    detectorConstruction->SetR(detectorR);
    detectorConstruction->SetBopt(bopt.Data());
    detectorConstruction->SetBoronPerCent(boronPerCent);
    runManager->SetUserInitialization(detectorConstruction);

    HEROPrimaryGenerator *primeGen = new HEROPrimaryGenerator();
    primeGen->SetPrimaryParticle(pdg);
    primeGen->SetParticleEnergy(primaryE);
    primeGen->SetR(G4double(detectorR) * cm);
    
    HEROActionInitialization *actionInit = new HEROActionInitialization();
    actionInit->SetPrimaryGenerator(primeGen);
    actionInit->SetOutFileName(outFileName.Data());
    runManager->SetUserInitialization(actionInit);

    runManager->Initialize();
    runManager->SetPrintProgress(1);
    runManager->BeamOn(nEvents);

    clock_t end_time = clock();
    G4double execution_time = (G4double)(end_time - start_time) / CLOCKS_PER_SEC;
    G4cout << "Execution time: " << G4int(execution_time / 3600.) << " h "
                                 << G4int(execution_time / 60.) << " min "
                                 << execution_time << " seconds " << G4endl;
 
    return 0;
}
