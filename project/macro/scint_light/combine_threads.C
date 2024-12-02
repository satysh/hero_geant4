void combine_threads()
{
	void *dirp = gSystem->OpenDirectory("input");
	Char_t *afile;

	std::vector<TString> vfile_names;
	std::vector<TString> vfile_name_tmps;
	while ((afile = const_cast<Char_t *>(gSystem->GetDirEntry(dirp)))) {
		TString safile(afile);
		TRegexp tmp("hero_nevents_.*_pdg_.*_R_.*_E_.*_bron_b_percent_.*_t.*.root");
		if (safile.Contains(tmp)) {
			vfile_names.push_back(safile);

			// Remove t*.root postfix
			Int_t now_len = safile.Length();
			Int_t now_cut = 5;
			for (Int_t i=now_len; i>=0; i--) {
				now_cut++;
				if (TString(safile.Data()[i]) == "t") break;
			}
			safile = safile(0, now_len - now_cut);

			// mem file tmps
			Bool_t check = true;
			for (Int_t i=0; i<vfile_name_tmps.size(); i++) {
				if (vfile_name_tmps[i] == safile) {
					check = false;
					break;
				}
			}

			if (check)
				vfile_name_tmps.push_back(safile);
		}
	}

	for (Int_t i=0; i<vfile_name_tmps.size(); i++) {
		TString now_file_name_tmp = vfile_name_tmps[i];

		cout << i << " now_tmp is " << now_file_name_tmp << endl;
		Double_t edep;
		Int_t nopticalphotons;

		TTree *now_tree = new TTree("HERO", "Data");
		now_tree->Branch("edep", &edep, "edep/D");
		now_tree->Branch("nopticalphotons", &nopticalphotons, "nopticalphotons/I");

		for (Int_t j=0; j<vfile_names.size(); j++) {
			TString now_file_name = vfile_names[j];
			if (now_file_name.Contains(now_file_name_tmp)) {
				cout << j << " " << now_file_name << " is processing..." << endl;
				TFile *now_file = new TFile("input/" + now_file_name, "READ");
				if (now_file->IsZombie()) {
					cerr << "Can't read file " << now_file_name << endl;
					return;
				}

				TTree *files_tree = (TTree*)now_file->Get("HERO");
				files_tree->SetBranchAddress("edep", &edep);
				files_tree->SetBranchAddress("nopticalphotons", &nopticalphotons);

				for (Int_t k=0; k<files_tree->GetEntries(); k++) {
					files_tree->GetEntry(k);
					now_tree->Fill();
				}

				delete files_tree;
				now_file->Close();
				//gROOT->cd();
			}
		}

		Int_t  now_file_name_len = now_file_name_tmp.Length();
		TString out_file_name = now_file_name_tmp(0, now_file_name_len - 1) + ".root"; // - 1 to remove _
		cout << "---- Output ---\n";
		cout << out_file_name << endl;

		TFile *out_file = new TFile("output/" + out_file_name, "RECREATE");
		if (out_file->IsZombie()) {
			cerr << "Can't find output dir!" << endl;
			return;
		}
		out_file->cd();
		now_tree->Write();

		delete now_tree;
		out_file->Close();
	}

}