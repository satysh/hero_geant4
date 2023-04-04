void thresholdscalculation()
{
  TFile *hist_file = new TFile("input/background/hist1per16.root", "READ");
  if (hist_file->IsZombie()) {
    cerr << "Can't open input file!" << endl;
    return;
  }

  TH1F *histo = (TH1F*)hist_file->Get("histo");
  if (!histo) {
    cerr << "Can't find histo!" << endl;
    return;
  }

  TCanvas* canv = new TCanvas("canv", "canv");
  canv->Divide(2, 1);
  canv->cd(1);
  histo->Draw();
  histo->SetStats(0);
  histo->GetXaxis()->SetTitle("time [usec]");

  Int_t binN = histo->GetNbinsX();
  Double_t xlow = histo->GetXaxis()->GetXmin();
  Double_t xup = histo->GetXaxis()->GetXmax();

  cout << "input hist: binN=" << binN << ", xlow=" << xlow << ", xup=" << xup << endl;

  TFile *file = new TFile("input/alpha/100GeV.root", "READ");
  if (file->IsZombie()) {
    cerr << "Can't open input file!" << endl;
    return;
  }

  TTree *tree = (TTree*)file->Get("HERO");
  if (!tree) {
    cerr << "Can't find input tree!" << endl;
    return;
  }

  Int_t evId;
  Int_t pdg;
  Double_t time;
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("t", &time);
  TH1F *new_hist = new TH1F("new_hist", "new_hist", binN, xlow, xup);
  tree->Draw("t*0.001 >>new_hist", "pdg == 1000020040", "SAME");

  TH1F *resultHist = new TH1F("resultHist", "resultHist", binN, xlow, xup);
  for (Int_t i=0; i<binN; i++) {
    Int_t newBinContent = histo->GetBinContent(i);
    newBinContent += new_hist->GetBinContent(i);
    resultHist->SetBinContent(i, newBinContent);
  }

  canv->cd(2);
  resultHist->Draw();
  resultHist->SetStats(0);
  resultHist->GetXaxis()->SetTitle("time [usec]");

  TFile *out_file = new TFile("outputfruqdist.root", "RECREATE");
  resultHist->Write();
}
