#include "HEROSteppingAction.hh"
#include "HEROEventAction.hh"

#include "G4Step.hh"
//#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"

HEROSteppingAction::HEROSteppingAction(HEROEventAction* eventAction)
: fEventAction(eventAction)
{}

void HEROSteppingAction::UserSteppingAction(const G4Step* step)
{
    // get track
    G4Track* track = step->GetTrack();
    const G4ParticleDefinition* particleDefinition = track->GetParticleDefinition();
    const G4String& particleName = particleDefinition->GetParticleName();

    // get volume of the current step
    G4String volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();

    G4double edepStep = step->GetTotalEnergyDeposit();

    // Primary info accumulate
    G4double trackID = track->GetTrackID();
    if (trackID == 1) {
        if (edepStep > fEventAction->GetMaxEdep()) {
            G4StepPoint *preStepPoint = step->GetPreStepPoint();
            G4StepPoint *postStepPoint = step->GetPostStepPoint();


            G4ThreeVector positionParticleOn = preStepPoint->GetPosition();
            G4ThreeVector positionParticleOff = postStepPoint->GetPosition();
            G4double x = 0.5 * (positionParticleOn[0] + positionParticleOff[0]);
            G4double y = 0.5 * (positionParticleOn[1] + positionParticleOff[1]);
            G4double z = 0.5 * (positionParticleOn[2] + positionParticleOff[2]);
            G4double globalTime = track->GetGlobalTime();

            fEventAction->AddMaxEdep(edepStep);
            fEventAction->AddMaxEdepXYZT(x, y, z, globalTime);
        }
    }

    // sensetive part
    if (volume.contains("logicBorScint_")) {
        // collect energy deposited in this step
        fEventAction->AddEdep(edepStep); // accumulate statistics in run action

        // opticalphoton
        if (particleName == "opticalphoton") {
            fEventAction->AddOpticalPhoton();
            G4double globalTime = track->GetGlobalTime();
            
            G4AnalysisManager* man = G4AnalysisManager::Instance();
            man->FillH1(0, globalTime * 0.001);
            

            track->SetTrackStatus(fStopAndKill);
        }
    }
    else if (volume.contains("logicWorld")) {
        const G4DynamicParticle* dParticle = track->GetDynamicParticle();
        G4double kinEnergy = dParticle->GetKineticEnergy();
        fEventAction->AddWEdep(kinEnergy); // accumulate statistics in run action

        track->SetTrackStatus(fStopAndKill);
    }

    // check
    if (particleName == "opticalphoton") {
        track->SetTrackStatus(fStopAndKill);
        if (volume.contains("logicWorld")) {
            G4cerr << "opticalphoton in World!" << G4endl;
        }
        else if (volume.contains("logicWolfram_")) {
            G4cerr << "opticalphoton in Wolfram!" << G4endl;
        }
    }

}
