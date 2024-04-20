TH1F *hist(TString file_name);
void resolution()
{
	Int_t npoints = 4;
	Int_t points[] = {100, 1000, 10000, 100000};

	TCanvas *c = new TCanvas("canv", "canv", 800, 800);
	c->Divide(2, 2);
	TGraph *result = new TGraph();
	for (UInt_t i=0; i<npoints; i++) {
		TString now_file_name;
		now_file_name.Form("output/%d_GeV_sum_edep_distribution_calc.root", points[i]);
		
		TH1F *now_h = hist(now_file_name);
		if (!now_h) {
			continue;
		}

		Double_t now_mean = now_h->GetMean();
		Double_t now_std_dev = now_h->GetStdDev();

		cout << points[i] << ": " << now_mean << ", " << now_std_dev << endl;
	    result->AddPoint((Double_t)points[i], now_std_dev / now_mean);
	    TString hist_title;
	    hist_title.Form("%d_GeV", points[i]);
	    now_h->SetName(hist_title);
	    now_h->SetTitle(hist_title + " primary proton energy");
	    now_h->GetXaxis()->SetTitle("sum_edep [GeV]");
	    now_h->GetYaxis()->SetTitle("counts");

	    c->cd(i+1);
	    now_h->Draw();
	}

	TCanvas *c_res = new TCanvas("c_res", "c_res");
	c_res->cd();
	result->Draw("ALP");
	result->SetMarkerStyle(20);
	result->SetMarkerSize(2);
	result->SetMarkerColor(kBlue);
	result->SetLineColor(kBlue);
	result->SetLineWidth(3);
	result->GetXaxis()->SetTitle("E [GeV]");
	result->GetYaxis()->SetTitle("resolution []");
	result->SetTitle("resolution as function of primary proton Energy");
	gPad->SetGrid(2, 2);
}

TH1F *hist(TString file_name)
{
	TFile *file = new TFile(file_name.Data(), "READ");
	if (file->IsZombie()) {
		cerr << "Can't read file " << file_name << endl;
		return NULL;
	}

	auto keys = file->GetListOfKeys();
	keys->Print(); 
	TIter iter(keys);
	TString now_hist_name(iter.Next()->GetName());
	auto result = (TH1F*)file->Get(now_hist_name);
	if (!result) {
		cerr << "Can't find h_0 in " << file_name << endl;
		return NULL;
	}

	for (UInt_t i=0; i<keys->GetSize() - 1; i++) {
		TString now_hist_name(iter.Next()->GetName());
		result->Add((TH1F*)file->Get(now_hist_name));
	}

	if (file_name.Contains("1000_"))
		result->Rebin(10);
	else if (file_name.Contains("10000_"))
		result->Rebin(100);
	else if (file_name.Contains("100000_"))
		result->Rebin(10000);

	return result;
}