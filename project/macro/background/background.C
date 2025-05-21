void background(Int_t binLen=16, TString path="", Int_t nevents=469318, TString bopt="b")
{
	TString fileName;
	fileName.Form("hero_nevents_%d_pdg_2212_R_125_background_1000000_bron_%s_percent_5_absorber.root", nevents, bopt.Data());
	auto f = new TFile(path + "/" + fileName, "READ");
	if (f->IsZombie()) {
		cerr << "Can't read file!" << endl;
		return;
	}

	auto t = (TTree*)f->Get("edep_global_time");
	if (!t) {
		cerr << "Can't find tree!" << endl;
		return;
	}

	fileName.Form("background_%d_%s.root", binLen, bopt.Data());
	auto out = new TFile(fileName, "RECREATE");
	TString name;
	name.Form("h_%d_usec", binLen);
	Int_t nbins = 1000000 / binLen;
	auto h = new TH1F(name, name, nbins, 0., 1000000.);

	Double_t edep, edept;
	t->SetBranchAddress("edep", &edep);
	t->SetBranchAddress("edept", &edept);

	for (UInt_t i=0; i<t->GetEntries(); i++) {
		t->GetEntry(i);
		h->Fill(edept, edep);
	}

	//h->Draw("HIST");
	h->Write();

	cout << binLen << " done!" << endl;
}