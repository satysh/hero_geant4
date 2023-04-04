void normThresh()
{
  TFile *file = new TFile("input/energy_is_spreaded_10kevents.root", "READ");
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
  c->Divide(1, 2);

  Int_t nBins = 100;
  TH1F *h = new TH1F("h", "h", nBins, 0., 65000.);
  TH1F *new_h = new TH1F("new_h", "new_h", nBins, 0., 65000.);

  // t*0.0001 потому что я ошибся при инициализации maxStartTime,
  //вместо 62 500 000 nanoseconds, я указал 625 000 000.
  // В выходной файл симуляции hero.root time пишется в наносекундах,
  // а мы хотим видеть микросекунды.
  c->cd(1);
  tree->Draw("t*0.0001 >> h", "pdg == 1000020040");

  Int_t numOfPrimaryEvents = 10000;
  Int_t numOfPrimaryEventsIn1pre16sec = 3877304;
  Double_t norm = (Double_t)numOfPrimaryEventsIn1pre16sec / numOfPrimaryEvents;

  for (Int_t i=0; i<nBins; i++) {
    Double_t oldBinContent = (Double_t)h->GetBinContent(i);
    Double_t newBinContent = oldBinContent * norm;
    cout << oldBinContent << ", " << newBinContent << ", " << norm << endl;
    new_h->SetBinContent(i, newBinContent);
  }

  c->cd(2);
  new_h->Draw();
}
