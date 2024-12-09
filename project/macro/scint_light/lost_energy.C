TH1F *Hist(TString file_path, TString opt);
void lost_energy()
{	
	Int_t n_points = 9;
	Int_t nevents_arr[] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 100, 100};
	Int_t e_arr[] = {1, 6, 12, 25, 50, 100, 1000, 10000, 100000};

	Double_t *lost_energy_arr = new Double_t[n_points];
	Double_t *lost_energy_error_arr = new Double_t[n_points];
	Double_t *proton_energies = new Double_t[n_points];
	for (Int_t i=0; i<n_points; i++) {

		proton_energies[i] = Double_t(e_arr[i]);
		TString path;
		path.Form("proton/hero_nevents_%d_pdg_2212_R_125_E_%d_bron_b_percent_5_absorber.root", nevents_arr[i], e_arr[i] * 1000);

		TH1F *h = Hist(path, "HERO");
		Double_t scint_mean = h->GetMean();
		Double_t scint_mean_error = h->GetMeanError();

		h = Hist(path, "AHERO");
		Double_t abs_mean = h->GetMean();
		Double_t abs_mean_error = h->GetMeanError();

		lost_energy_arr[i] = 1. - (scint_mean + abs_mean) / Double_t(e_arr[i] * 1000);
		lost_energy_error_arr[i] = lost_energy_arr[i] / TMath::Sqrt(nevents_arr[i]);

		cout << lost_energy_arr[i] << ", " << lost_energy_error_arr[i] << endl;
	}

	TGraphErrors *gr = new TGraphErrors(n_points,
		                                proton_energies,
		                                lost_energy_arr,
		                                NULL,
		                                NULL);

	gr->SetName("gr");
	gr->Draw("AP");
	gr->SetLineWidth(7);
	gr->SetLineColor(kBlack);
	gr->SetMarkerStyle(20);
	gr->SetMarkerSize(5);
	gr->SetMarkerColor(kBlack);

	gr->GetXaxis()->SetTitle("Energy [GeV]");
	gr->GetYaxis()->SetTitle("fraction of energy not absorbed by the detector");

	gPad->SetGrid();

	TF1 *f0 = new TF1("f0", "[1] * x + [0]", 1., 10.);

	Double_t x0[] = {proton_energies[0], proton_energies[1],
                     proton_energies[2]};
    Double_t y0[] = {lost_energy_arr[0], lost_energy_arr[1], 
    				 lost_energy_arr[2]};

	TGraph *gr0 = new TGraph(3, x0, y0);
	gr0->Fit(f0);
	f0->Draw("SAME");
	f0->SetLineColor(kBlack);
	f0->SetLineWidth(7);
	f0->SetLineStyle(9);


	TF1 *f1 = new TF1("f1", "[1] * x + [0]", 10., 100.);

	Double_t x1[] = {proton_energies[2], proton_energies[3],
                     proton_energies[4], proton_energies[5]};
    Double_t y1[] = {lost_energy_arr[2], lost_energy_arr[3], 
    				 lost_energy_arr[4], lost_energy_arr[5]};

	TGraph *gr1 = new TGraph(4, x1, y1);
	gr1->Fit(f1);
	f1->Draw("SAME");
	f1->SetLineColor(kBlack);
	f1->SetLineWidth(7);

	TF1 *f2 = new TF1("f2", "[1] * x + [0]", 100., 1000.);

	Double_t x2[] = {100., 1000.};
	Double_t y2[] = {lost_energy_arr[5], lost_energy_arr[6]};

	TGraph *gr2 = new TGraph(2, x2, y2);
	gr2->Fit(f2);
	f2->Draw("SAME");
	f2->SetLineColor(kBlack);
	f2->SetLineWidth(7);
	f2->SetLineStyle(10);

	TF1 *f3 = new TF1("f3", "[1] * x + [0]", 1000., 10000.);

	Double_t x3[] = {1000., 10000.};
	Double_t y3[] = {lost_energy_arr[6], lost_energy_arr[7]};

	TGraph *gr3 = new TGraph(2, x3, y3);
	gr3->Fit(f3);
	f3->Draw("SAME");
	f3->SetLineColor(kBlack);
	f3->SetLineWidth(7);
	f3->SetLineStyle(1);


	TF1 *f4 = new TF1("f4", "[1] * x + [0]", 10000., 100000.);

	Double_t x4[] = {10000., 100000.};
	Double_t y4[] = {lost_energy_arr[7], lost_energy_arr[8]};

	TGraph *gr4 = new TGraph(2, x4, y4);
	gr4->Fit(f4);
	f4->Draw("SAME");
	f4->SetLineColor(kBlack);
	f4->SetLineWidth(7);
	f4->SetLineStyle(10);
}

TH1F *Hist(TString file_path, TString opt)
{
	TFile *file = new TFile(file_path, "READ");
	if (file->IsZombie()) {
		cerr << "Can't read the file " << file_path << endl;
		return NULL; 
	}

	auto tree = (TTree*)file->Get(opt);
	if (!tree) {
		cerr << "Can't find HERO tree in the file " << file_path << endl;
		return NULL;
	}

	Int_t nopticalphotons;
	Double_t edep;
	tree->SetBranchAddress("edep", &edep);

	gROOT->cd();
	TH1F *result = new TH1F(opt + "_" + file_path, opt + "_" + file_path, 100, 1., 0.);
	for (Int_t i=0; i<tree->GetEntries(); i++) {
		tree->GetEntry(i);
		result->Fill(edep);
	}

	delete tree;
	file->Close();

	return result;
}