#include "HEROPrimaryGenerator.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

HEROPrimaryGenerator::HEROPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
    fParticleSource = new G4GeneralParticleSource();
}

HEROPrimaryGenerator::~HEROPrimaryGenerator()
{
    delete fParticleGun;
    delete fParticleSource;
}

void HEROPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
/*    
    G4int eventId = anEvent->GetEventID();
    G4cerr << "Event: " << eventId << G4endl;
*/
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("proton");
    if (!particle) {
        G4cerr << "Can't find particle proton!" << G4endl;
    }

    G4ThreeVector pos(0.,0.,-125.*cm);
    G4double theta = (-90. + G4UniformRand()*180.)*deg;
    G4double phi = G4UniformRand()*360.*deg;
    G4double px = sin(theta)*cos(phi);
    G4double py = sin(theta)*sin(phi);
    G4double pz = cos(theta);
    G4ThreeVector mom(px, py, pz);

    if (fMaxStartTimeIsSet) {
        G4double start_time = G4UniformRand()*fParticleMaxStartTime; // nanoseconds
        fParticleGun->SetParticleTime(start_time); // nanoseconds
        // Debug
        G4cerr << "HEROPrimaryGenerator::GeneratePrimaries_start_time= ";
        G4cerr << fParticleGun->GetParticleTime() << G4endl;
    }

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(fParticleEnergy*GeV);

    fParticleGun->GeneratePrimaryVertex(anEvent);
    fParticleSource->GeneratePrimaryVertex(anEvent);
}
