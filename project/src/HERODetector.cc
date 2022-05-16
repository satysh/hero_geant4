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
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

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
	man->FillNtupleDColumn(0, 0, depositEnergy / keV);
	man->FillNtupleDColumn(0, 1, kinEnergy / keV);
	man->FillNtupleDColumn(0, 2, positionParticle[0]);
	man->FillNtupleDColumn(0, 3, positionParticle[1]);
	man->FillNtupleDColumn(0, 4, positionParticle[2]);
	man->FillNtupleDColumn(0, 5, globalTime / nanosecond);
	man->AddNtupleRow(0);

    if (pdg == 1000020040) {
		man->FillNtupleDColumn(1, 0, depositEnergy / keV);
		man->FillNtupleDColumn(1, 1, kinEnergy / keV);
		man->FillNtupleDColumn(1, 2, positionParticle[0]);
		man->FillNtupleDColumn(1, 3, positionParticle[1]);
		man->FillNtupleDColumn(1, 4, positionParticle[2]);
		man->FillNtupleDColumn(1, 5, globalTime / nanosecond);
		man->AddNtupleRow(1);
    }
	if (pdg == 2112) {
		man->FillNtupleDColumn(2, 0, depositEnergy / keV);
		man->FillNtupleDColumn(2, 1, kinEnergy / keV);
		man->FillNtupleDColumn(2, 2, positionParticle[0]);
		man->FillNtupleDColumn(2, 3, positionParticle[1]);
		man->FillNtupleDColumn(2, 4, positionParticle[2]);
		man->FillNtupleDColumn(2, 5, globalTime / nanosecond);
		man->AddNtupleRow(2);
	}
	for (int i = 0; i <= 8; ++i) {
		if (copyNo == i) {
			man->FillNtupleDColumn(i+3, 0, depositEnergy / keV);
			man->FillNtupleDColumn(i + 3, 1, kinEnergy / keV);
			man->FillNtupleDColumn(i + 3, 2, positionParticle[0]);
			man->FillNtupleDColumn(i + 3, 3, positionParticle[1]);
			man->FillNtupleDColumn(i + 3, 4, positionParticle[2]);
			man->FillNtupleDColumn(i + 3, 5, globalTime / nanosecond);
			man->AddNtupleRow(i + 3);
			if (pdg == 1000020040) {
				man->FillNtupleDColumn(i+12, 0, depositEnergy / keV);
				man->FillNtupleDColumn(i + 12, 1, kinEnergy / keV);
				man->FillNtupleDColumn(i + 12, 2, positionParticle[0]);
				man->FillNtupleDColumn(i + 12, 3, positionParticle[1]);
				man->FillNtupleDColumn(i + 12, 4, positionParticle[2]);
				man->FillNtupleDColumn(i + 12, 5, globalTime / nanosecond);
				man->AddNtupleRow(i + 12);
			}
			if (pdg == 2112) {
				man->FillNtupleDColumn(i+21, 0, depositEnergy / keV);
				man->FillNtupleDColumn(i + 21, 1, kinEnergy / keV);
				man->FillNtupleDColumn(i + 21, 2, positionParticle[0]);
				man->FillNtupleDColumn(i + 21, 3, positionParticle[1]);
				man->FillNtupleDColumn(i + 21, 4, positionParticle[2]);
				man->FillNtupleDColumn(i + 21, 5, globalTime / nanosecond);
				man->AddNtupleRow(i + 21);
			}
		}
	}

	return true;
}
