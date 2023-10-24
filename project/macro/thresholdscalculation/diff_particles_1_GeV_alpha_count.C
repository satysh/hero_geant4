TH1F* get_normed_hist(TString inputFileName="", Int_t nEvents=0);

void diff_particles_1_GeV_alpha_count()
{

  TLegend* legend = new TLegend(0.1,0.7,0.48,0.9);
  legend->SetHeader("primary E=1 GeV/n","C"); // option "C" allows to center the header
  TH1F* normed_hist = get_normed_hist("Fe_1_GeV_10000_events.root", 10000);
  TCanvas* canv = new TCanvas("canv", "canv");
  canv->cd();
  normed_hist->Draw();
  normed_hist->SetStats(0);
  normed_hist->SetLineWidth(2);
  normed_hist->SetLineColor(1);
  normed_hist->SetTitle("Mean alpha counts per 1 primary particle(1 bin ~ 1 usec)");
  normed_hist->GetXaxis()->SetTitle("time moment [usec]");
  normed_hist->GetYaxis()->SetTitle("alpha counts");
  legend->AddEntry(normed_hist,"Fe","l");

  normed_hist = get_normed_hist("He_1_GeV_10000_events.root", 10000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(2);
  normed_hist->SetLineColor(2);
  legend->AddEntry(normed_hist,"He","l");

  normed_hist = get_normed_hist("proton_1_GeV_10000_events.root", 10000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(2);
  normed_hist->SetLineColor(3);
  legend->AddEntry(normed_hist,"proton","l");


  legend->Draw();
}

TH1F* get_normed_hist(TString inputFileName, Int_t nEvents) {
  TFile* file = new TFile("input/" + inputFileName, "READ");
  if (file->IsZombie()) {
    cerr << "Can't read file:" << inputFileName << endl;
    return NULL;
  }

  TTree* tree = (TTree*)file->Get("HERO");
  if (!tree) {
    cerr << "Can't find tree!" << endl;
    return NULL;
  }

  Int_t binN = 50;
  Double_t minBin = 0.;
  Double_t maxBin = (Double_t)binN;
  TH1F* hist = new TH1F("hist", "hist", binN, minBin, maxBin);
/*
  Double_t t;
  tree->SetBranchAddress("t", &t);
  for (Int_t i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    hist->Fill(t * 0.001); // to usec
  }
*/
  TCanvas* tmp = new TCanvas("tmp", "tmp");
  tmp->cd();
  tree->Draw("t*0.001 >> hist", "pdg==1000020040 && t < 128000");
  TH1F* normed_hist = new TH1F("normed_hist", "normed_hist", binN, minBin, maxBin);

  for (Int_t i=0; i<binN; i++) {
    normed_hist->SetBinContent(i, hist->GetBinContent(i) / nEvents);
  }

  delete tmp;
  return normed_hist;
}
