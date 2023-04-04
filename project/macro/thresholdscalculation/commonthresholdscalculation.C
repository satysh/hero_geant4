void add(TH1F *histo, TH1F *resultHist, TString fileName);

void commonthresholdscalculation()
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

  Int_t binN = histo->GetNbinsX();
  Double_t xlow = histo->GetXaxis()->GetXmin();
  Double_t xup = histo->GetXaxis()->GetXmax();

  cout << "input hist: binN=" << binN << ", xlow=" << xlow << ", xup=" << xup << endl;

  TH1F *resultHist = new TH1F("resultHist", "resultHist", binN, xlow, xup);
  resultHist->SetTitle("alpha background distribution");

  add(histo, resultHist, "proton/10TeV.root");
  add(resultHist, resultHist, "proton/500GeV.root");
  add(resultHist, resultHist, "proton/1TeV.root");
  add(resultHist, resultHist, "electron/10TeV.root");
  add(resultHist, resultHist, "electron/20TeV.root");
  add(resultHist, resultHist, "gamma/5TeV.root");
  add(resultHist, resultHist, "gamma/10TeV.root");
  add(resultHist, resultHist, "alpha/100GeV.root");
  add(resultHist, resultHist, "alpha/500GeV.root");
  add(resultHist, resultHist, "alpha/1TeV.root");

  TCanvas* canv = new TCanvas("canv", "canv");
  canv->cd();
  resultHist->Draw();
  resultHist->SetStats(0);
  resultHist->GetXaxis()->SetTitle("time [usec]");

  TLine *line_up = new TLine(0., 4942.2, 70000., 4942.2);
  line_up->Draw();
  line_up->SetLineWidth(3);
  line_up->SetLineColor(kRed);
  line_up->SetLineStyle(9);

  TLine *line_down = new TLine(0., 2767.8, 70000., 2767.8);
  line_down->Draw();
  line_down->SetLineWidth(3);
  line_down->SetLineColor(kRed);
  line_down->SetLineStyle(9);
}

void add(TH1F *histo, TH1F *resultHist, TString fileName)
{
  fileName = "input/" + fileName;
  TFile *file = new TFile(fileName.Data(), "READ");
  if (file->IsZombie()) {
    cerr << "Can't open input file!" << endl;
    return;
  }

  TTree *tree = (TTree*)file->Get("HERO");
  if (!tree) {
    cerr << "Can't find input tree!" << endl;
    return;
  }

  Int_t binN = histo->GetNbinsX();
  Double_t xlow = histo->GetXaxis()->GetXmin();
  Double_t xup = histo->GetXaxis()->GetXmax();

  Int_t evId;
  Int_t pdg;
  Double_t time;
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("t", &time);
  static Int_t i = 0;
  i++;
  TString histo_name;
  histo_name.Form("new_hist_%d", i);
  TH1F *new_hist = new TH1F(histo_name, histo_name, binN, xlow, xup);
  TString rule = TString("t*0.001 >>") + histo_name;
  tree->Draw(rule, "pdg == 1000020040", "SAME");

  for (Int_t i=0; i<binN; i++) {
    Int_t newBinContent = histo->GetBinContent(i);
    newBinContent += new_hist->GetBinContent(i);
    resultHist->SetBinContent(i, newBinContent);
  }
}
