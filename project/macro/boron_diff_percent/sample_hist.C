TTree *get_tree(TString fileName = "data/hero_nevents_10000_pdg_2212_R_125_E_1000_bron_b_percent_5.root")
{
	TFile *file = new TFile(fileName, "READ");

	if (file->IsZombie()) {
		cerr << "Can't open file " << fileName << "!" << endl;
		return NULL;
	}

	TTree *tree = (TTree*)file->Get("HERO");
	if (!tree) {
		cerr << "Can't open tree HERO in file " << fileName << "!" << endl;
		return NULL;
	}

	return tree;
}
void sample_hist()
{
	Int_t nbins = 100;
	Double_t min_bin = 0.;
	Double_t max_bin = 1.;

	TCanvas *c = new TCanvas("c", "c");
	TTree *tree = get_tree();
	c->cd();
	TH1F *h_5 = new TH1F("h_5", "h_5", nbins, min_bin, max_bin);
	h_5->Scale(1. / 10000.);
	h_5->SetLineColor(kGreen);
	h_5->SetLineWidth(5);
	h_5->SetFillStyle(3005);
	h_5->SetFillColor(kGreen);
	h_5->GetXaxis()->SetRangeUser(0.3, 0.7);
	h_5->SetStats(0);
	h_5->GetXaxis()->SetTitle("edep [GeV]");
	h_5->GetYaxis()->SetTitle("counts / nprotons");
	h_5->SetTitle("edep hists for 0%, 1% and 5% boron in a scintillator");
	tree->Draw("edep * 0.001 >> h_5", "", "HIST");

	tree = get_tree("data/hero_nevents_10000_pdg_2212_R_125_E_1000_bron_b_percent_1.root");
	c->cd();
	TH1F *h_1 = new TH1F("h_1", "h_1", nbins, min_bin, max_bin);
	h_1->Scale(1. / 10000.);
	h_1->SetLineColor(kBlack);
	h_1->SetLineWidth(5);
	h_1->SetFillStyle(3003);
	h_1->SetFillColor(kBlack);
	tree->Draw("edep * 0.001 >> h_1", "", "HIST&&SAME");

	tree = get_tree("data/hero_nevents_10000_pdg_2212_R_125_E_1000_bron_-b_percent_0.root");
	c->cd();
	TH1F *h_0 = new TH1F("h_0", "h_0", nbins, min_bin, max_bin);
	h_0->Scale(1. / 10000.);
	h_0->SetLineColor(kBlue);
	h_0->SetLineWidth(1);
	h_0->SetFillStyle(3010);
	h_0->SetFillColor(kBlue);
	tree->Draw("edep * 0.001 >> h_0", "", "HIST&&SAME");

  //gPad->SetGrid(2, 2);

  // x1, y1, x2, y2
	auto legend = new TLegend(0.1, 0.9, 0.45, 0.65);
	//legend->SetTextSize(0.028);
	legend->SetTextFont(60);
	TString title;
	title.Form("5 percent, <edep> = %f", Float_t(h_5->GetMean()));
	legend->AddEntry(h_5, title);
	title.Form("1 percent, <edep> = %f", Float_t(h_1->GetMean()));
	legend->AddEntry(h_1, title);
	title.Form("0 percent, <edep> = %f", Float_t(h_0->GetMean()));
	legend->AddEntry(h_0, title);
	legend->Draw();
}