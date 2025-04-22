TH1F *Hist(TString file_path, TString opt);
TH1F *OldHist(TString file_path);
void p_resolution()
{	
	const Double_t student = 1.96;
	Int_t n_points = 12;
	Double_t proton_energies[] = {0.25, 0.5, 0.75, 1, 2.5, 5, 7.5, 10, 25, 50, 75, 100}; // TeV

	Double_t *proton_boron_resolution_edep = new Double_t[n_points];
	Double_t *proton_boron_resolution_edep_errors = new Double_t[n_points];

	Double_t *proton_noboron_resolution_edep = new Double_t[n_points];
	Double_t *proton_noboron_resolution_edep_errors = new Double_t[n_points];

	for (Int_t i=0; i<n_points; i++) {
// +b ------------------------------------------------------------------------------------------------------------------
		// path +b
		TString path;
		path.Form("proton/hero_nevents_1000_pdg_2212_R_125_E_%d_bron_b_percent_5_absorber.root", 
			       Int_t((proton_energies[i] * 1000000)));

		TH1F *h = Hist(path, "edep");
		if (!h) { 
			cerr << "Can't find file from path: " << path << endl;
            path.Form("../diff_r/output/%d_GeV_params_result.root", Int_t(proton_energies[i] * 1000));
			cerr << "Try to find file from path: " << path << endl;

			h = OldHist(path);

			if (!h) { return; }
		}

		TString canv_name;
		canv_name.Form("+b edep Energy %d", i * 1000);
		//TCanvas *c = new TCanvas(canv_name, canv_name);
		//c->cd();
		//h->Draw();
		

		Double_t now_mean = h->GetMean();
		//Double_t now_mean_error = h->GetMeanError();
		Double_t now_std = h->GetStdDev();
		//Double_t now_std_error = h->GetStdDevError();

		Double_t now_resolution = now_std / now_mean;
		/*Double_t now_resolution_error = TMath::Sqrt(1. + 2. * TMath::Power(0.01 * now_resolution, 2)) * now_resolution / 
		                                TMath::Sqrt(2. * h->GetEntries());*/
		Double_t now_resolution_error = student * now_resolution / TMath::Sqrt(2. * h->GetEntries());

		//cout << i << ", " << now_resolution << ", " << now_resolution_error << endl;
		proton_boron_resolution_edep[i] = now_resolution;
		proton_boron_resolution_edep_errors[i] = now_resolution_error;
// -------------------------------------------------------------------------------------------------------------------------------------
// -b ----------------------------------------------------------------------------------------------------------------------------------
		// path -b
		path.Form("proton/hero_nevents_1000_pdg_2212_R_125_E_%d_bron_-b_percent_5_absorber.root", 
			       Int_t((proton_energies[i]) * 1000000));

		h = Hist(path, "edep");
		if (!h) { 
			cerr << "Can't find file from path: " << path << endl;

			path.Form("../diff_r/output/wb_%d_GeV_params_result.root", Int_t(proton_energies[i] * 1000));
			cerr << "Try to find file from path: " << path << endl;

			if (!h) { 
				proton_noboron_resolution_edep[i] = 0.;
				proton_noboron_resolution_edep_errors[i] = 0.;
				continue; 
			}
		}

		now_mean = h->GetMean();
		now_std = h->GetStdDev();
		now_resolution = now_std / now_mean;
		/*now_resolution_error = TMath::Sqrt(1. + 2. * TMath::Power(0.01 * now_resolution, 2)) * now_resolution / 
		                                TMath::Sqrt(2. * h->GetEntries());*/
		now_resolution_error = student * now_resolution / TMath::Sqrt(2. * h->GetEntries());

		cout << i << ", " << now_resolution << ", " << now_resolution_error << endl;

		proton_noboron_resolution_edep[i] = now_resolution;
		proton_noboron_resolution_edep_errors[i] = now_resolution_error;
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
	p_b_edep_gr->SetLineWidth(4);
	p_b_edep_gr->SetLineColor(kBlack);
	p_b_edep_gr->SetMarkerStyle(20);
	p_b_edep_gr->SetMarkerSize(2);
	p_b_edep_gr->SetMarkerColor(kBlack);

	TF1 *p_nob_edep_f = new TF1("p_nob_edep_f", "[2] * x^2 + [1] * x + [0]");
	p_nob_edep_f->SetLineColor(kBlack);
	//p_nob_edep_gr->Fit(p_nob_edep_f);
	p_nob_edep_gr->SetLineWidth(5);
	p_nob_edep_gr->SetLineStyle(9);
	p_nob_edep_gr->SetLineColor(kBlack);
	p_nob_edep_gr->SetMarkerStyle(21);
	p_nob_edep_gr->SetMarkerSize(2);
	p_nob_edep_gr->SetMarkerColor(kBlack);

	TMultiGraph *mg = new TMultiGraph();
	mg->Add(p_b_edep_gr);
	mg->Add(p_nob_edep_gr);
	mg->GetXaxis()->SetTitle("Energy [TeV]");
	mg->GetYaxis()->SetTitle("resolution");

	TCanvas *c = new TCanvas("resolution", "resolution");
	c->cd();
	mg->Draw("ALP");

	auto legend = new TLegend(0.5, 0.65, 0.9, 0.9);
	legend->AddEntry(p_b_edep_gr, "scint+boron");
	legend->AddEntry(p_nob_edep_gr, "scint");
	legend->Draw();

	gPad->SetGrid(2, 2);
	gPad->SetLogx(1);
	//mg->GetYaxis()->SetRangeUser(0., 0.3);
	//mg->GetXaxis()->SetLimits(1., 101000.);

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

TH1F *OldHist(TString file_path)
{
	TFile *f = new TFile(file_path, "READ");
	if (f->IsZombie()) {
		cerr << "Can't read file " << file_path << endl;
		return NULL; 
	}

	TTree *t = (TTree*)f->Get("params_result");
	if (!t) {
		cerr << "Can't find tree " << file_path << endl;
		return NULL;
	}

	gROOT->cd();
	TH1F *h = new TH1F(file_path, file_path, 100, 1., 0.);

	Double_t edep;
	t->SetBranchAddress("sum_edep_scint", &edep);

	for (Int_t i=0; i<t->GetEntries(); i++) {
		t->GetEntry(i);
		h->Fill(edep);
	} 

	return h;
}