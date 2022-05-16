#include "HERODetector.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

HEROSensitiveDetector::HEROSensitiveDetector(G4String name) :
G4VSensitiveDetector(name)
{}
HEROSensitiveDetector::~HEROSensitiveDetector()
{}

G4bool HEROSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory*ROhist)
{
	G4Track* track = aStep->GetTrack();
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	//G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

	//track->SetTrackStatus(fStopAndKill); should track be killed???

	G4ThreeVector positionParticle = preStepPoint->GetPosition();
	const G4ParticleDefinition* particleDefinition = track->GetParticleDefinition();
	const G4String& particleName = particleDefinition->GetParticleName();
	G4int pdg = particleDefinition->GetPDGEncoding();
	const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
	G4int copyNo = touchable->GetCopyNumber();
	G4double globalTime = track->GetGlobalTime();

	const G4DynamicParticle* dParticle = track->GetDynamicParticle();
	G4double depositEnergy = aStep->GetTotalEnergyDeposit();
	G4double kinEnergy = dParticle->GetKineticEnergy();


	//G4cout << "The Particle NAME: " << particleName << " with PDG = " << pdg << G4endl;
	//G4cout << "some particle position: " << positionParticle << G4endl;
	//G4cout << "copy number " << copyNo << G4endl;
	//G4cout << particleName << " time: " << globalTime << ", energy: " << particleEnergy << " no kill" << G4endl;

	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->FillNtupleDColumn(0, depositEnergy / keV);
	man->FillNtupleDColumn(1, kinEnergy / keV);
	man->FillNtupleDColumn(2, positionParticle[0]);
	man->FillNtupleDColumn(3, positionParticle[1]);
	man->FillNtupleDColumn(4, positionParticle[2]);
	man->FillNtupleDColumn(5, globalTime / nanosecond);
	man->AddNtupleRow(0);

    // 2112 neutron
    // 1000020040 alpha

	return true;
}
