// Макрос для построения гистограммы фонового счета с заданной шириной бина
// Сохраняет построенную гистограмму в root файл 

void make_background_hist()
{
	TString input_file_name = "background_2014_207512_ev_1_sec.root";
	TFile *file = new TFile("input/" + input_file_name);
	if (file->IsZombie()) {	
		cerr << "Can't open input file!" << endl;
		return;
	}

	TTree *tree = (TTree*)file->Get("HERO");
	if (!tree) {
		cerr << "Can't find tree!" << endl;
		return;
	}

	Double_t bin_len = 1.; // [usec]
	Double_t max_bin_val = 1500500.; // 1.0005 million usec-s
	Int_t n_bins = Int_t(max_bin_val / bin_len);
	TH1F *b_hist = new TH1F("b_hist", "b_hist", n_bins, 0., max_bin_val);
	TH1F *a_b_hist = new TH1F("a_b_hist", "a_b_hist", n_bins, 0., max_bin_val);
	a_b_hist->SetLineColor(kRed);

	TCanvas *b_canv = new TCanvas("b_canv", "b_canv");
	tree->Draw("t*0.001 >> b_hist", "");
	tree->Draw("t*0.001 >> a_b_hist", "pdg == 1000020040", "SAME");
/*
	TFile *out_file = new TFile("output/background/hists/hists_"+input_file_name, "RECREATE");
	b_hist->Write();
	a_b_hist->Write();
	*/
}