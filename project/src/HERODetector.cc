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
	G4VPhysicalVolume* vol = track->GetVolume();
	const G4String volname = vol->GetLogicalVolume()->GetName();
	G4bool statusOn = aStep->IsFirstStepInVolume();
	G4bool statusOff = aStep->IsLastStepInVolume();
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

	//track->SetTrackStatus(fStopAndKill); should track be killed???

	G4ThreeVector positionParticle = preStepPoint->GetPosition();
	G4ThreeVector positionParticleOff = postStepPoint->GetPosition();
	const G4ParticleDefinition* particleDefinition = track->GetParticleDefinition();
	const G4String& particleName = particleDefinition->GetParticleName();
	G4int pdg = particleDefinition->GetPDGEncoding();
	const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
	G4int copyNo = touchable->GetCopyNumber();
	G4double globalTime = track->GetGlobalTime();

	const G4DynamicParticle* dParticle = track->GetDynamicParticle();
	G4double depositEnergy = aStep->GetTotalEnergyDeposit();
	G4double kinEnergy = dParticle->GetKineticEnergy();

/*
    // -- DEBUG ------------------------------------------------------------------------------------------------------------
    G4double radiusOn = positionParticle[0]*positionParticle[0];
    radiusOn += positionParticle[1]*positionParticle[1];
    radiusOn += positionParticle[2]*positionParticle[2];
    radiusOn = sqrt(radiusOn)/cm;
    G4double radiusOff = positionParticleOff[0]*positionParticleOff[0];
    radiusOff += positionParticleOff[1]*positionParticleOff[1];
    radiusOff += positionParticleOff[2]*positionParticleOff[2];
    radiusOff = sqrt(radiusOff)/cm;
	G4cerr << statusOn << ", " << statusOff << " :";
	G4cerr << particleName << ", pdg=" << pdg << ", trackId=" << track->GetTrackID() << ", parentId=" << track->GetParentID()
	       << ", eKin=" << kinEnergy << ", edep=" << depositEnergy
	       << ", rOn=" << radiusOn << ", rOff=" << radiusOff << ", time=" << globalTime / nanosecond
	       << ", volname=" << volname << G4endl;
	// ------------------------------------------------------------------------------------------------------------------------
*/

    // Save data only for alpha particles
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	if (pdg == 1000020040) {
	    man->FillNtupleDColumn(0, depositEnergy); // MeV
	    man->FillNtupleDColumn(1, kinEnergy); // MeV
	    man->FillNtupleDColumn(2, positionParticle[0]);
	    man->FillNtupleDColumn(3, positionParticle[1]);
	    man->FillNtupleDColumn(4, positionParticle[2]);
	    man->FillNtupleDColumn(5, globalTime); // nanosecond
	    man->AddNtupleRow(0);
	}

    // 2112 neutron pdg
    // 1000020040 alpha pdg

	return true;
}
