auto get_root_files_list(TString path)
{
	std::vector<TString> result;
	void *dirp = gSystem->OpenDirectory(path.Data());
	Char_t *afile;

	while ((afile = const_cast<Char_t *>(gSystem->GetDirEntry(dirp)))) {
		TString safile(afile);
		if (safile.Contains(".root")) {
			result.push_back(safile);
		}
	}

	return result;
}

void fill_tree(TTree *tree, TString file_name);

void sim_params_calculation_agg()
{
	auto files_list = get_root_files_list("tmp_output");
	TTree *tree = new TTree("params_result", "params_result");
	for (UInt_t i=0; i<files_list.size(); i++) {
		fill_tree(tree, "tmp_output/" + files_list[i]);
	}
	
	TFile *out_file = new TFile("params_result.root", "RECREATE");
	out_file->cd();
	tree->Write();
}

void fill_tree(TTree *tree, TString file_name)
{	
	TFile *file = new TFile(file_name, "READ");
	if (file->IsZombie()) {
		cerr << "Can't read file " << file_name << endl;
		return;
	}

	TTree *in_tree = (TTree*)file->Get("params");
	if (!in_tree) {
		cerr << "Can't find tree params in " << file_name << endl;
		return;
	}

	Int_t out_event_id;
	Double_t sum_edep_scint;
	Double_t sum_edep_absorb;
	Double_t sum_edep_ultra;
	Double_t sum_edep;

	Int_t n_born_neutrons;
	Int_t n_dead_neutrons;
	Double_t ratio_neutrons;

	Double_t sum_edep_e_scint;
	Double_t sum_edep_e_absorb;
	Double_t sum_edep_e_ultra;
	Double_t sum_edep_e;

	Int_t n_born_alpha;
	Int_t n_dead_alpha;
	Double_t sum_edep_alpha_scint;
	Double_t sum_edep_alpha_absorb;
	Double_t sum_edep_alpha_ultra;
	Double_t sum_edep_alpha;

	in_tree->SetBranchAddress("event_id", &out_event_id);

	in_tree->SetBranchAddress("sum_edep_scint", &sum_edep_scint);
	in_tree->SetBranchAddress("sum_edep_absorb", &sum_edep_absorb);
	in_tree->SetBranchAddress("sum_edep_ultra", &sum_edep_ultra);
	in_tree->SetBranchAddress("sum_edep", &sum_edep);

	in_tree->SetBranchAddress("n_born_neutrons", &n_born_neutrons);
	in_tree->SetBranchAddress("n_dead_neutrons", &n_dead_neutrons);
	in_tree->SetBranchAddress("ratio_neutrons", &ratio_neutrons);

	in_tree->SetBranchAddress("sum_edep_e_scint", &sum_edep_e_scint);
	in_tree->SetBranchAddress("sum_edep_e_absorb", &sum_edep_e_absorb);
	in_tree->SetBranchAddress("sum_edep_e_ultra", &sum_edep_e_ultra);
	in_tree->SetBranchAddress("sum_edep_e", &sum_edep_e);

	in_tree->SetBranchAddress("n_born_alpha", &n_born_alpha);
	in_tree->SetBranchAddress("n_dead_alpha", &n_dead_alpha);
	in_tree->SetBranchAddress("sum_edep_alpha_scint", &sum_edep_alpha_scint);
	in_tree->SetBranchAddress("sum_edep_alpha_absorb", &sum_edep_alpha_absorb);
	in_tree->SetBranchAddress("sum_edep_alpha_ultra", &sum_edep_alpha_ultra);
	in_tree->SetBranchAddress("sum_edep_alpha", &sum_edep_alpha);
	
	if (tree->GetEntries() > 0) {
		tree->SetBranchAddress("event_id", &out_event_id);

		tree->SetBranchAddress("sum_edep_scint", &sum_edep_scint);
		tree->SetBranchAddress("sum_edep_absorb", &sum_edep_absorb);
		tree->SetBranchAddress("sum_edep_ultra", &sum_edep_ultra);
		tree->SetBranchAddress("sum_edep", &sum_edep);

		tree->SetBranchAddress("n_born_neutrons", &n_born_neutrons);
		tree->SetBranchAddress("n_dead_neutrons", &n_dead_neutrons);
		tree->SetBranchAddress("ratio_neutrons", &ratio_neutrons);

		tree->SetBranchAddress("sum_edep_e_scint", &sum_edep_e_scint);
		tree->SetBranchAddress("sum_edep_e_absorb", &sum_edep_e_absorb);
		tree->SetBranchAddress("sum_edep_e_ultra", &sum_edep_e_ultra);
		tree->SetBranchAddress("sum_edep_e", &sum_edep_e);

		tree->SetBranchAddress("n_born_alpha", &n_born_alpha);
		tree->SetBranchAddress("n_dead_alpha", &n_dead_alpha);
		tree->SetBranchAddress("sum_edep_alpha_scint", &sum_edep_alpha_scint);
		tree->SetBranchAddress("sum_edep_alpha_absorb", &sum_edep_alpha_absorb);
		tree->SetBranchAddress("sum_edep_alpha_ultra", &sum_edep_alpha_ultra);
		tree->SetBranchAddress("sum_edep_alpha", &sum_edep_alpha);
	}
	else {
		tree->Branch("event_id", &out_event_id, "event_id/I");

		tree->Branch("sum_edep_scint", &sum_edep_scint, "sum_edep_scint/D");
		tree->Branch("sum_edep_absorb", &sum_edep_absorb, "sum_edep_absorb/D");
		tree->Branch("sum_edep_ultra", &sum_edep_ultra, "sum_edep_ultra/D");
		tree->Branch("sum_edep", &sum_edep, "sum_edep/D");

		tree->Branch("n_born_neutrons", &n_born_neutrons, "n_born_neutrons/I");
		tree->Branch("n_dead_neutrons", &n_dead_neutrons, "n_dead_neutrons/I");
		tree->Branch("ratio_neutrons", &ratio_neutrons, "ratio_neutrons/D");

		tree->Branch("sum_edep_e_scint", &sum_edep_e_scint, "sum_edep_e_scint/D");
		tree->Branch("sum_edep_e_absorb", &sum_edep_e_absorb, "sum_edep_e_absorb/D");
		tree->Branch("sum_edep_e_ultra", &sum_edep_e_ultra, "sum_edep_e_ultra/D");
		tree->Branch("sum_edep_e", &sum_edep_e, "sum_edep_e/D");

		tree->Branch("n_born_alpha", &n_born_alpha, "n_born_alpha/I");
		tree->Branch("n_dead_alpha", &n_dead_alpha, "n_dead_alpha/I");
		tree->Branch("sum_edep_alpha_scint", &sum_edep_alpha_scint, "sum_edep_alpha_scint/D");
		tree->Branch("sum_edep_alpha_absorb", &sum_edep_alpha_absorb, "sum_edep_alpha_absorb/D");
		tree->Branch("sum_edep_alpha_ultra", &sum_edep_alpha_ultra, "sum_edep_alpha_ultra/D");
		tree->Branch("sum_edep_alpha", &sum_edep_alpha, "sum_edep_alpha/D");
	}


	for (UInt_t i=0; i<in_tree->GetEntries(); i++) {
		in_tree->GetEntry(i);
		tree->Fill();
	}


	file->Close();
}