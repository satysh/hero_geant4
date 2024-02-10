void Print(TString fileName="");

void energy_release(TString fileName="")
{
	ifstream input(fileName.Data());
	if (!input.is_open()) {
		cerr << "Can't find file" + fileName << endl;
		return;
	}
	
	while (!input.eof()) {
		TString nowFileName;
		input >> nowFileName;
		if (nowFileName.Contains(".root")) {
			Print(nowFileName);
		}
	}
}

void Print(TString fileName="") {
	TFile *file = new TFile("input/" + fileName, "READ");
	if (file->IsZombie()) {
		cerr << "Can't read input file!" << endl;
		return;
	}

	TTree *tree = (TTree*)file->Get("HERO");
	if (!tree) {
		cerr << "Can't find tree!" << endl;
		return;
	}

	UInt_t nEntries = tree->GetEntries();
	//cout << "nEntries = " << nEntries << endl;

	Int_t pdg;
	Double_t deposit_E;
	Double_t kin_E;

	tree->SetBranchAddress("pdg", &pdg);
	tree->SetBranchAddress("deposit_E", &deposit_E);
	tree->SetBranchAddress("kin_E", &kin_E);

	Double_t full_edep = 0.;
	Double_t full_edep_alpha = 0.;
	for (UInt_t i=0; i<nEntries; i++) {
		tree->GetEntry(i);
		cout << Int_t(100. * Double_t(i) / Double_t(nEntries)) << " %" << endl;
		full_edep += deposit_E;

		if (pdg == 1000020040) {
			full_edep_alpha += deposit_E;
		}
	}

	cerr << "------------------------------------------------------------------------" << endl;
	cerr << fileName << endl;
	cerr << "full_edep = " << full_edep << endl;
	cerr << "full_edep_alpha = " << full_edep_alpha << endl;
	cerr << "------------------------------------------------------------------------" << endl;
	file->Close();
}