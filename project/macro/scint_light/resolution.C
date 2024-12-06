TH1F *Hist(TString file_path, TString opt);
void resolution()
{
	Int_t e_beg = 1;
	Int_t e_end = 15;

	Int_t n_points = e_end - e_beg + 1;

	Double_t *proton_boron_resolution_optic = new Double_t[n_points];
	Double_t *proton_boron_resolution_optic_errors = new Double_t[n_points];
	Double_t *proton_energies = new Double_t[n_points];

	Double_t *proton_boron_resolution_edep = new Double_t[n_points];
	Double_t *proton_boron_resolution_edep_errors = new Double_t[n_points];


	Double_t *proton_noboron_resolution_optic = new Double_t[n_points];
	Double_t *proton_noboron_resolution_optic_errors = new Double_t[n_points];

	Double_t *proton_noboron_resolution_edep = new Double_t[n_points];
	Double_t *proton_noboron_resolution_edep_errors = new Double_t[n_points];

	for (Int_t i=e_beg; i<=e_end; i++) {

// +b ------------------------------------------------------------------------------------------------------------------
		// path +b
		TString path;
		path.Form("output/hero_nevents_1000_pdg_2212_R_125_E_%d_bron_b_percent_5.root", i * 1000);

		// optic resloution +b
		TH1F *h = Hist(path, "nopticalphotons");
		if (!h) { return; }

		/*
		TString canv_name;
		canv_name.Form("+b nopticalphotons Energy %d", i * 1000);
		TCanvas *c = new TCanvas(canv_name, canv_name);
		c->cd();
		h->Draw();
		*/

		Double_t now_mean = h->GetMean();
		//Double_t now_mean_error = h->GetMeanError();
		Double_t now_std = h->GetStdDev();
		//Double_t now_std_error = h->GetStdDevError();

		Double_t now_resolution = now_std / now_mean;
		Double_t now_resolution_error = TMath::Sqrt(1. + 2. * TMath::Power(0.01 * now_resolution, 2)) * now_resolution / 
		                                TMath::Sqrt(2. * h->GetEntries());

		//cout << i << ", " << now_resolution << ", " << now_resolution_error << endl;

		proton_boron_resolution_optic[i-1] = now_resolution;
		proton_boron_resolution_optic_errors[i-1] = now_resolution_error;
		proton_energies[i-1] = (Double_t)i * 1000.;
		// --------------------------------------------------------------------------------------------------------------------

		// edep resolution +b
		h = Hist(path, "edep");
		if (!h) { return; }
		/*
		canv_name.Form("+b edep Energy %d", i * 1000);
		c = new TCanvas(canv_name, canv_name);
		c->cd();
		h->Draw();
		*/

		now_mean = h->GetMean();
		now_std = h->GetStdDev();
		now_resolution = now_std / now_mean;
		now_resolution_error = TMath::Sqrt(1. + 2. * TMath::Power(0.01 * now_resolution, 2)) * now_resolution / 
		                                TMath::Sqrt(2. * h->GetEntries());

		//cout << i << ", " << now_resolution << ", " << now_resolution_error << endl;
		proton_boron_resolution_edep[i-1] = now_resolution;
		proton_boron_resolution_edep_errors[i-1] = now_resolution_error;
// -------------------------------------------------------------------------------------------------------------------------------------
// -b ----------------------------------------------------------------------------------------------------------------------------------
		// path -b
		path.Form("output/hero_nevents_1000_pdg_2212_R_125_E_%d_bron_-b_percent_5.root", i * 1000);

		// optic resolution
		h = Hist(path, "nopticalphotons");
		if (!h) { return; }

		now_mean = h->GetMean();
		now_std = h->GetStdDev();
		now_resolution = now_std / now_mean;
		now_resolution_error = TMath::Sqrt(1. + 2. * TMath::Power(0.01 * now_resolution, 2)) * now_resolution / 
		                                TMath::Sqrt(2. * h->GetEntries());

		//cout << i << ", " << now_resolution << ", " << now_resolution_error << endl;

		proton_noboron_resolution_optic[i-1] = now_resolution;
		proton_noboron_resolution_optic_errors[i-1] = now_resolution_error;



		// edep resolution
		h = Hist(path, "edep");
		if (!h) { return; }

		now_mean = h->GetMean();
		now_std = h->GetStdDev();
		now_resolution = now_std / now_mean;
		now_resolution_error = TMath::Sqrt(1. + 2. * TMath::Power(0.01 * now_resolution, 2)) * now_resolution / 
		                                TMath::Sqrt(2. * h->GetEntries());

		cout << i << ", " << now_resolution << ", " << now_resolution_error << endl;

		proton_noboron_resolution_edep[i-1] = now_resolution;
		proton_noboron_resolution_edep_errors[i-1] = now_resolution_error;
// -----------------------------------------------------------------------------------------------------------------------------------
	}
	
	TMultiGraph *mg = new TMultiGraph();
	TGraphErrors *p_b_optic_gr = new TGraphErrors(n_points,
												  proton_energies,
		                                          proton_boron_resolution_optic,
		                                          NULL,
		                                          proton_boron_resolution_optic_errors);

	TGraphErrors *p_b_edep_gr = new TGraphErrors(n_points,
												 proton_energies,
		                                         proton_boron_resolution_edep,
		                                         NULL,
		                                         proton_boron_resolution_edep_errors);

	TGraphErrors *p_nob_optic_gr = new TGraphErrors(n_points,
												    proton_energies,
		                                            proton_noboron_resolution_optic,
		                                            NULL,
		                                            proton_noboron_resolution_optic_errors);

	TGraphErrors *p_nob_edep_gr = new TGraphErrors(n_points,
												   proton_energies,
		                                           proton_noboron_resolution_edep,
		                                           NULL,
		                                           proton_noboron_resolution_edep_errors);
	p_b_optic_gr->SetLineWidth(4);
	p_b_optic_gr->SetLineColor(kRed);
	p_b_optic_gr->SetMarkerStyle(20);
	p_b_optic_gr->SetMarkerSize(2);
	p_b_optic_gr->SetMarkerColor(kRed);

	p_nob_optic_gr->SetLineWidth(4);
	p_nob_optic_gr->SetLineColor(kBlue);
	p_nob_optic_gr->SetMarkerStyle(20);
	p_nob_optic_gr->SetMarkerSize(2);
	p_nob_optic_gr->SetMarkerColor(kBlue);

	p_b_edep_gr->SetLineWidth(2);
	p_nob_edep_gr->SetLineWidth(2);
	p_nob_edep_gr->SetLineStyle(10);

	mg->Add(p_b_optic_gr);
	mg->Add(p_b_edep_gr);
	mg->Add(p_nob_optic_gr);
	mg->Add(p_nob_edep_gr);
	mg->GetXaxis()->SetTitle("Energy [GeV]");
	mg->GetYaxis()->SetTitle("resolution");

	TCanvas *c = new TCanvas("resolution", "resolution");
	c->cd();
	mg->Draw("ACP");

	auto legend = new TLegend(0.1, 0.77, 0.5, 0.9);
	legend->AddEntry(p_b_optic_gr, "proton, det+boron, optic");
	legend->AddEntry(p_b_edep_gr, "proton, det+boron, edep");
	legend->AddEntry(p_nob_optic_gr, "proton, det-boron, optic");
	legend->AddEntry(p_nob_edep_gr, "proton, det-boron, edep");
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