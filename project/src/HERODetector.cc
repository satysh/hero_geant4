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

// -----------------------------------------------------------------
	// We need It because of multithread in the other case fStartEventId=0 by default
    eventId += fStartEventId;
// -----------------------------------------------------------------

    // 2112 neutron pdg
    // 1000020040 alpha pdg

    if (pdg != 0 && volname != "logicWorld"/* && pdg == 1000020040*/) { // skip geantino
	    G4AnalysisManager* man = G4AnalysisManager::Instance();
	    man->FillNtupleIColumn(0, 0, eventId);
	    man->FillNtupleIColumn(0, 1, pdg);
	    man->FillNtupleIColumn(0, 2, trackID);
	    man->FillNtupleDColumn(0, 3, depositEnergy); // MeV
	    man->FillNtupleDColumn(0, 4, kinEnergy); // MeV
	    man->FillNtupleDColumn(0, 5, positionParticle[0]);
	    man->FillNtupleDColumn(0, 6, positionParticle[1]);
	    man->FillNtupleDColumn(0, 7, positionParticle[2]);
	    man->FillNtupleDColumn(0, 8, positionParticleOff[0]);
	    man->FillNtupleDColumn(0, 9, positionParticleOff[1]);
	    man->FillNtupleDColumn(0, 10, positionParticleOff[2]);
	    man->FillNtupleDColumn(0, 11, globalTime); // nanosecond
	    man->FillNtupleDColumn(0, 12, localTime); // nanosecond
	    man->AddNtupleRow(0);
	}
	else if (volname == "logicWorld") {
		G4AnalysisManager* man = G4AnalysisManager::Instance();
	    man->FillNtupleIColumn(1, 0, eventId);
	    man->FillNtupleIColumn(1, 1, pdg);
	    man->FillNtupleIColumn(1, 2, trackID);
	    man->FillNtupleDColumn(1, 3, depositEnergy); // MeV
	    man->FillNtupleDColumn(1, 4, kinEnergy); // MeV
	    man->FillNtupleDColumn(1, 5, positionParticle[0]);
	    man->FillNtupleDColumn(1, 6, positionParticle[1]);
	    man->FillNtupleDColumn(1, 7, positionParticle[2]);
	    man->FillNtupleDColumn(1, 8, positionParticleOff[0]);
	    man->FillNtupleDColumn(1, 9, positionParticleOff[1]);
	    man->FillNtupleDColumn(1, 10, positionParticleOff[2]);
	    man->FillNtupleDColumn(1, 11, globalTime); // nanosecond
	    man->FillNtupleDColumn(1, 12, localTime); // nanosecond
	    man->AddNtupleRow(1);

	    track->SetTrackStatus(fStopAndKill);
	}

	return true;
}
