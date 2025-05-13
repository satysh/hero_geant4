void extract(Int_t energy)
{
	TString fileName;
	fileName.Form("hero_nevents_1000_pdg_2212_R_125_E_%d_bron_b_percent_5_absorber.root", energy * 1000);

	auto file = new TFile(fileName, "READ");

	auto HERO = (TTree*)file->Get("HERO");
	auto digi = (TTree*)file->Get("digi");


	auto out = new TFile("protons/" + fileName, "RECREATE");
	out->cd();
	HERO->CloneTree()->Write();
	digi->CloneTree()->Write();
}