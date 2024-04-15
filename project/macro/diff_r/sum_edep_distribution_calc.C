void Fill(TH1F *hist, TString fileName);

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

void sum_edep_distribution_calc()
{
	TString dirpath = "../input/fixed/100gev/";
	auto files_list = get_root_files_list(dirpath);
	
	Int_t binN = 100;
	TH1F *hist = new TH1F("hist", "hist", binN, 0., 100.);
	
	for (UInt_t i=0; i<files_list.size(); i++) {
		Fill(hist, dirpath + "/" + files_list[i]);
	}


	TFile *file = new TFile("sum_edep_distribution_calc.root", "RECREATE");
	hist->Write();
}

void Fill(TH1F *hist, TString fileName)
{
	TFile *file = new TFile(fileName, "READ");
	if (file->IsZombie()) {
		cerr << "Can't read root file!" << endl;
		return;
	}
	

	TTree *tree = (TTree*)file->Get("HERO");
	if (!tree) {
		cerr << "Can't find tree!" << endl;
		return;
	}

	cout << "Filling from file " << fileName << "!" << endl;

	Int_t event_id;
	Double_t edep;
	tree->SetBranchAddress("eventId", &event_id);
	tree->SetBranchAddress("deposit_E", &edep);

	std::map<int, Double_t> event;
	for (UInt_t i=0; i<tree->GetEntries(); i++) {
		tree->GetEntry(i);
		if (event.count(event_id)) {
			event[event_id] += edep;
		}
		else {
			event[event_id] = edep;
		}
	}


	for (const auto& [ev_id, sum_edep] : event) {
		Double_t now_sum = sum_edep * 0.001; // to GeV
		hist->Fill(now_sum); 
	}

	event.clear();
	file->Close();
}
