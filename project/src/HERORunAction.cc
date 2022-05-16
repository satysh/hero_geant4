#include "HERORunAction.hh"
#include "G4AnalysisManager.hh"

HERORunAction::HERORunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->SetNtupleMerging(true);

	man->CreateNtuple("all", "all");
	man->CreateNtupleDColumn("deposit_E");
	man->CreateNtupleDColumn("kin_E");
	man->CreateNtupleDColumn("x");
	man->CreateNtupleDColumn("y");
	man->CreateNtupleDColumn("z");
	man->CreateNtupleDColumn("t");
	man->FinishNtuple(0);

	man->CreateNtuple("alpha", "alpha");
	man->CreateNtupleDColumn("deposit_E");
	man->CreateNtupleDColumn("kin_E");
	man->CreateNtupleDColumn("x");
	man->CreateNtupleDColumn("y");
	man->CreateNtupleDColumn("z");
	man->CreateNtupleDColumn("t");
	man->FinishNtuple(1);

	man->CreateNtuple("neutron", "neutron");
	man->CreateNtupleDColumn("deposit_E");
	man->CreateNtupleDColumn("kin_E");
	man->CreateNtupleDColumn("x");
	man->CreateNtupleDColumn("y");
	man->CreateNtupleDColumn("z");
	man->CreateNtupleDColumn("t");
	man->FinishNtuple(2);
	for (int i = 0; i <= 8; ++i) {
		man->CreateNtuple("all_"+to_string(i), "all_" + to_string(i));
		man->CreateNtupleDColumn("deposit_E");
		man->CreateNtupleDColumn("kin_E");
		man->CreateNtupleDColumn("x");
		man->CreateNtupleDColumn("y");
		man->CreateNtupleDColumn("z");
		man->CreateNtupleDColumn("t");
		man->FinishNtuple(i+3);
	}
	for (int i = 0; i <= 8; ++i) {
		man->CreateNtuple("alpha_" + to_string(i), "alpha_" + to_string(i));
		man->CreateNtupleDColumn("deposit_E");
		man->CreateNtupleDColumn("kin_E");
		man->CreateNtupleDColumn("x");
		man->CreateNtupleDColumn("y");
		man->CreateNtupleDColumn("z");
		man->CreateNtupleDColumn("t");
		man->FinishNtuple(i+12);
	}
	for (int i = 0; i <= 8; ++i) {
		man->CreateNtuple("neutron_" + to_string(i), "neutron_" + to_string(i));
		man->CreateNtupleDColumn("deposit_E");
		man->CreateNtupleDColumn("kin_E");
		man->CreateNtupleDColumn("x");
		man->CreateNtupleDColumn("y");
		man->CreateNtupleDColumn("z");
		man->CreateNtupleDColumn("t");
		man->FinishNtuple(i + 21);
	}
}

HERORunAction::~HERORunAction()
{}

void HERORunAction::BeginOfRunAction(const G4Run* run)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	G4int runID = run->GetRunID();
	std::stringstream strRunID;
	strRunID << runID;
	man->OpenFile("output" + strRunID.str() + ".root");

}
void HERORunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->Write();
	man->CloseFile();
}
