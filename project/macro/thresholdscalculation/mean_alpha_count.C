void mean_alpha_count()
{
  TFile* file = new TFile("input/gamma_10000_GeV_100_events.root", "READ");
  if (file->IsZombie()) {
    cerr << "Can't read file!" << endl;
    return;
  }

  TTree* tree = (TTree*)file->Get("HERO");
  if (!tree) {
    cerr << "Can't find tree!" << endl;
    return;
  }

  Int_t binN = 50;
  Double_t minBin = 0.;
  Double_t maxBin = (Double_t)binN;
  TH1F* hist = new TH1F("hist", "hist", binN, minBin, maxBin);

  tree->Draw("t*0.001 >> hist", "pdg==1000020040 && t < 128000");

  Int_t nEvents = 100;

  TH1F* normed_hist = new TH1F("normed_hist", "normed_hist", binN, minBin, maxBin);
/*
  TTree* thd_count_tree = new TTree("thd_count_tree", "thd_count_tree");
  Int_t thd_count;
  thd_count_tree->Branch("thd_count", &thd_count, "thd_count/I");

  for (Int_t i=0; i<30; i++) {
    thd_count = hist->GetBinContent(i);

    cout << thd_count << endl;
    if (thd_count > 0)
      thd_count_tree->Fill();
  }

  TCanvas* canv = new TCanvas("canv", "canv");
  thd_count_tree->Draw("thd_count");
*/

  TTree* thd_count_tree = new TTree("thd_count_tree", "thd_count_tree");
  Int_t thd_count;
  thd_count_tree->Branch("thd_count", &thd_count, "thd_count/I");

  Double_t meanCount = 0.;
  Double_t max_count = 0.;
  Double_t sx = 0.;
  for(Int_t i=0; i<binN; i++) {
    Int_t now_bin_content = (Int_t)hist->GetBinContent(i) / nEvents;
    normed_hist->SetBinContent(i, now_bin_content);
    if (now_bin_content > max_count)
      max_count = now_bin_content;
    if (now_bin_content != 0) {
      sx += (Double_t)(now_bin_content * now_bin_content);
      meanCount += (Double_t)now_bin_content;
      thd_count = now_bin_content;
      thd_count_tree->Fill();
    }
  }

  meanCount /= binN;
  Double_t coeff = 1. / binN / (binN - 1);
  sx = TMath::Sqrt(coeff * (sx - binN * meanCount * meanCount));
  cout << "mean=" << meanCount << ", stdDev=" << sx << ", max_count=" << max_count << endl;
  normed_hist->Draw("SAME");
  normed_hist->SetLineColor(kGreen);
  normed_hist->SetLineWidth(3);


  TCanvas* canv = new TCanvas("canv", "canv");
  thd_count_tree->Draw("thd_count");

}
