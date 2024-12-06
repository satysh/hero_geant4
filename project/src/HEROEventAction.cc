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
    fWEdep = 0.;
    fNOpticalPhotons = 0;
    fMaxEdep = 0.;
}

void HEROEventAction::EndOfEventAction(const G4Event *event)
{
    G4double eventID = event->GetEventID();
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, 0, eventID);
    man->FillNtupleDColumn(0, 1, fEdep); // MeV
    man->FillNtupleIColumn(0, 2, fNOpticalPhotons);
    man->AddNtupleRow(0);

    man->FillNtupleIColumn(1, 0, eventID);
    man->FillNtupleDColumn(1, 1, fWEdep); // MeV
    man->AddNtupleRow(1);

    man->FillNtupleDColumn(2, 0, fMaxEdep); // MeV
    man->FillNtupleDColumn(2, 1, fMaxEdepX); // mm
    man->FillNtupleDColumn(2, 2, fMaxEdepY); // mm
    man->FillNtupleDColumn(2, 3, fMaxEdepZ); // mm
    man->FillNtupleDColumn(2, 4, fMaxEdepTime); // n
    man->AddNtupleRow(2);
}

