#include "HERORunAction.hh"
#include "G4AnalysisManager.hh"

HERORunAction::HERORunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->SetNtupleMerging(true);

	// Scintillator
	man->CreateNtuple("HERO", "Data");
	man->CreateNtupleIColumn(0, "eventid");
	man->CreateNtupleDColumn(0, "edep");
	man->CreateNtupleIColumn(0, "nopticalphotons");
	man->FinishNtuple(0);

	// World
	man->CreateNtuple("AHERO", "Data");
	man->CreateNtupleIColumn(1, "eventid");
	man->CreateNtupleDColumn(1, "edep");
	man->FinishNtuple(1);

	// Primary
	man->CreateNtuple("PRIMARY", "Data");
	man->CreateNtupleDColumn(2, "maxedep");
	man->CreateNtupleDColumn(2, "maxedepx");
	man->CreateNtupleDColumn(2, "maxedepy");
	man->CreateNtupleDColumn(2, "maxedepz");
	man->CreateNtupleDColumn(2, "maxedept");
	man->FinishNtuple(2);
	
	man->CreateH1("global_time", "global_time", 1700, 0., 17.);
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
