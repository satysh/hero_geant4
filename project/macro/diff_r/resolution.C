TH1F *hist(TString file_name);
TH1F *hist_from_tree(Int_t energy, Bool_t alpha=false);
void resolution()
{
	const Int_t npoints = 7;
	Int_t points[] = {1000, 10000, 20000, 40000, 60000, 80000, 100000};

	Double_t ratio[npoints];
	Double_t ratioe[npoints];

	Double_t alpha_ratio[npoints];
	Double_t alpha_ratioe[npoints];

	for (UInt_t i=0; i<npoints; i++) {
		//TString now_file_name;
		//now_file_name.Form("output/%d_GeV_sum_edep_distribution_calc.root", points[i]);
		
		TH1F *now_h = hist_from_tree(points[i]);
		if (!now_h) {
			continue;
		}

		Double_t now_mean = now_h->GetMean();
		Double_t now_std_dev = now_h->GetStdDev();
		Double_t now_meane = now_h->GetMeanError();
		Double_t now_std_deve = now_h->GetStdDevError();


	    ratio[i] = now_std_dev / now_mean;
		cout << points[i] << ": " << now_mean << ", " << now_std_dev << ", " << ratio[i] 
			 << " nevents = " << now_h->GetEntries() << endl;
	    ratioe[i] = 3. * (((now_std_dev + now_std_deve) / (now_mean + now_meane)) - ratio[i]);
	    cout << "errors: " << now_meane << ", " << now_std_deve << ", " << ratioe[i] << endl;

	    TString hist_title;
	    hist_title.Form("%d_GeV", points[i]);
	    now_h->SetName(hist_title);
	    now_h->SetTitle(hist_title + " primary proton energy");
	    now_h->GetXaxis()->SetTitle("sum_edep [GeV]");
	    now_h->GetYaxis()->SetTitle("counts");
	    now_h->SetLineWidth(3);
	    now_h->SetFillStyle(3008); // 3008 3003
	    now_h->SetFillColor(kBlue);

	    now_h->Draw();
	    //now_h->SetStats(0);

		auto legend = new TLegend(0.5,0.1,0.9,0.9);
		//legend->SetTextSize(0.028);
	    legend->SetTextFont(60);
		TString legendTitle;
		legendTitle.Form("%d events", (Int_t)now_h->GetEntries());
		legend->SetHeader(legendTitle);
		legendTitle.Form("all particles, mean = %.2f, stdDev = %.2f", now_mean, now_std_dev);
		legend->AddEntry(now_h, legendTitle);


		now_h = hist_from_tree(points[i], true);
		if (!now_h) {
			continue;
		}

		now_mean = now_h->GetMean();
		now_meane = now_h->GetMeanError();	
		now_std_dev = now_h->GetStdDev();
		now_std_deve = now_h->GetStdDevError();

		cout << "alpha: " << points[i] << ": " << now_mean << ", " << now_std_dev << ", " << now_std_dev / now_mean << endl;
	    alpha_ratio[i] = now_std_dev / now_mean;
	    alpha_ratioe[i] = 3. * ((now_std_dev + now_std_deve) / (now_mean + now_meane) - alpha_ratio[i]);
		cout << "errors: " << now_meane << ", " << now_std_deve << ", " << alpha_ratio[i] << endl;

	    legendTitle.Form("alpha particles only, mean = %.2f, stdDev = %.2f", now_mean, now_std_dev);
		legend->AddEntry(now_h, legendTitle);
		now_h->Draw("SAME");
		now_h->SetLineColor(kRed);
		now_h->SetLineWidth(3);
	    now_h->SetFillStyle(3003); // 3008 3003
	    now_h->SetFillColor(kRed);
	    hist_title.Form("alpha_%d_GeV", points[i]);
	    now_h->SetName(hist_title);

		//legend->Draw();

	}

	Double_t energies[npoints];
	for (UInt_t i=0; i<npoints; i++) {
		energies[i] = (Double_t)points[i] / 1000.;
		/*ratio[i] /= 1000.;
		ratioe[i] /= 1000.;
		alpha_ratio[i] /= 1000.;
		alpha_ratioe[i] /= 1000.;
		ratio[i] = TMath::Log(ratio[i]);
		ratioe[i] = TMath::Log(ratio[i] + ratioe[i]) - ratio[i];
		alpha_ratio[i] = TMath::Log(alpha_ratio[i]);
		alpha_ratioe[i] = TMath::Log(alpha_ratio[i] + alpha_ratioe[i]) - alpha_ratio[i];*/
	}

	TCanvas *c_res = new TCanvas("c_res", "c_res");
	c_res->cd();
	TGraphErrors *result = new TGraphErrors(npoints, energies, ratio, NULL, ratioe);
	result->SetMarkerStyle(20);
	result->SetMarkerSize(2);
	result->SetMarkerColor(kBlue);
	result->SetLineColor(kBlue);
	result->SetLineWidth(5);
    
    TGraphErrors *alpha_result = new TGraphErrors(npoints, energies, alpha_ratio, NULL, alpha_ratioe);
    alpha_result->SetMarkerStyle(20);
	alpha_result->SetMarkerSize(2);
	alpha_result->SetMarkerColor(kRed);
	alpha_result->SetLineColor(kRed);
	alpha_result->SetLineWidth(3);
	
	auto legend = new TLegend(0.6,0.8,0.75,0.9);
	legend->AddEntry(result, "all particles");
	legend->AddEntry(alpha_result, "alpha particles only");
	
	TMultiGraph *mg = new TMultiGraph();
	mg->SetTitle("");
	mg->Add(result);
	mg->Add(alpha_result);
	mg->GetXaxis()->SetTitle("E [TeV]");
	mg->GetYaxis()->SetTitle("resolution");
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

TH1F *hist_from_tree(Int_t energy, Bool_t alpha=false)
{
	TString file_name;
	file_name.Form("output/%d_GeV_params_result.root", energy);
	TFile *file = new TFile(file_name, "READ");
	if (file->IsZombie()) {
		cerr << "Can't read file " << file_name << "!" << endl;
		return NULL;
	}

	TTree *tree = (TTree*)file->Get("params_result");
	if (!tree) {
		cerr << "Can't find tree! in" << file_name << endl;
		tree = (TTree*)file->Get("params");
		if (!tree)
			return NULL;
	}

	TString canv_name;
	if (!alpha)
		canv_name.Form("canv_%d_GeV", energy);
	else
		canv_name.Form("alpha_canv_%d_GeV", energy);
	TCanvas *tmp_c = new TCanvas(canv_name, canv_name, 800, 800);
	//TString hist_name;
	//hist_name.Form("result_%d", energy);
	//Int_t nbins = energy*2;
	//TH1F *result = new TH1F(hist_name, hist_name, nbins, 0., (Double_t)energy);
	TString draw_opt;
	if (!alpha)
		//draw_opt.Form("sum_edep_scint*0.001 >> %s", hist_name.Data());
		draw_opt = "sum_edep_scint*0.001";

	else
		//draw_opt.Form("sum_edep_alpha_scint*0.001 >> %s", hist_name.Data());
		draw_opt = "sum_edep_alpha_scint*0.001";
	tree->Draw(draw_opt);

	TH1F *result = (TH1F*)gPad->FindObject("htemp");
	return result;
}


