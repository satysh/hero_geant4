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
    if (fEnergyInvCDF)
        delete fEnergyInvCDF;
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
    //G4ParticleDefinition* particle = Iontable->GetIon(6, 12, 0); // Ñ-12
    //G4ParticleDefinition* particle = Iontable->GetIon(8, 16, 0); // O-16
    //G4ParticleDefinition *particle = G4IonTable::GetIonTable()->GetIon(26, 56, 0); // Fe-56
    if (!particle) {
        G4cerr << "Can't find the particle with pdg: " << fPrimaryParticlePDG << G4endl;
    }

    fParticleGun->SetParticleDefinition(particle);

    G4ThreeVector pos(0.,0.,-G4double(fR)*cm);
    G4double cost = (G4double)G4UniformRand();
    G4double phi = (G4double)G4UniformRand()*360.*deg;
    cost = 1.; // DELETE ME
    G4double px = (1. - cost)*cos(phi);
    G4double py = (1. - cost)*sin(phi);
    G4double pz = cost;
    G4ThreeVector mom(px, py, pz);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);

    if (fMaxStartTimeIsSet) {
        G4double start_time = (G4double)G4UniformRand()*fParticleMaxStartTime; // nanoseconds
        fParticleGun->SetParticleTime(start_time); // nanoseconds
        // Debug
        //G4cerr << "start_time=" << fParticleGun->GetParticleTime() << G4endl;
    }
    else if (fMaxFixedTimeIsSet) {
        fParticleGun->SetParticleTime(fParticleFixedStartTime);
        //G4cerr << "start_time=" << fParticleGun->GetParticleTime() << G4endl;
    }

    if (fEnergyIsSet) {
        fParticleGun->SetParticleEnergy(fParticleEnergy*GeV);
        //G4cerr << "primaryE=" << fParticleEnergy << G4endl;
    }
    else if (fEnergyRangeIsSet) {
        G4double particleEnergy = fParticleEnergy0 + (G4double)G4UniformRand()*(fParticleEnergy1-fParticleEnergy0);
        fParticleGun->SetParticleEnergy(particleEnergy*GeV);
        //G4cerr << "primaryE=" << particleEnergy << G4endl;
    }
    else {
        G4double particleEnergy = PrimaryEGen();
        fParticleGun->SetParticleEnergy(particleEnergy*GeV);
        //G4cerr << particleEnergy << G4endl;
    }

    fParticleGun->GeneratePrimaryVertex(anEvent);
    fParticleSource->GeneratePrimaryVertex(anEvent);
}

void HEROPrimaryGenerator::ReadFluxTXT() {
    G4cout << "[HEROcout]: Flux input file is " << fInputFluxFileName << " !" << G4endl << G4endl;

    TString path = "../project/input/";
    std::ifstream fin((path + fInputFluxFileName).Data());
    if (!fin.is_open()) {
        G4cerr << "Can't find " << fInputFluxFileName << "!" << G4endl;
    }

    G4int nPoints = std::count(std::istreambuf_iterator<char>(fin),
                               std::istreambuf_iterator<char>(), '\n');
    fin.seekg(0, std::ios::beg);

    TVectorD energy(nPoints + 1);
    TVectorD flux(nPoints + 1);
    G4int i=0;

    while (!fin.eof()) {
        G4double curE; // GeV
        G4double curFlux; // particles/m^2 sr s
        fin >> curE >> curFlux;
        energy(i) = curE;
        flux(i) = curFlux;
        //G4cerr << energy(i) << " " << flux(i) << G4endl;
        if (fin.eof()) break;
        i++;
    }
    fin.close();

    energyInvCDFGr = new TGraph(flux, energy);
    fEnergyInvCDF = new TF1("EnergyInvCDF", EnergyInvCDF, 0., 1., 0);
}

G4double HEROPrimaryGenerator::PrimaryEGen() {
    G4double rndflux = G4UniformRand();
    G4double eval = fEnergyInvCDF->Eval(rndflux);
    return eval;
}
