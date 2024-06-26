#ifndef HEROPRIMARYGENERATOR_HH
#define HEROPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"

class TF1;
#include "TString.h"

class HEROPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    HEROPrimaryGenerator();
    ~HEROPrimaryGenerator();

    void SetParticleEnergy(G4double particleE)  { fParticleEnergy=particleE; fEnergyIsSet=true; }
    void SetParticleEnergy(G4double particleE0, G4double particleE1);
    void SetParticleMaxStartTime(G4double time) { fParticleMaxStartTime=time; fMaxStartTimeIsSet=true; }
    void SetParticleFixedStartTime(G4double time) { fParticleFixedStartTime=time; fMaxFixedTimeIsSet=true;}
    void SetPrimaryParticle(G4int pdgcode) { fPrimaryParticlePDG=pdgcode; }
    void SetR(G4int r) { fR=r; }
    void SetInputFluxFileName(TString name) { fInputFluxFileName=name; }
    virtual void GeneratePrimaries(G4Event*);
    void ReadFluxTXT();

private:
    G4double PrimaryEGen();

private:
    TF1* fEnergyInvCDF=NULL;
    TString fInputFluxFileName="";

private:
    G4double fParticleEnergy=0.;
    G4double fParticleEnergy0=0.;
    G4double fParticleEnergy1=0.;
    G4double fParticleMaxStartTime=0.;
    G4double fParticleFixedStartTime=0.;
    G4int    fPrimaryParticlePDG=2212;

private:
    G4ParticleGun *fParticleGun;
    G4GeneralParticleSource *fParticleSource;

private:
    G4bool fMaxStartTimeIsSet=false;
    G4bool fMaxFixedTimeIsSet=false;
    G4bool fEnergyIsSet=false;
    G4bool fEnergyRangeIsSet=false;

    G4double fMinFlux;
    G4double fMaxFlux;
    G4int fR=125; // detector radius in cm
};

#endif
