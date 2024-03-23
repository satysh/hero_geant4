void draw()
{
	Double_t x[45];
	Double_t y[45];

	ifstream fin("output/calculate_alpha_counts_per_intervals_out.txt");
	if (!fin.is_open()) {
		cerr << "Can't find input!" << endl;
		return;
	}


	for (Int_t i=0; i<45; i++) {
		Double_t E0, E1, value;
		fin >> E0 >> E1 >> value;
		x[i] = 0.5 * (E0 + E1);
		y[i] = value;
	}


	TCanvas *canv = new TCanvas("canv", "canv");
	canv->SetLogy();
	TGraph *gr = new TGraph(45, x, y);
	gr->Draw("AP");
	TAxis *xax = gr->GetXaxis();	
	TAxis *yax = gr->GetYaxis();	

	//yax->SetRangeUser(0., 10000000000000.);
	gr->SetMarkerStyle(8);
	gr->SetMarkerSize(2);
	gr->SetMarkerColor(kRed);
	gr->SetTitle("alpha's count statistic for primary protons of different. energies during one year");
	xax->SetTitle("primary proton energy [GeV]");
	yax->SetTitle("alpha's count in the scintillator");
}