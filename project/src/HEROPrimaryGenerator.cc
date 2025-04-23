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

    // Energy and Time
    if (fBackgroundMCIsSet) {
        G4double startTime = (G4double)G4UniformRand() * fBackgroundDTime; // nanoseconds
        fParticleGun->SetParticleTime(startTime); // nanoseconds
        G4double particleEnergy = GetBackgroundPrimaryEnergy();
        fParticleGun->SetParticleEnergy(particleEnergy);

        G4double theta = (-90. + G4UniformRand()*180.)*deg;
        G4double phi = G4UniformRand()*360.*deg;
        px = sin(theta)*cos(phi);
        py = sin(theta)*sin(phi);
        pz = cos(theta);

        // Debug
        //G4cerr << fParticleGun->GetParticleTime() * 0.001 << " " << fParticleGun->GetParticleEnergy() * 0.001 << G4endl;
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

    G4ThreeVector mom(px, py, pz);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->GeneratePrimaryVertex(anEvent);
    fParticleSource->GeneratePrimaryVertex(anEvent);
}

void HEROPrimaryGenerator::ReadFluxTXT() {
    std::ifstream fin("../project/input/cumulative_func_2010.txt");
    if (!fin.is_open()) {
        G4cerr << "Can't find cumulative_func_2010.txt!" << G4endl;
    }

    G4int nPoints = std::count(std::istreambuf_iterator<char>(fin),
                               std::istreambuf_iterator<char>(), '\n');
    fin.seekg(0, std::ios::beg);

    TVectorD energy(nPoints + 1);
    TVectorD flux(nPoints + 1);
    G4int i=0;

    while (!fin.eof()) {
        G4double curE; // GeV
        G4double curFlux; 
        fin >> curE >> curFlux;
        energy(i) = curE;
        flux(i) = curFlux;
        if (fin.eof()) break;
        i++;
    }
    fin.close();

    energyInvCDFGr = new TGraph(flux, energy);
    fEnergyInvCDF = new TF1("EnergyInvCDF", EnergyInvCDF, 0., 1., 0);
}

G4double HEROPrimaryGenerator::GetBackgroundPrimaryEnergy() 
{
    G4double rndflux = G4UniformRand();
    G4double eval = fEnergyInvCDF->Eval(rndflux);
    return eval * GeV;   
}