#include "HEROSteppingAction.hh"
#include "HEROEventAction.hh"

#include "G4Step.hh"

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

    // sensetive part
    if (volume.contains("logicBorScint_")) {
        // collect energy deposited in this step
        G4double edepStep = step->GetTotalEnergyDeposit();

        // accumulate statistics in run action
        fEventAction->AddEdep(edepStep);   

        // opticalphoton
        if (particleName == "opticalphoton") {
            fEventAction->AddOpticalPhoton();
            track->SetTrackStatus(fStopAndKill);
        }
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
