TH1F *hist(TString file_name);
void resolution()
{
	Int_t npoints = 2;
	Int_t points[] = {20, 40, 60, 80, 100, 1000, 10000, 100000};

	TCanvas *c = new TCanvas("canv", "canv", 800, 800);
	//c->Divide(4, 2);
	TGraph *result = new TGraph();
	TGraph *wb_result = new TGraph();
	for (UInt_t i=0; i<npoints; i++) {
		TString now_file_name;
		now_file_name.Form("output/%d_GeV_sum_edep_distribution_calc.root", points[i]);
		
		TH1F *now_h = hist(now_file_name);
		if (!now_h) {
			continue;
		}

		Double_t now_mean = now_h->GetMean();
		Double_t now_std_dev = now_h->GetStdDev();

		cout << points[i] << ": " << now_mean << ", " << now_std_dev << ", " << now_std_dev / now_mean << " nevents = " << now_h->GetEntries() << endl;
	    result->AddPoint((Double_t)points[i], now_std_dev / now_mean);
	    TString hist_title;
	    hist_title.Form("%d_GeV", points[i]);
	    now_h->SetName(hist_title);
	    now_h->SetTitle(hist_title + " primary proton energy");
	    now_h->GetXaxis()->SetTitle("sum_edep [GeV]");
	    now_h->GetYaxis()->SetTitle("counts");
	    now_h->SetLineWidth(3);
	    now_h->SetFillStyle(3008); // 3008 3003
	    now_h->SetFillColor(kBlue);

	    c->cd(i+1);
	    now_h->Draw();
	    now_h->SetStats(0);

		auto legend = new TLegend(0.5,0.1,0.9,0.9);
		//legend->SetTextSize(0.028);
	    legend->SetTextFont(60);
		TString legendTitle;
		legendTitle.Form("%d events", (Int_t)now_h->GetEntries());
		legend->SetHeader(legendTitle);
		legendTitle.Form("scint + B, mean = %.2f, stdDev = %.2f", now_mean, now_std_dev);
		legend->AddEntry(now_h, legendTitle);

		now_file_name.Form("output/wb_%d_GeV_sum_edep_distribution_calc.root", points[i]);
		now_h = hist(now_file_name);
		if (!now_h) {
			continue;
		}

		now_mean = now_h->GetMean();
		now_std_dev = now_h->GetStdDev();

		cout << "wb: " << points[i] << ": " << now_mean << ", " << now_std_dev << ", " << now_std_dev / now_mean << endl;
	    wb_result->AddPoint((Double_t)points[i], now_std_dev / now_mean);

	    legendTitle.Form("scint + only, mean = %.2f, stdDev = %.2f", now_mean, now_std_dev);
		legend->AddEntry(now_h, legendTitle);
		now_h->Draw("SAME");
		now_h->SetLineColor(kRed);
		now_h->SetLineWidth(3);
	    now_h->SetFillStyle(3003); // 3008 3003
	    now_h->SetFillColor(kRed);
	    hist_title.Form("wb_%d_GeV", points[i]);
	    now_h->SetName(hist_title);

		legend->Draw();

	}

	TCanvas *c_res = new TCanvas("c_res", "c_res");
	c_res->cd();
	result->SetMarkerStyle(20);
	result->SetMarkerSize(2);
	result->SetMarkerColor(kBlue);
	result->SetLineColor(kBlue);
	result->SetLineWidth(3);
    
    wb_result->SetMarkerStyle(20);
	wb_result->SetMarkerSize(2);
	wb_result->SetMarkerColor(kRed);
	wb_result->SetLineColor(kRed);
	wb_result->SetLineWidth(3);
	
	auto legend = new TLegend(0.6,0.8,0.75,0.9);
	legend->AddEntry(result, "scint + B");
	legend->AddEntry(wb_result, "scint only");
	
	TMultiGraph *mg = new TMultiGraph();
	mg->SetTitle("resolution as function of primary proton Energy");
	mg->Add(result);
	mg->Add(wb_result);
	mg->GetXaxis()->SetTitle("E [GeV]");
	mg->GetYaxis()->SetTitle("resolution []");
	mg->Draw("ALP");
	legend->Draw();
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
	//keys->Print(); 
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