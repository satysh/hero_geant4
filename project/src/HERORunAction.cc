#include "HERORunAction.hh"
#include "G4AnalysisManager.hh"

HERORunAction::HERORunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->SetNtupleMerging(false);

	man->CreateNtuple("HERO", "Data");
	man->CreateNtupleIColumn("eventId");
	man->CreateNtupleIColumn("pdg");
	man->CreateNtupleDColumn("deposit_E");
	man->CreateNtupleDColumn("kin_E");
	man->CreateNtupleDColumn("x");
	man->CreateNtupleDColumn("y");
	man->CreateNtupleDColumn("z");
	man->CreateNtupleDColumn("t");
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
