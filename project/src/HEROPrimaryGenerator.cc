#include "HEROPrimaryGenerator.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4IonTable.hh"

#include "TGraph.h"
#include "TF1.h"
#include "TVectorD.h"

#include <fstream>

TGraph* energyInvCDFGr = NULL;

Double_t EnergyInvCDF(Double_t *x, Double_t *par) {
  return energyInvCDFGr->Eval(x[0]);
}

HEROPrimaryGenerator::HEROPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
    fParticleSource = new G4GeneralParticleSource();
}

HEROPrimaryGenerator::~HEROPrimaryGenerator()
{
    if (fParticleGun)
        delete fParticleGun;
    if (fParticleSource)
        delete fParticleSource;
}
void HEROPrimaryGenerator::SetParticleEnergy(G4double particleE0, G4double particleE1)
{
    if (particleE1 > particleE0) {
        fEnergyRangeIsSet=true;
        fParticleEnergy0=particleE0;
        fParticleEnergy1=particleE1;
    }
    else {
        fEnergyIsSet = true;
        fParticleEnergy= particleE0;
    }

}

void HEROPrimaryGenerator::SetBackgroundMCMode(G4double dtime) 
{ 
    fBackgroundDTime=dtime; 
    fBackgroundMCIsSet=true; 
    ReadFluxTXT();
}

void HEROPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4int eventId = anEvent->GetEventID();
/*
    G4cerr << "Event: " << eventId << G4endl;
*/
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4IonTable* Iontable = particleTable->GetIonTable();
    G4ParticleDefinition *particle = particleTable->FindParticle(fPrimaryParticlePDG);
    //G4IonTable* Iontable = particleTable->GetIonTable();
    //G4ParticleDefinition* particle = Iontable->GetIon(2, 4, 0); // He-4

    if (!particle) {
        G4cerr << "Can't find the particle with pdg: " << fPrimaryParticlePDG << G4endl;
        return;
    }


    fParticleGun->SetParticleDefinition(particle);

    G4ThreeVector pos(0., 0., -fR);
    fParticleGun->SetParticlePosition(pos);
    
    G4double px = 0.;
    G4double py = 0.;
    G4double pz = 1.;
    G4ThreeVector mom(px, py, pz);
    fParticleGun->SetParticleMomentumDirection(mom);

    // Energy and Time
    if (fBackgroundMCIsSet) {
        G4double startTime = (G4double)G4UniformRand() * fBackgroundDTime; // nanoseconds
        fParticleGun->SetParticleTime(startTime); // nanoseconds
        G4double particleEnergy = GetBackgroundPrimaryEnergy();
        fParticleGun->SetParticleEnergy(particleEnergy);
        // Debug
        G4cerr << "startTime=" << fParticleGun->GetParticleTime() << G4endl;
        G4cerr << "Energy=" << fParticleGun->GetParticleEnergy() << G4endl;
    }
    else if (fEnergyIsSet) {
        fParticleGun->SetParticleEnergy(fParticleEnergy);
        //G4cerr << "primaryE=" << fParticleEnergy << G4endl;
    }
    else if (fEnergyRangeIsSet) {
        G4double particleEnergy = fParticleEnergy0 + (G4double)G4UniformRand()*(fParticleEnergy1-fParticleEnergy0);
        fParticleGun->SetParticleEnergy(particleEnergy);
    }
    else {
        G4cerr << "User must set primary Energy!" << G4endl;
    }

    fParticleGun->GeneratePrimaryVertex(anEvent);
    fParticleSource->GeneratePrimaryVertex(anEvent);
}

void HEROPrimaryGenerator::ReadFluxTXT() {
    std::ifstream fin("../project/input/IntPam2010.txt");
    if (!fin.is_open()) {
        G4cerr << "Can't find IntPam2010.txt!" << G4endl;
    }

    G4int nPoints = std::count(std::istreambuf_iterator<char>(fin),
                               std::istreambuf_iterator<char>(), '\n');
    fin.seekg(0, std::ios::beg);

    G4String info="";
    for (Int_t i=0; i<7; i++) {fin >> info;}

    TVectorD energy(nPoints);
    TVectorD flux(nPoints);
    G4int i=0;

    while (!fin.eof()) {
        G4double curE; // GeV
        G4double curFlux; // particles/m^2 sr s
        fin >> curE >> curFlux;
        energy(i) = curE;
        flux(i) = curFlux;
        if (fin.eof()) break;
        i++;
    }
    fin.close();

    fMinFlux = flux(nPoints - 1);
    fMaxFlux = flux(0);

    energyInvCDFGr = new TGraph(flux, energy);
    fEnergyInvCDF = new TF1("EnergyInvCDF", EnergyInvCDF, fMinFlux, fMaxFlux, 0);
}

G4double HEROPrimaryGenerator::GetBackgroundPrimaryEnergy() 
{
    G4double rndflux = fMinFlux + G4UniformRand() * (fMaxFlux - fMinFlux);
    G4double eval = fEnergyInvCDF->Eval(rndflux);
    return eval;   
}