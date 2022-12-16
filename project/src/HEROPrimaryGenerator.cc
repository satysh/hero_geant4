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
    ReadFluxTXT();
}

HEROPrimaryGenerator::~HEROPrimaryGenerator()
{
    delete fParticleGun;
    delete fParticleSource;
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
    G4ParticleDefinition *particle = particleTable->FindParticle("proton");
    //G4ParticleDefinition *particle = G4IonTable::GetIonTable()->GetIon(26, 56, 0); // Fe-56
    if (!particle) {
        G4cerr << "Can't find particle proton!" << G4endl;
    }

    G4ThreeVector pos(0.,0.,-125.*cm);
    G4double theta = (-90. + (G4double)G4UniformRand()*180.)*deg;
    G4double phi = (G4double)G4UniformRand()*360.*deg;
    G4double px = sin(theta)*cos(phi);
    G4double py = sin(theta)*sin(phi);
    G4double pz = cos(theta);
    G4ThreeVector mom(px, py, pz);

    if (fMaxStartTimeIsSet) {
        G4double start_time = (G4double)G4UniformRand()*fParticleMaxStartTime; // nanoseconds
        fParticleGun->SetParticleTime(start_time); // nanoseconds
        // Debug
        //G4cerr << "HEROPrimaryGenerator::GeneratePrimaries_start_time= ";
        //G4cerr << fParticleGun->GetParticleTime() << G4endl;
    }

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);

    if (fEnergyIsSet) {
        G4cerr << "Event: " << eventId << " ";
        G4double start_time = 0.*(eventId+1)*100.*1000.; // useconds remove me
        //fParticleGun->SetParticleTime(start_time); // useconds remove me
        fParticleGun->SetParticleEnergy(fParticleEnergy*GeV);
        G4cerr << fParticleEnergy << ", " << start_time << G4endl;
        //fParticleEnergy += 10.; // remove me
    }
    else if (fEnergyRangeIsSet) {
        G4double particleEnergy = fParticleEnergy0 + (G4double)G4UniformRand()*(fParticleEnergy1-fParticleEnergy0);
        fParticleGun->SetParticleEnergy(particleEnergy*GeV);
        G4cerr << "primaryE=" << particleEnergy << G4endl;
    }
    else {
        G4double particleEnergy = PrimaryEGen();
        fParticleGun->SetParticleEnergy(particleEnergy*GeV);
        G4cerr << particleEnergy << G4endl;
    }

    fParticleGun->GeneratePrimaryVertex(anEvent);
    fParticleSource->GeneratePrimaryVertex(anEvent);
}

void HEROPrimaryGenerator::ReadFluxTXT() {
    std::ifstream fin("../project/input/IntPam2009.txt");
    if (!fin.is_open()) {
        G4cerr << "Can't find IntPam2009.txt!" << G4endl;
    }

    G4String info="";
    for (Int_t i=0; i<7; i++) {fin >> info;}

    const G4int nPoints = 71;
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

    energyInvCDFGr = new TGraph(flux, energy);
    fEnergyInvCDF = new TF1("thetaInvCDF", EnergyInvCDF, 80.73, 7808.99, 0);
}

G4double HEROPrimaryGenerator::PrimaryEGen() {
    G4double rndflux = 80.73 + (G4double)G4UniformRand()*7808.99;
    G4double eval = fEnergyInvCDF->Eval(rndflux);
    return eval;
}
