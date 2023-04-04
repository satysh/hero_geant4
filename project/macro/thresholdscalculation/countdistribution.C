void countdistribution()
{
  TFile *hist_file = new TFile("input/hist1per16.root", "READ");
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

  cout << binN << ", " << xlow << ", " << xup << endl;

  Double_t minCount = DBL_MAX;
  Double_t maxCount = 0.;

  for (Int_t i=0; i<binN; i++) {
    Int_t currCount = histo->GetBinContent(i);
    if (currCount > maxCount) maxCount = currCount;
    if (currCount < minCount) minCount = currCount;
    /*if (currCount > 6413 and currCount <= 22446) {
      cout << histo->GetBinCenter(i) << endl;
    }*/
  }
  cout << minCount << ", " << maxCount << endl;
  //minCount = 30000;
  Int_t binNN = 100;
  TH1F *resultHist = new TH1F("resultHist", "resultHisto", binNN, (Double_t)minCount, (Double_t)maxCount);

  for (Int_t i=0; i<binN; i++) {
    if (histo->GetBinCenter(i) >= 60000.) {
      //cout << histo->GetBinCenter(i) << ", " << histo->GetBinContent(i) << endl;
      continue;
    }
    resultHist->Fill(histo->GetBinContent(i));
  }

  resultHist->Draw();
  resultHist->SetLineWidth(3);
  resultHist->GetXaxis()->SetTitle("count");
  resultHist->GetYaxis()->SetTitle("frequency");
  resultHist->SetTitle("the background count distribution");

  TFile *input_file = new TFile("outputfruqdist.root", "READ");
  if (input_file->IsZombie()) {
    cerr << "Can't open input file!" << endl;
    return;
  }

  TH1F *input_hist = (TH1F*)input_file->Get("resultHist");
  if (!input_hist) {
    cerr << "Can't find resultHist!" << endl;
    return;
  }

  TH1F *new_resultHist = new TH1F("new_hist", "new_h", binNN, (Double_t)minCount, (Double_t)maxCount);

  for (Int_t i=0; i<binN; i++) {
    if (input_hist->GetBinCenter(i) >= 60000.) {
      //cout << input_hist->GetBinCenter(i) << ", " << histo->GetBinContent(i) << endl;
      continue;
    }
    new_resultHist->Fill(input_hist->GetBinContent(i));
  }

  //new_resultHist->Draw("SAME");
  new_resultHist->SetLineColor(kRed);

}
