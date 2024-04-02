void mean_bakcground_count_and_std()
{
	TString path = "/home/ilyas/soft/hero_geant4/project/macro/thresholdscalculation/output/background/hists/";
	TString inFileName = "hists_1_bin_1usecbackground_2010_469319_ev_1_sec.root";
	//TString inFileName = "hists_background_2014_207512_ev_1_sec.root";

	TFile *file = new TFile(path + inFileName, "READ");
  if (file->IsZombie()) {
    cerr << "Can't open input file:" << inFileName << endl;
    return;
  }
  else {
    cout << inFileName << " is processing!" << endl;
  }

  TH1F *a_hist = (TH1F*)file->Get("a_b_hist");
  if (!a_hist) {
  	cerr << "Can't find hist!" << endl;
  	return;
  }

  TH1I *bh = new TH1I("bh", "bh", 100, 0, 200);
  Double_t xi = 0.;
  Double_t xi2 = 0.;
  Int_t nbins = a_hist->GetNbinsX();
  cout << "nbins = " << nbins << endl;
  for (Int_t i=0; i<nbins; i++) {
  	bh->Fill(a_hist->GetBinContent(i));
  	xi += a_hist->GetBinContent(i);
  	xi2 += a_hist->GetBinContent(i) * a_hist->GetBinContent(i);
  }

  bh->Draw();

  cout << "mean = " << xi / nbins << endl;
  cout << "stdDev = " << TMath::Sqrt((nbins * xi2 + xi * xi) / nbins / (nbins - 1)) << endl;
}