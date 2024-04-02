void test_unique_pdg_for_ev_id_and_track_id_pair()
{
	TFile *file = new TFile("../energy_source/input/only_boron_geo_3_percent_of_boron_and_r125cm_3804_ev_2010.root", "READ");
	if (file->IsZombie()) {
		cerr << "Can't read root file!" << endl;
		return;
	}

	TTree *tree = (TTree*)file->Get("HERO");
	if (!tree) {
		cerr << "Can't find tree!" << endl;
		return;
	}

	Int_t event_id;
	Int_t track_id;
	Int_t pdg;
	tree->SetBranchAddress("eventId", &event_id);
	tree->SetBranchAddress("trackID", &track_id);
	tree->SetBranchAddress("pdg", &pdg);

	std::map<int, std::map<int, std::unordered_set<int>>> event;
	for (UInt_t i=0; i<tree->GetEntries(); i++) {
		tree->GetEntry(i);
		event[event_id][track_id].insert(pdg);
		//cout << event_id << ", " << track_id << ", " << pdg << endl;
	}

	for (const auto& [ev_id, track_map] : event) {
		for (const auto& [t_id, pdg_set] : track_map) {
			if (pdg_set.size() > 1) {
				cout << "for event " << ev_id << ", track_id " << t_id << ", pdg is not unique!" << endl;
				return; 
			}
		}

	}

	cout << "-------------- DONE ----------------------" << endl;
}