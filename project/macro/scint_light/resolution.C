TH1F *DrawOpticalPhotons(TString file_path);
void resolution()
{
	Int_t e_beg = 1;
	Int_t e_end = 7;

	Int_t n_points = e_end - e_beg + 1;

	Double_t *proton_boron_resolution = new Double_t[n_points];
	Double_t *proton_boron_resolution_errors = new Double_t[n_points];
	Double_t *proton_boron_energies = new Double_t[n_points];

	for (Int_t i=e_beg; i<=e_end; i++) {
		TString path;
		path.Form("output/hero_nevents_1000_pdg_2212_R_125_E_%d_bron_b_percent_5.root", i * 1000);
		TH1F *h = DrawOpticalPhotons(path);

		TString canv_name;
		canv_name.Form("Energy %d", i * 1000);
		TCanvas *c = new TCanvas(canv_name, canv_name);
		c->cd();

		h->Draw();

		Double_t now_mean = h->GetMean();
		//Double_t now_mean_error = h->GetMeanError();
		Double_t now_std = h->GetStdDev();
		//Double_t now_std_error = h->GetStdDevError();

		Double_t now_resolution = now_std / now_mean;
		Double_t now_resolution_error = TMath::Sqrt(1. + 2. * TMath::Power(0.01 * now_resolution, 2)) * now_resolution / 
		                                TMath::Sqrt(2. * h->GetEntries());

		cout << i * 1000 << ", " << now_resolution << ", " << now_resolution_error << endl;

		proton_boron_resolution[i-1] = now_resolution;
		proton_boron_resolution_errors[i-1] = now_resolution_error;
		proton_boron_energies[i-1] = (Double_t)i * 1000.;
	}
	
	TMultiGraph *mg = new TMultiGraph();
	TGraphErrors *p_b_gr = new TGraphErrors(n_points,
																					proton_boron_energies,
		                                      proton_boron_resolution,
		                                      NULL,
		                                      proton_boron_resolution_errors);
	p_b_gr->SetLineWidth(4);
	p_b_gr->SetLineColor(kRed);
	p_b_gr->SetMarkerStyle(20);
	p_b_gr->SetMarkerSize(2);

	mg->Add(p_b_gr);
	mg->GetXaxis()->SetTitle("Energy [GeV]");
	mg->GetYaxis()->SetTitle("resolution");

	TCanvas *c = new TCanvas("resolution", "resolution");
	c->cd();
	mg->Draw("ACP");

	auto legend = new TLegend(0.1, 0.77, 0.5, 0.9);
	legend->AddEntry(p_b_gr, "proton, det+boron");
	legend->Draw();

	gPad->SetGrid(2, 2);
}

TH1F *DrawOpticalPhotons(TString file_path)
{
	TFile *file = new TFile(file_path, "READ");
	if (file->IsZombie()) {
		cerr << "Can't read the file " << file_path << endl;
		return NULL; 
	}

	auto tree = (TTree*)file->Get("HERO");
	if (!tree) {
		cerr << "Can't find HERO tree in the file " << file_path << endl;
		return NULL;
	}

	Int_t nopticalphotons;
	tree->SetBranchAddress("nopticalphotons", &nopticalphotons);


	gROOT->cd();
	TH1F *result = new TH1F("h_" + file_path, "h_" + file_path, 100, 1., 0.);
	for (Int_t i=0; i<tree->GetEntries(); i++) {
		tree->GetEntry(i);
		result->Fill(nopticalphotons);
	}

	delete tree;
	file->Close();

	return result;
}