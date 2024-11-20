#include "HEROSteppingAction.hh"
#include "HEROEventAction.hh"

#include "G4Step.hh"

HEROSteppingAction::HEROSteppingAction(HEROEventAction* eventAction)
: fEventAction(eventAction)
{}

void HEROSteppingAction::UserSteppingAction(const G4Step* step)
{
    // get volume of the current step
    G4String volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();

    if (volume.contains("logicBorScint_")) {
        // collect energy deposited in this step
        G4double edepStep = step->GetTotalEnergyDeposit();

        // accumulate statistics in run action
        fEventAction->AddEdep(edepStep);   
    }
}
