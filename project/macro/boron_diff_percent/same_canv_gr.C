void same_canv_gr()
{
	TFile *file = new TFile("gr_out.root", "READ");

	TGraphErrors *gr_1 = (TGraphErrors*)file->Get("gr_1");
	TGraphErrors *gr_10 = (TGraphErrors*)file->Get("gr_10");
	TGraphErrors *gr_100 = (TGraphErrors*)file->Get("gr_100");
	TGraphErrors *gr_1000 = (TGraphErrors*)file->Get("gr_1000");
	gr_1->SetMarkerColor(kBlue);
	gr_10->SetMarkerColor(kBlack);
	gr_100->SetMarkerColor(kGreen);
	gr_1000->SetMarkerColor(kViolet);
	gr_10->SetLineColor(kBlack);
	gr_100->SetLineColor(kGreen);
	gr_1000->SetLineColor(kViolet);

	TF1 *f_1 = new TF1("f_1", "[1] * x + [0]");
	gr_1->Fit(f_1);
	TF1 *f_10 = new TF1("f_10", "[1] * x + [0]");
	gr_10->Fit(f_10);
	TF1 *f_100 = new TF1("f_100", "[1] * x + [0]");
	gr_100->Fit(f_100);
	TF1 *f_1000 = new TF1("f_1000", "[1] * x + [0]");
	gr_1000->Fit(f_1000);

	TMultiGraph *mg = new TMultiGraph();
	mg->Add(gr_1);
	mg->Add(gr_10);
	mg->Add(gr_100);
	mg->Add(gr_1000);

	TCanvas *c = new TCanvas("c", "c");
	mg->Draw("ACP");
	mg->SetTitle("mean edep vs boron percent");
	mg->GetXaxis()->SetTitle("boron percent");
	mg->GetYaxis()->SetTitle("mean_edep / pE");

	// x1, y1, x2, y2
	auto legend = new TLegend(0.65, 0.9, 0.9, 0.75);
	//legend->SetTextSize(0.028);
	legend->SetTextFont(60);
	legend->AddEntry(gr_1,  "pE=1 GeV, 10000 protons");
	legend->AddEntry(gr_10, "pE=10 GeV, 10000 protons");
	legend->AddEntry(gr_100,"pE=100 GeV, 1000 protons");
	legend->AddEntry(gr_1000,"pE=1000 GeV, 100 protons");
	legend->Draw();

	gPad->SetGrid(2, 2);
	mg->GetYaxis()->SetRangeUser(0.34, 0.57);
	mg->GetXaxis()->SetLimits(0., 6.1);
}