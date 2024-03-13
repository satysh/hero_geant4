#include "HERORunAction.hh"
#include "G4AnalysisManager.hh"

HERORunAction::HERORunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->SetNtupleMerging(false);

	man->CreateNtuple("HERO", "Data");
	man->CreateNtupleIColumn(0, "eventId");
	man->CreateNtupleIColumn(0, "pdg");
	man->CreateNtupleIColumn(0, "trackID");
	man->CreateNtupleDColumn(0, "deposit_E");
	man->CreateNtupleDColumn(0, "kin_E");
	man->CreateNtupleDColumn(0, "pre_x");
	man->CreateNtupleDColumn(0, "pre_y");
	man->CreateNtupleDColumn(0, "pre_z");
	man->CreateNtupleDColumn(0, "post_x");
	man->CreateNtupleDColumn(0, "post_y");
	man->CreateNtupleDColumn(0, "post_z");
	man->CreateNtupleDColumn(0, "t");
	man->CreateNtupleDColumn(0, "track_time");
	man->FinishNtuple(0);

	man->CreateNtuple("HERO_outside", "Data");
	man->CreateNtupleIColumn(1, "eventId");
	man->CreateNtupleIColumn(1, "pdg");
	man->CreateNtupleIColumn(1, "trackID");
	man->CreateNtupleDColumn(1, "deposit_E");
	man->CreateNtupleDColumn(1, "kin_E");
	man->CreateNtupleDColumn(1, "pre_x");
	man->CreateNtupleDColumn(1, "pre_y");
	man->CreateNtupleDColumn(1, "pre_z");
	man->CreateNtupleDColumn(1, "post_x");
	man->CreateNtupleDColumn(1, "post_y");
	man->CreateNtupleDColumn(1, "post_z");
	man->CreateNtupleDColumn(1, "t");
	man->CreateNtupleDColumn(1, "track_time");
	man->FinishNtuple(1);
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
