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

void FillTree(Int_t index, Int_t i, TString file_name);

void sim_params_calculation(Int_t index=0)
{
	TString dirpath = "../../output";
	auto files_list = get_root_files_list(dirpath);

	for (UInt_t i=0; i<files_list.size(); i++) {
		FillTree(index, i, dirpath + "/" + files_list[i]);
	}
}

void FillTree(Int_t index, Int_t i, TString file_name)
{
	cout << "Filling from file " << file_name << "!" << endl;

	TFile *out_file = new TFile("params.root", "UPDATE");
	out_file->cd();
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

	TString tree_name;
	tree_name.Form("params_%d_%d", index, i);
	TTree *tree = new TTree(tree_name, tree_name);

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
	

	TFile *file = new TFile(file_name, "READ");
	if (file->IsZombie()) {
		cerr << "Can't read file " << file_name << endl;
		return;
	}
	file->cd();
	TTree *scint = (TTree*)file->Get("HERO");
	if (!scint) {
		cerr << "Can't find scint tree HERO!" << endl;
		return;
	}

	Int_t event_id;
	Int_t pdg;
	Int_t track_id;
	Double_t edep;

	scint->SetBranchAddress("eventId", &event_id);
	scint->SetBranchAddress("pdg", &pdg);
	scint->SetBranchAddress("trackID", &track_id);
	scint->SetBranchAddress("deposit_E", &edep);

	TTree *absorb = (TTree*)file->Get("HERO_absorber");
	if (!absorb) {
		cerr << "Can't find absorb tree!" << endl;
		return;
	}

	absorb->SetBranchAddress("eventId", &event_id);
	absorb->SetBranchAddress("pdg", &pdg);
	absorb->SetBranchAddress("trackID", &track_id);
	absorb->SetBranchAddress("deposit_E", &edep);

	TTree *ultra = (TTree*)file->Get("HERO_outside");
	if (!ultra) {
		cerr << "Can't find outside tree!" << endl;
		return;
	}
	
	ultra->SetBranchAddress("eventId", &event_id);
	ultra->SetBranchAddress("pdg", &pdg);
	ultra->SetBranchAddress("trackID", &track_id);
	ultra->SetBranchAddress("deposit_E", &edep);


	std::unordered_set<int> out_event_id_agg;
	std::map<int, Double_t> sum_edep_scint_agg;
	std::map<int, Double_t> sum_edep_absorb_agg;
	std::map<int, Double_t> sum_edep_ultra_agg;
	std::map<int, Double_t> sum_edep_agg;
	std::map<int, std::unordered_set<int>> n_born_neutrons_agg;
	std::map<int, std::unordered_set<int>> n_dead_neutrons_agg;
	std::map<int, Double_t> sum_edep_e_scint_agg;
	std::map<int, Double_t> sum_edep_e_absorb_agg;
	std::map<int, Double_t> sum_edep_e_ultra_agg;
	std::map<int, Double_t> sum_edep_e_agg;
	std::map<int, std::unordered_set<int>> n_born_alpha_agg;
	std::map<int, std::unordered_set<int>> n_dead_alpha_agg;
	std::map<int, Double_t> sum_edep_alpha_scint_agg;
	std::map<int, Double_t> sum_edep_alpha_absorb_agg;
	std::map<int, Double_t> sum_edep_alpha_ultra_agg;
	std::map<int, Double_t> sum_edep_alpha_agg;

	// fill aggs from scint
	cout << "Filling from scint!" << endl;
	for (UInt_t i=0; i<scint->GetEntries(); i++) {
		scint->GetEntry(i);

		out_event_id_agg.insert(event_id);

		if (sum_edep_agg.count(event_id)) {
			sum_edep_agg[event_id] += edep;
		}
		else {
			sum_edep_agg[event_id] = edep;
		}

		if (sum_edep_scint_agg.count(event_id)) {
			sum_edep_scint_agg[event_id] += edep;
		}
		else {
			sum_edep_scint_agg[event_id] = edep;
		}

		if (pdg == 2112) {
			n_born_neutrons_agg[event_id].insert(track_id);	
		}

		if (sum_edep_e_scint_agg.count(event_id)) {
			sum_edep_e_scint_agg[event_id] += edep;
		}
		else {
			sum_edep_e_scint_agg[event_id] = edep;
		}

		if (pdg == 11) {
			if (sum_edep_e_agg.count(event_id)) {
				sum_edep_e_agg[event_id] += edep;
			}
			else {
				sum_edep_e_agg[event_id] = edep;
			}
		}

		if (pdg == 1000020040) {
			n_born_alpha_agg[event_id].insert(track_id);

			if (sum_edep_alpha_scint_agg.count(event_id)) {
				sum_edep_alpha_scint_agg[event_id] += edep;
			}
			else {
				sum_edep_alpha_scint_agg[event_id] = edep;
			}

			if (sum_edep_alpha_agg.count(event_id)) {
				sum_edep_alpha_agg[event_id] += edep;
			}
			else {
				sum_edep_alpha_agg[event_id] = edep;
			}
		}

	}

	// fill aggs from absorb
	cout << "Filling from absorb!" << endl;
	for (UInt_t i=0; i<absorb->GetEntries(); i++) {
		absorb->GetEntry(i);

		out_event_id_agg.insert(event_id);

		if (sum_edep_agg.count(event_id)) {
			sum_edep_agg[event_id] += edep;
		}
		else {
			sum_edep_agg[event_id] = edep;
		}
		if (sum_edep_absorb_agg.count(event_id)) {
			sum_edep_absorb_agg[event_id] += edep;
		}
		else {
			sum_edep_absorb_agg[event_id] = edep;
		}

		if (pdg == 2112) {
			n_born_neutrons_agg[event_id].insert(track_id);	
		}

		if (sum_edep_absorb_agg.count(event_id)) {
			sum_edep_absorb_agg[event_id] += edep;
		}
		else {
			sum_edep_absorb_agg[event_id] = edep;
		}

		if (pdg == 11) {
			if (sum_edep_e_agg.count(event_id)) {
				sum_edep_e_agg[event_id] += edep;
			}
			else {
				sum_edep_e_agg[event_id] = edep;
			}
		}

		if (pdg == 1000020040) {
			n_born_alpha_agg[event_id].insert(track_id);

			if (sum_edep_alpha_absorb_agg.count(event_id)) {
				sum_edep_alpha_absorb_agg[event_id] += edep;
			}
			else {
				sum_edep_alpha_absorb_agg[event_id] = edep;
			}

			if (sum_edep_alpha_agg.count(event_id)) {
				sum_edep_alpha_agg[event_id] += edep;
			}
			else {
				sum_edep_alpha_agg[event_id] = edep;
			}
		}
	}

	// fill from ultra
	cout << "Filling from ultra!" << endl;
	for (UInt_t i=0; i<ultra->GetEntries(); i++) {
		ultra->GetEntry(i);

		out_event_id_agg.insert(event_id);

		if (sum_edep_agg.count(event_id)) {
			sum_edep_agg[event_id] += edep;
		}
		else {
			sum_edep_agg[event_id] = edep;
		}
		if (sum_edep_ultra_agg.count(event_id)) {
			sum_edep_ultra_agg[event_id] += edep;
		}
		else {
			sum_edep_ultra_agg[event_id] = edep;
		}

		if (pdg == 2112) {
			n_dead_neutrons_agg[event_id].insert(track_id);
		}

		if (pdg == 11) {
			if (sum_edep_e_ultra_agg.count(event_id)) {
				sum_edep_e_ultra_agg[event_id] += edep;
			}
			else {
				sum_edep_e_ultra_agg[event_id] = edep;
			}
		}

		if (pdg == 1000020040) {
			n_dead_alpha_agg[event_id].insert(track_id);

			if (sum_edep_alpha_ultra_agg.count(event_id)) {
				sum_edep_alpha_ultra_agg[event_id] += edep;
			}
			else {
				sum_edep_alpha_ultra_agg[event_id] = edep;
			}
		}
	}

	//file->Close();

	out_file->cd();
	for (const auto& e_id : out_event_id_agg) {
        out_event_id = e_id;
		sum_edep_scint = sum_edep_scint_agg[e_id];
		sum_edep_absorb = sum_edep_absorb_agg[e_id];
		sum_edep_ultra = sum_edep_ultra_agg[e_id];
		sum_edep = sum_edep_agg[e_id];

		n_born_neutrons = n_born_neutrons_agg[e_id].size();
		n_dead_neutrons = n_dead_neutrons_agg[e_id].size();
		ratio_neutrons = (n_born_neutrons - n_dead_neutrons) / n_born_neutrons;

		sum_edep_e_scint = sum_edep_e_scint_agg[e_id];
		sum_edep_e_absorb = sum_edep_absorb_agg[e_id];
		sum_edep_e_ultra = sum_edep_ultra_agg[e_id];
		sum_edep_e = sum_edep_agg[e_id];

		n_born_alpha = n_born_alpha_agg[e_id].size();
		n_dead_alpha = n_dead_alpha_agg[e_id].size();
		sum_edep_alpha_scint = sum_edep_alpha_scint_agg[e_id];
		sum_edep_alpha_absorb = sum_edep_alpha_absorb_agg[e_id];
		sum_edep_alpha_ultra = sum_edep_alpha_ultra_agg[e_id];
		sum_edep_alpha = sum_edep_alpha_agg[e_id];

		tree->Fill();
	}
	
	tree->Write();
	out_file->Close();
	file->Close();
}
