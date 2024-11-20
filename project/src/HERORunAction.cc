#include "HERORunAction.hh"
#include "G4AnalysisManager.hh"

HERORunAction::HERORunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->SetNtupleMerging(false);

	// Scintillator
	man->CreateNtuple("HERO", "Data");
	man->CreateNtupleIColumn(0, "eventid");
	man->CreateNtupleDColumn(0, "edep");
	man->FinishNtuple(0);
}

HERORunAction::~HERORunAction()
{}

void HERORunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->OpenFile(fOutFileName);
}
void HERORunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->Write();
	man->CloseFile();
}
