#include "HEROEventAction.hh"
#include "HERORunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"

HEROEventAction::HEROEventAction(HERORunAction* runAction)
: fRunAction(runAction)
{}

void HEROEventAction::BeginOfEventAction(const G4Event *event)
{
    fEdep = 0.;
    fWEdep = 0.;
    fNOpticalPhotons = 0;
    SetIsPrimaryFirstStep(true);
    fEventID = event->GetEventID();
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

    man->FillNtupleDColumn(2, 0, fMaxEdepX); // mm
    man->FillNtupleDColumn(2, 1, fMaxEdepY); // mm
    man->FillNtupleDColumn(2, 2, fMaxEdepZ); // mm
    man->FillNtupleDColumn(2, 3, fMaxEdepTime); // n
    man->FillNtupleIColumn(2, 4, eventID);
    man->AddNtupleRow(2);
}

