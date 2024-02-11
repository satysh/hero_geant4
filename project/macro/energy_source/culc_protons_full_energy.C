Double_t sum_protons_energy(TString fileName="");

void culc_protons_full_energy()
{
	cout << "protons energy for:" << endl;
	void *dirp = gSystem->OpenDirectory("input");
	Char_t *afile;

	while ((afile = const_cast<Char_t *>(gSystem->GetDirEntry(dirp)))) {
		TString safile(afile);
		if (safile.Contains(".txt") && !safile.Contains("full")) {
			cout << safile << ": " << sum_protons_energy(safile) << " GeV" << endl;
		}
	}

}

Double_t sum_protons_energy(TString fileName="") {
	ifstream input(("input/" + fileName).Data());
	if (!input.is_open()) {
		cerr << "Can't read file " << fileName << endl;
		return -1;
	}



	Double_t esum = 0.;
	while (!input.eof()) {
		Double_t now_e;
		input >> now_e;
		esum += now_e;
	}

	input.close();
	return esum;
}