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

void FillTree(TString file_name);

void sim_params_calculation(Int_t seed=1)
{
	auto files_list = get_root_files_list("../../output");
	TString file_name = "";
	TString tmp;
	tmp.Form("seed_%d_fixed_E_", seed + 1);
	for (UInt_t i=0; i<files_list.size(); i++) {
		if (files_list[i].Contains(tmp)) {
			file_name = files_list[i];
			break;
		}
	}
	
	FillTree(file_name);
}

typedef std::map<int, Double_t> HEROAggMap;

void FillAgg(HEROAggMap *m_ap, Int_t event_id, Double_t edep)
{
	if ((*m_ap).count(event_id)) {
		(*m_ap)[event_id] += edep;
	}
	else {
		(*m_ap)[event_id] = edep;
	}
}

Double_t PullValue(HEROAggMap *m_ap, Int_t event_id)
{
	if ((*m_ap).count(event_id)) {
		return (*m_ap)[event_id];
	}

	return 0.;
}

// без индексов только имя файла для обработки 
void FillTree(TString file_name)
{
	cout << "Filling from file " << file_name << "!" << endl;

	// формируем имя выходного файлы на основе имени входного, он называется точно так же, но создается в директории ./tmp_output
	TFile *out_file = new TFile("tmp_output/" + file_name, "RECREATE");
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

	TString tree_name = "params";
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
	

	TFile *file = new TFile("../../output/" + file_name, "READ");
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
	HEROAggMap sum_edep_scint_agg;
	HEROAggMap sum_edep_absorb_agg;
	HEROAggMap sum_edep_ultra_agg;
	HEROAggMap sum_edep_agg;
	std::map<int, std::unordered_set<int>> n_born_neutrons_agg;
	std::map<int, std::unordered_set<int>> n_dead_neutrons_agg;
	HEROAggMap sum_edep_e_scint_agg;
	HEROAggMap sum_edep_e_absorb_agg;
	HEROAggMap sum_edep_e_ultra_agg;
	HEROAggMap sum_edep_e_agg;
	std::map<int, std::unordered_set<int>> n_born_alpha_agg;
	std::map<int, std::unordered_set<int>> n_dead_alpha_agg;
	HEROAggMap sum_edep_alpha_scint_agg;
	HEROAggMap sum_edep_alpha_absorb_agg;
	HEROAggMap sum_edep_alpha_ultra_agg;
	HEROAggMap sum_edep_alpha_agg;

	// fill aggs from scint
	cout << "Filling from scint!" << endl;
	for (UInt_t i=0; i<scint->GetEntries(); i++) {
		scint->GetEntry(i);

		out_event_id_agg.insert(event_id);

		FillAgg(&sum_edep_scint_agg, event_id, edep);

		FillAgg(&sum_edep_agg, event_id, edep);

		if (pdg == 2112) {
			n_born_neutrons_agg[event_id].insert(track_id);	
		}

		if (pdg == 11) {
			FillAgg(&sum_edep_e_scint_agg, event_id, edep);

			FillAgg(&sum_edep_e_agg, event_id, edep);
		}

		if (pdg == 1000020040) {
			n_born_alpha_agg[event_id].insert(track_id);

			FillAgg(&sum_edep_alpha_scint_agg, event_id, edep);

			FillAgg(&sum_edep_alpha_agg, event_id, edep);
		}

	}

	// fill aggs from absorb
	cout << "Filling from absorb!" << endl;
	for (UInt_t i=0; i<absorb->GetEntries(); i++) {
		absorb->GetEntry(i);

		out_event_id_agg.insert(event_id);

		FillAgg(&sum_edep_absorb_agg, event_id, edep);

		FillAgg(&sum_edep_agg, event_id, edep);

		if (pdg == 2112) {
			n_born_neutrons_agg[event_id].insert(track_id);	
		}

		if (pdg == 11) {
			FillAgg(&sum_edep_e_absorb_agg, event_id, edep);

			FillAgg(&sum_edep_e_agg, event_id, edep);
		}

		if (pdg == 1000020040) {
			n_born_alpha_agg[event_id].insert(track_id);

			FillAgg(&sum_edep_alpha_absorb_agg, event_id, edep);

			FillAgg(&sum_edep_alpha_agg, event_id, edep);
		}
	}

	// fill from ultra
	cout << "Filling from ultra!" << endl;
	for (UInt_t i=0; i<ultra->GetEntries(); i++) {
		ultra->GetEntry(i);

		out_event_id_agg.insert(event_id);

		FillAgg(&sum_edep_ultra_agg, event_id, edep);

		if (pdg == 2112) {
			n_dead_neutrons_agg[event_id].insert(track_id);
		}

		if (pdg == 11) {
			FillAgg(&sum_edep_e_ultra_agg, event_id, edep);
		}

		if (pdg == 1000020040) {
			n_dead_alpha_agg[event_id].insert(track_id);

			FillAgg(&sum_edep_alpha_ultra_agg, event_id, edep);
		}
	}

	//file->Close();

	out_file->cd();
	for (const auto& e_id : out_event_id_agg) {
        out_event_id = e_id;
		sum_edep_scint = PullValue(&sum_edep_scint_agg, e_id);
		sum_edep_absorb = PullValue(&sum_edep_absorb_agg, e_id);
		sum_edep_ultra = PullValue(&sum_edep_ultra_agg, e_id);
		sum_edep = PullValue(&sum_edep_agg, e_id);

		n_born_neutrons = n_born_neutrons_agg[e_id].size();
		n_dead_neutrons = n_dead_neutrons_agg[e_id].size();
		if (n_born_neutrons != 0.)
			ratio_neutrons = Double_t(n_born_neutrons - n_dead_neutrons) / n_born_neutrons;
		else
			ratio_neutrons = 0.;
		sum_edep_e_scint = PullValue(&sum_edep_e_scint_agg, e_id);
		sum_edep_e_absorb = PullValue(&sum_edep_e_absorb_agg, e_id);
		sum_edep_e_ultra = PullValue(&sum_edep_e_ultra_agg, e_id);
		sum_edep_e = PullValue(&sum_edep_e_agg, e_id);

		n_born_alpha = n_born_alpha_agg[e_id].size();
		n_dead_alpha = n_dead_alpha_agg[e_id].size();
		sum_edep_alpha_scint = PullValue(&sum_edep_alpha_scint_agg, e_id);
		sum_edep_alpha_absorb = PullValue(&sum_edep_alpha_absorb_agg, e_id);
		sum_edep_alpha_ultra = PullValue(&sum_edep_alpha_ultra_agg, e_id);
		sum_edep_alpha = PullValue(&sum_edep_alpha_agg, e_id);

		tree->Fill();
	}
	
	tree->Write();
	out_file->Close();
	file->Close();
}
