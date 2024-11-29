#include "HEROEventAction.hh"
#include "HERORunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"

HEROEventAction::HEROEventAction(HERORunAction* runAction)
: fRunAction(runAction)
{}

void HEROEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
}

void HEROEventAction::EndOfEventAction(const G4Event *event)
{
    G4double eventID = event->GetEventID();
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, 0, eventID);
    man->FillNtupleDColumn(0, 1, fEdep); // MeV
    man->FillNtupleIColumn(0, 2, fNOpticalPhotons);
    man->AddNtupleRow(0);
}

