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

    void SetParticleEnergy(G4double particleE)  { fParticleEnergy=particleE; }
    void SetParticleMaxStartTime(G4double time) { fParticleMaxStartTime=time; }
    virtual void GeneratePrimaries(G4Event*);

private:
    G4double fParticleEnergy=0.;
    G4double fParticleMaxStartTime=0.;
    G4ParticleGun *fParticleGun;
    G4GeneralParticleSource *fParticleSource;
};

#endif
