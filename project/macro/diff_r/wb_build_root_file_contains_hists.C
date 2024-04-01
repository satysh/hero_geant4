void now_r_hist_build(Int_t R=125, Int_t year=2010, Int_t nevents=10000, TFile *out_file=NULL);
void wb_build_root_file_contains_hists()
{
	Int_t year = 2010;
	Int_t nevents = 10000;
	TString out_file_name;
	out_file_name.Form("%d_wb_hists_diff_r.root", year);
	TFile *out_file = new TFile(out_file_name, "RECREATE");

	const Int_t n_poinst = 9;
	Int_t r_list[n_poinst] = {1000, 500, 250, 125, 100, 80, 62, 50, 30};

	for (Int_t i=0; i<n_poinst; i++) {
		now_r_hist_build(r_list[i], year, nevents, out_file);
	}
}

void now_r_hist_build(Int_t R=125, Int_t year=2010, Int_t nevents=10000, TFile *out_file=NULL)
{
	Double_t det_size_norm = 8. * (TMath::Pi() * Double_t(R) / 100.) * (TMath::Pi() * Double_t(R) / 100.);
	Double_t integral;
	if (year == 2010) {
		integral = 3913.328;
	}
	else if (year == 2014) {
		integral = 1695.491;
	}
	else {
		integral = 0.;
	}

	Double_t time = 100. / 1000000.; // sec ~ 100 usec
	Double_t det_norm = det_size_norm * integral * time;
	cout << "det_norm=" << det_norm << endl;

	Double_t norm = Double_t(nevents) / det_norm; // 2010: 46.9, 2014: 20.75 
	cout << norm << endl;
	TString fileName;
	fileName.Form("../input/%d/wb/%d_C_48_H_52_background_100_usec_nevents_%d_r_%d.root", year, year, nevents, R);
	TFile *file = new TFile(fileName.Data(), "READ");
	if (file->IsZombie()) {
		cerr << "Can't read root file!" << endl;
		return;
	}

	TTree *tree = (TTree*)file->Get("HERO");
	if (!tree) {
		cerr << "Can't find tree!" << endl;
		return;
	}

	TString now_hist_name;
	now_hist_name.Form("f_hist_r");
	Int_t bin_num = 120;
	Double_t max_bin_value = Double_t(bin_num); // usec
	TH1F *f_now_hist = new TH1F(now_hist_name, now_hist_name, bin_num, 0., max_bin_value);

	TCanvas *c = new TCanvas("c", "c");
	c->SetLogy();
	tree->Draw("t*0.001 >> f_hist_r", "pdg == 1000020040");

	now_hist_name.Form("hist_r_%d", R);
	TH1F *now_hist = new TH1F(now_hist_name, now_hist_name, bin_num, 0., max_bin_value);

	for (Int_t i=0; i<bin_num; i++) {
		now_hist->SetBinContent(i, Double_t(f_now_hist->GetBinContent(i) / norm));
	}

	c->cd();
	now_hist->Draw();

	out_file->cd();
	now_hist->Write();
	c->Close();
}