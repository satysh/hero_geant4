void normThresh()
{
  TFile *file = new TFile("input/costheta_128us.root", "READ");
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

  TCanvas *c = new TCanvas("c", "c");
  //c->Divide(1, 2);

  Int_t nBins = 100;
  TH1F *h = new TH1F("base", "base", nBins, 0., 160.);
  TH1F *new_h = new TH1F("normed", "normed", nBins, 0., 160.);

  // t*0.0001 потому что я ошибся при инициализации maxStartTime,
  //вместо 62 500 000 nanoseconds, я указал 625 000 000.
  // В выходной файл симуляции hero.root time пишется в наносекундах,
  // а мы хотим видеть микросекунды.
  //c->cd(1);
  tree->Draw("t*0.001 >> base", "pdg == 1000020040");

  Double_t numOfPrimaryEvents = 10000.;
  Double_t numOfPrimaryEventsIn1pre16sec = 7941.;
  Double_t norm = numOfPrimaryEventsIn1pre16sec / numOfPrimaryEvents;

  for (Int_t i=0; i<nBins; i++) {
    Double_t oldBinContent = (Double_t)h->GetBinContent(i);
    Double_t newBinContent = oldBinContent * norm;
    cout << oldBinContent << ", " << newBinContent << ", " << norm << endl;
    new_h->SetBinContent(i, newBinContent);
  }

  //c->cd(2);
  new_h->Draw();
  h->SetStats(0);
  new_h->SetStats(0);
  h->GetXaxis()->SetTitle("time [usec]");
  new_h->GetXaxis()->SetTitle("time [usec]");
  h->GetYaxis()->SetTitle("count");
  new_h->GetYaxis()->SetTitle("count");
}
