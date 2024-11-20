#include "HEROPrimaryGenerator.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4IonTable.hh"


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
    }

    fParticleGun->SetParticleDefinition(particle);

    G4ThreeVector pos(0., 0., -fR);
    fParticleGun->SetParticlePosition(pos);
    
    G4double px = 0.;
    G4double py = 0.;
    G4double pz = 1.;
    G4ThreeVector mom(px, py, pz);
    fParticleGun->SetParticleMomentumDirection(mom);

    // Energy
    if (fEnergyIsSet) {
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