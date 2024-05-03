void sim_params_calculation_agg()
{
	TFile *file = new TFile("params.root", "READ");
	if (file->IsZombie()) {
		cerr << "Can't find params.root" << endl;
		return;
	}

	auto keys = file->GetListOfKeys();
	TIter iter(keys);
	TList *list = new TList;
	for (UInt_t i=0; i<keys->GetSize(); i++) {
		TString now_tree_name(iter.Next()->GetName());
		cout << now_tree_name << endl;
		list->Add((TTree*)file->Get(now_tree_name));
	}

	TFile *out_file = new TFile("params_result.root", "RECREATE");
	out_file->cd();
	TTree *tree = TTree::MergeTrees(list);
	tree->SetName("params_result");
	tree->Write();
}