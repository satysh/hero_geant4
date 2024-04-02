void energy_save_check()
{
	TFile *file = new TFile("../../output/hero.root", "READ");
	if (file->IsZombie()) {
		cerr << "Can't read root file!" << endl;
		return;
	}

	TTree *tree_inside = (TTree*)file->Get("HERO");
	if (!tree_inside) {
		cerr << "Can't find tree!" << endl;
		return;
	}

	Double_t edep;
	Double_t ekin;
	tree_inside->SetBranchAddress("deposit_E", &edep);
	tree_inside->SetBranchAddress("kin_E", &ekin);


	TTree *tree_outside = (TTree*)file->Get("HERO_outside");
	if (!tree_outside) {
		cerr << "Can't find outside tree!" << endl;
		return;
	}

	tree_outside->SetBranchAddress("deposit_E", &edep);
	tree_outside->SetBranchAddress("kin_E", &ekin);

	Double_t edep_sum_inside = 0.;
	Double_t ekin_sum_inside = 0.;
	cout << "tree_inside entries is " << tree_inside->GetEntries() << endl;
	for (UInt_t i=0; i<tree_inside->GetEntries(); i++) {
		tree_inside->GetEntry(i);
		edep_sum_inside += edep;
		ekin_sum_inside += ekin;
	}

	cout << "edep_sum_inside = " << edep_sum_inside / 1000. << " GeV" << endl;
	cout << "ekin_sum_inside = " << ekin_sum_inside / 1000. << " GeV" << endl;

	Double_t edep_sum_outside = 0.;
	Double_t ekin_sum_outside = 0.;
	cout << "tree_outside entries is " << tree_outside->GetEntries() << endl;
	for (UInt_t i=0; i<tree_outside->GetEntries(); i++) {
		tree_outside->GetEntry(i);
		edep_sum_outside += edep;
		ekin_sum_outside += ekin;
	}

	cout << "edep_sum_outside = " << edep_sum_outside / 1000. << " GeV" << endl;
	cout << "ekin_sum_outside = " << ekin_sum_outside / 1000. << " GeV" << endl;

	Double_t e_sum = edep_sum_inside + edep_sum_outside + ekin_sum_outside;
	cout << "total e_sum is " << e_sum << ", MeV or " << e_sum / 1000. << " GeV" << endl;
}