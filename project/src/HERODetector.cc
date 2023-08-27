#include "HERODetector.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

HEROSensitiveDetector::HEROSensitiveDetector(G4String name) :
G4VSensitiveDetector(name)
{}
HEROSensitiveDetector::~HEROSensitiveDetector()
{}

G4bool HEROSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory*ROhist)
{
	G4int eventId = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	//G4cerr << eventId << G4endl;
	G4Track* track = aStep->GetTrack();
	G4double trackID = track->GetTrackID();
	G4VPhysicalVolume* vol = track->GetVolume();
	const G4String volname = vol->GetLogicalVolume()->GetName();
	G4bool statusOn = aStep->IsFirstStepInVolume();
	G4bool statusOff = aStep->IsLastStepInVolume();
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();


	G4ThreeVector positionParticle = preStepPoint->GetPosition();
	G4ThreeVector positionParticleOff = postStepPoint->GetPosition();
	const G4ParticleDefinition* particleDefinition = track->GetParticleDefinition();
	const G4String& particleName = particleDefinition->GetParticleName();
	G4int pdg = particleDefinition->GetPDGEncoding();
	const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
	G4int copyNo = touchable->GetCopyNumber();
	G4double globalTime = track->GetGlobalTime();
	G4double localTime = track->GetLocalTime();

	const G4DynamicParticle* dParticle = track->GetDynamicParticle();
	G4double depositEnergy = aStep->GetTotalEnergyDeposit();
	G4double kinEnergy = dParticle->GetKineticEnergy();


    // 2112 neutron pdg
    // 1000020040 alpha pdg

// -----------------------------------------------------------------
	// We need It because of multithread in the other case fStartEventId=0 by default
    eventId += fStartEventId;
// -----------------------------------------------------------------

	if (pdg == 1000020040) {
		track->SetTrackStatus(fStopAndKill); // should track be killed???
	}

    if (pdg != 0 && pdg == 1000020040) { // skip geantino
	    G4AnalysisManager* man = G4AnalysisManager::Instance();
	    man->FillNtupleIColumn(0, eventId);
	    man->FillNtupleIColumn(1, pdg);
	    man->FillNtupleIColumn(2, trackID);
	    man->FillNtupleDColumn(3, depositEnergy); // MeV
	    man->FillNtupleDColumn(4, kinEnergy); // MeV
	    man->FillNtupleDColumn(5, positionParticle[0]);
	    man->FillNtupleDColumn(6, positionParticle[1]);
	    man->FillNtupleDColumn(7, positionParticle[2]);
	    man->FillNtupleDColumn(8, positionParticleOff[0]);
	    man->FillNtupleDColumn(9, positionParticleOff[1]);
	    man->FillNtupleDColumn(10, positionParticleOff[2]);
	    man->FillNtupleDColumn(11, globalTime); // nanosecond
	    man->FillNtupleDColumn(12, localTime); // nanosecond
	    man->AddNtupleRow(0);
	}

	return true;
}
