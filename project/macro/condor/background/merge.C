void merge(Int_t usec=16, TString bopt="b", Int_t nevents=469318, Int_t R=125)
{
	TString dir;
	dir.Form("%d_usec", usec);
	void *dirp = gSystem->OpenDirectory(dir);
	Char_t *afile;

	std::vector<TString> vfile_names;
	while ((afile = const_cast<Char_t *>(gSystem->GetDirEntry(dirp)))) {
		TString safile(afile);
		TString tmp;
		tmp.Form("bron_%s_percent_5_absorber.root", bopt.Data());
		if (safile.Contains(tmp)) {
			vfile_names.push_back(safile);
		}
	}

	TList *listedep_global_time = new TList;
	
	for (UInt_t i=0; i<vfile_names.size(); i++) {
		cout << vfile_names[i] << endl;

		auto f = new TFile(dir + "/" + vfile_names[i], "READ");
		gROOT->cd();
		auto edep_global_time = ((TTree*)f->Get("edep_global_time"))->CloneTree();
		listedep_global_time->Add(edep_global_time);
		f->Close();
	}
	
	TString outFileName;
	outFileName.Form("hero_nevents_%d_pdg_2212_R_%d_background_1000000_bron_%s_percent_5_absorber.root", nevents, R, bopt.Data());
	auto fout = new TFile(outFileName, "RECREATE");
	auto medep_global_time = TTree::MergeTrees(listedep_global_time);

	fout->cd();	
	medep_global_time->Write();
}