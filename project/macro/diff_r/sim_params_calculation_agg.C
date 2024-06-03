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

void sim_params_calculation_agg()
{
	auto files_list = get_root_files_list("tmp_output");
	TList *list = new TList;
	TChain *chain = new TChain("params");
	for (UInt_t i=0; i<files_list.size(); i++) {
		chain->Add("tmp_output/" + files_list[i]);
	}

	//chain->Draw("sum_edep_scint");
	
	TFile *out_file = new TFile("params_result.root", "RECREATE");
	out_file->cd();
	chain->Write();
}