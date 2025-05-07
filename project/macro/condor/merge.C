void merge()
{
	TString dir = "1_gev";
	void *dirp = gSystem->OpenDirectory(dir);
	Char_t *afile;

	std::vector<TString> vfile_names;
	while ((afile = const_cast<Char_t *>(gSystem->GetDirEntry(dirp)))) {
		TString safile(afile);
		if (safile.Contains("bron_b_percent_5_absorber.root")) {
			vfile_names.push_back(safile);
		}
	}

	TList *listHERO = new TList;
	TList *listAHERO = new TList;
	TList *listPRIMARY = new TList;
	TList *listoptic_global_time = new TList;
	TList *listedep_global_time = new TList;
	TList *listparticles_agg_edep = new TList;
	TList *listdigi = new TList;
	
	for (UInt_t i=0; i<vfile_names.size(); i++) {
		cout << vfile_names[i] << endl;

		auto f = new TFile(dir + "/" + vfile_names[i], "READ");
		gROOT->cd();
		auto HERO = ((TTree*)f->Get("HERO"))->CloneTree();
		listHERO->Add(HERO);
		gROOT->cd();
		auto AHERO = ((TTree*)f->Get("AHERO"))->CloneTree();
		listAHERO->Add(AHERO);
		gROOT->cd();
		auto PRIMARY = ((TTree*)f->Get("PRIMARY"))->CloneTree();
		listPRIMARY->Add(PRIMARY);
		gROOT->cd();
		auto optic_global_time = ((TTree*)f->Get("optic_global_time"))->CloneTree();
		listoptic_global_time->Add(optic_global_time);
		gROOT->cd();
		auto edep_global_time = ((TTree*)f->Get("edep_global_time"))->CloneTree();
		listedep_global_time->Add(edep_global_time);
		gROOT->cd();
		auto particles_agg_edep = ((TTree*)f->Get("particles_agg_edep"))->CloneTree();
		listparticles_agg_edep->Add(particles_agg_edep);
		gROOT->cd();
		auto digi = ((TTree*)f->Get("digi"))->CloneTree();
		listdigi->Add(digi);

		f->Close();
	}
	
	auto fout = new TFile("merged.root", "RECREATE");
	auto mHERO = TTree::MergeTrees(listHERO);
	auto mAHERO = TTree::MergeTrees(listAHERO);
	auto mPRIMARY = TTree::MergeTrees(listPRIMARY);
	auto moptic_global_time = TTree::MergeTrees(listoptic_global_time);
	auto medep_global_time = TTree::MergeTrees(listedep_global_time);
	auto mparticles_agg_edep = TTree::MergeTrees(listparticles_agg_edep);
	auto mdigi = TTree::MergeTrees(listdigi);

	fout->cd();
	mHERO->Write();
	mAHERO->Write();
	mPRIMARY->Write();
	//moptic_global_time->Write();
	medep_global_time->Write();
	medep_global_time->Write();
	mparticles_agg_edep->Write();
	mdigi->Write();
}