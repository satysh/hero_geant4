TH1F *Hist(TString file_path, TString opt);
void e_resolution()
{	
	Int_t n_points = 5;
	Int_t e_arr[] = {6, 12, 25, 50, 100};

	Double_t *proton_boron_resolution_edep = new Double_t[n_points];
	Double_t *proton_boron_resolution_edep_errors = new Double_t[n_points];
	Double_t *proton_energies = new Double_t[n_points];

	Double_t *proton_noboron_resolution_edep = new Double_t[n_points];
	Double_t *proton_noboron_resolution_edep_errors = new Double_t[n_points];

	for (Int_t i=0; i<n_points; i++) {
		proton_energies[i] = Double_t(e_arr[i]);
// +b ------------------------------------------------------------------------------------------------------------------
		// path +b
		TString path;
		path.Form("electron/hero_nevents_1000_pdg_11_R_125_E_%d_bron_b_percent_5.root", e_arr[i] * 1000);

		TH1F *h = Hist(path, "edep");
		if (!h) { return; }

		TString canv_name;
		canv_name.Form("+b edep Energy %d", i * 1000);
		TCanvas *c = new TCanvas(canv_name, canv_name);
		c->cd();
		h->Draw();
		

		Double_t now_mean = h->GetMean();
		Double_t now_mean_error = h->GetMeanError();
		Double_t now_std = h->GetStdDev();
		//Double_t now_std_error = h->GetStdDevError();

		Double_t now_resolution = now_std / now_mean;
		Double_t now_resolution_error = TMath::Sqrt(1. + 2. * TMath::Power(0.01 * now_resolution, 2)) * now_resolution / 
		                                TMath::Sqrt(2. * h->GetEntries());

		//cout << i << ", " << now_resolution << ", " << now_resolution_error << endl;
		proton_boron_resolution_edep[i] = now_mean * 0.001;
		proton_boron_resolution_edep_errors[i] = now_mean_error * 0.001;
// -------------------------------------------------------------------------------------------------------------------------------------
// -b ----------------------------------------------------------------------------------------------------------------------------------
		// path -b
		path.Form("electron/hero_nevents_1000_pdg_11_R_125_E_%d_bron_-b_percent_5.root", e_arr[i] * 1000);

		h = Hist(path, "edep");
		if (!h) { return; }

		now_mean = h->GetMean();
		now_mean_error = h->GetMeanError();
		now_std = h->GetStdDev();
		now_resolution = now_std / now_mean;
		now_resolution_error = TMath::Sqrt(1. + 2. * TMath::Power(0.01 * now_resolution, 2)) * now_resolution / 
		                                TMath::Sqrt(2. * h->GetEntries());

		cout << i << ", " << now_resolution << ", " << now_resolution_error << endl;

		proton_noboron_resolution_edep[i] = now_mean * 0.001;
		proton_noboron_resolution_edep_errors[i] = now_mean_error * 0.001;
// -----------------------------------------------------------------------------------------------------------------------------------
	}
	
	TGraphErrors *p_b_edep_gr = new TGraphErrors(n_points,
												 proton_energies,
		                                         proton_boron_resolution_edep,
		                                         NULL,
		                                         proton_boron_resolution_edep_errors);

	TGraphErrors *p_nob_edep_gr = new TGraphErrors(n_points,
												   proton_energies,
		                                           proton_noboron_resolution_edep,
		                                           NULL,
		                                           proton_noboron_resolution_edep_errors);

	TF1 *p_b_edep_f = new TF1("p_b_edep_f", "[2] * x^2 + [1] * x + [0]");
	//p_b_edep_gr->Fit(p_b_edep_f);
	p_b_edep_gr->SetLineWidth(7);
	p_b_edep_gr->SetLineColor(kBlack);
	p_b_edep_gr->SetMarkerStyle(20);
	p_b_edep_gr->SetMarkerSize(2);
	p_b_edep_gr->SetMarkerColor(kBlack);

	TF1 *p_nob_edep_f = new TF1("p_nob_edep_f", "[2] * x^2 + [1] * x + [0]");
	//p_nob_edep_gr->Fit(p_nob_edep_f);
	p_nob_edep_f->SetLineColor(kBlack);
	p_nob_edep_gr->SetLineWidth(4);
	p_nob_edep_gr->SetLineStyle(9);
	p_nob_edep_gr->SetLineColor(kBlack);
	p_nob_edep_gr->SetMarkerStyle(21);
	p_nob_edep_gr->SetMarkerSize(2);
	p_nob_edep_gr->SetMarkerColor(kBlack);

	TMultiGraph *mg = new TMultiGraph();
	mg->Add(p_b_edep_gr);
	mg->Add(p_nob_edep_gr);
	mg->GetXaxis()->SetTitle("E [GeV]");
	mg->GetYaxis()->SetTitle("E_means [GeV]");

	TCanvas *c = new TCanvas("resolution", "resolution");
	c->cd();
	mg->Draw("ALP");

	auto legend = new TLegend(0.1, 0.77, 0.5, 0.9);
	legend->AddEntry(p_b_edep_gr, "scint+boron");
	legend->AddEntry(p_nob_edep_gr, "scint");
	legend->Draw();

	gPad->SetGrid(2, 2);
}

TH1F *Hist(TString file_path, TString opt)
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
	Double_t edep;
	tree->SetBranchAddress("nopticalphotons", &nopticalphotons);
	tree->SetBranchAddress("edep", &edep);

	gROOT->cd();
	TH1F *result = new TH1F(opt + "_" + file_path, opt + "_" + file_path, 100, 1., 0.);
	for (Int_t i=0; i<tree->GetEntries(); i++) {
		tree->GetEntry(i);
		if (opt == "nopticalphotons")
			result->Fill(nopticalphotons);
		else if (opt == "edep")
			result->Fill(edep);
		else {
			cerr << "Unknown option " << opt << "!" << endl;
			return NULL;
		}
	}

	delete tree;
	file->Close();

	return result;
}