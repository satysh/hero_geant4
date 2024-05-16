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

TTree *tree(TString file_name)
{
	TFile *file = new TFile("tmp_output/" + file_name, "READ");

	return (TTree*)file->Get("params");
}

void sim_params_calculation_agg()
{
	auto files_list = get_root_files_list("tmp_output");
	TList *list = new TList;
	for (UInt_t i=0; i<files_list.size(); i++) {
		list->Add(tree(files_list[i]));
	}

	TFile *out_file = new TFile("params_result.root", "RECREATE");
	out_file->cd();
	TTree *tree = TTree::MergeTrees(list);
	tree->SetName("params_result");
	tree->Write();
}