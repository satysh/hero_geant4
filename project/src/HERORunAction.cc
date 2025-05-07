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
	man->CreateNtupleDColumn(2, "edepx");
	man->CreateNtupleDColumn(2, "edepy");
	man->CreateNtupleDColumn(2, "edepz");
	man->CreateNtupleDColumn(2, "edept");
	man->CreateNtupleIColumn(2, "eventid");
	man->FinishNtuple(2);
	
	// Optic time
	man->CreateNtuple("optic_global_time", "optic_global_time");
	man->CreateNtupleDColumn(3, "opticalphoton_global_time");
	man->CreateNtupleIColumn(3, "eventid");
	man->FinishNtuple(3);

	// Edep time
	man->CreateNtuple("edep_global_time", "edep_global_time");
	man->CreateNtupleDColumn(4, "edep");
	man->CreateNtupleDColumn(4, "edept");
	man->CreateNtupleIColumn(4, "eventid");
	man->CreateNtupleIColumn(4, "pdg");
	man->FinishNtuple(4);

	// Particles
	man->CreateNtuple("particles_agg_edep", "particles_agg_edep");
	man->CreateNtupleIColumn(5, "eventid");
	man->CreateNtupleIColumn(5, "pdg");
	man->CreateNtupleDColumn(5, "edep");
	man->CreateNtupleIColumn(5, "nparticles");
	man->FinishNtuple(5);

	// Digi
	man->CreateNtuple("digi", "digi");
	man->CreateNtupleIColumn(6, "eventid");
	man->CreateNtupleDColumn(6, "edep");
	man->FinishNtuple(6);
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
