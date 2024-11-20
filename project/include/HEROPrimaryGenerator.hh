#ifndef HEROPRIMARYGENERATOR_HH
#define HEROPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"

class HEROPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    HEROPrimaryGenerator();
    ~HEROPrimaryGenerator();

    void SetParticleEnergy(G4double particleE)  { fParticleEnergy=particleE; fEnergyIsSet=true; }
    void SetParticleEnergy(G4double particleE0, G4double particleE1);
    void SetPrimaryParticle(G4int pdgcode) { fPrimaryParticlePDG=pdgcode; }
    void SetR(G4int r) { fR=r; }
    virtual void GeneratePrimaries(G4Event*);

private:
    G4double fParticleEnergy=0.;
    G4double fParticleEnergy0=0.;
    G4double fParticleEnergy1=0.;
    G4int    fPrimaryParticlePDG=2212;
    G4int    fR=125; // detector radius in cm

private:
    G4ParticleGun *fParticleGun;
    G4GeneralParticleSource *fParticleSource;

private:
    G4bool fEnergyIsSet=false;
    G4bool fEnergyRangeIsSet=false;

};

#endif
