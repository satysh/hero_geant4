void now_r_hist_build(Int_t R=125, Int_t year=2010, Int_t nevents=10000, TString bopt="b", TFile *out_file=NULL);
void build_background_edep_hists()
{
	Int_t year = 2010;
	Int_t nevents = 10000;
	TString bopt = "b";
	TString out_file_name;
	if (bopt == "wb")
		out_file_name.Form("alpha_wb_%d_edep_hists_diff_r.root", year);
	else 
		out_file_name.Form("del_e_%d_edep_hists_diff_r.root", year);
	TFile *out_file = new TFile(out_file_name, "RECREATE");

	const Int_t n_poinst = 1;
	Int_t r_list[n_poinst] = {125};

	for (Int_t i=0; i<n_poinst; i++) {
		now_r_hist_build(r_list[i], year, nevents, bopt, out_file);
	}
}

void now_r_hist_build(Int_t R=125, Int_t year=2010, Int_t nevents=10000, TString bopt="b", TFile *out_file=NULL)
{
	TString input_file_name;
	if (bopt == "b")
		input_file_name.Form("../input/%d/b/%d_background_100_usec_nevents_%d_r_%d.root", year, year, nevents, R);
	else if (bopt == "wb")
		input_file_name.Form("../input/%d/wb/%d_C_46_H_54_background_100_usec_nevents_%d_r_%d.root", year, year, nevents, R);
	else {
		cerr << "Unknown boron option! Set b or wb!" << endl;
		return;
	}

	TFile *file = new TFile(input_file_name, "READ");
	if (file->IsZombie()) {
		cerr << "Can't read file " << input_file_name << endl;
		return;
	}

	TString now_hist_name;
	now_hist_name.Form("background_edep_r_%d", R);
	Int_t bin_n = 200;
	Double_t max_bin_val = (Double_t)bin_n;
	TH1F *now_hist = new TH1F(now_hist_name, now_hist_name, bin_n, 0., max_bin_val);
	TString tree_name[2] = {"HERO", "HERO_absorber"};
	for (Int_t i=0; i<1; i++) {
		TTree *tree = (TTree*)file->Get(tree_name[i].Data());
		if (!tree) {
			cerr << "Can't find " << tree_name[i] << " tree in file " << input_file_name << endl;
			return;
		}
		Int_t now_nentries = tree->GetEntries();
		cout << tree_name[i] << " netries = " << now_nentries << endl;
		Double_t edep;
		Double_t time;
		Int_t pdg;
		tree->SetBranchAddress("deposit_E", &edep);
		tree->SetBranchAddress("t", &time);
		tree->SetBranchAddress("pdg", &pdg);
		for (Int_t i=0; i<now_nentries; i++) {
			tree->GetEntry(i);
			// time * 0.001 to usec
			if (pdg != 11)
				now_hist->AddBinContent(now_hist->FindBin(time * 0.001), edep); 
		}

	}
	out_file->cd();
	now_hist->Write();
}