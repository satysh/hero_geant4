TH1F* hist(TString fileName="", Double_t nBaseEvents=0., Double_t nEvents=0.);

void normThresh()
{
  TCanvas* canv = new TCanvas("canv", "canv");
  canv->cd();
  TH1F *new_h;
  TLegend* legend = new TLegend(0.1, 0.7, 0.48, 0.9);

  new_h = hist("background_2010_10000_ev_old_wrong_geo.root", 47., 10000.);
  new_h->Draw();
  new_h->SetStats(0);
  new_h->SetTitle("background alpha counts [0:100] usec (1 bin ~ 1 usec)");
  new_h->GetXaxis()->SetTitle("time [usec]");
  new_h->GetYaxis()->SetTitle("alpha counts");
  new_h->SetLineColor(kBlue);

/*
  legend->AddEntry(new_h,"R=250 cm","l");
  new_h = hist("alpha_background_2010_10000_ev_100_usec.root", 47., 10000.);
  canv->cd();
  new_h->Draw("SAME");
  new_h->SetLineColor(kBlue);
  legend->AddEntry(new_h, "R=125 cm","l");

  new_h = hist("alpha_background_2010_10000_ev_100_usec_r62.root", 47., 10000.);
  canv->cd();
  new_h->Draw("SAME");
  new_h->SetLineColor(kGreen);
  legend->AddEntry(new_h, "R=62 cm","l");

  legend->Draw();
*/

}

TH1F* hist(TString fileName, Double_t nBaseEvents, Double_t nEvents) {
  TFile *file = new TFile("input/"+fileName, "READ");
  if (file->IsZombie()) {
    cerr << "Can't open input file:" << fileName << endl;
    return NULL;
  }
  else {
    cout << fileName << " is processing!" << endl;
  }

  TTree *tree = (TTree*)file->Get("HERO");
  if (!tree) {
    cerr << "Can't find input tree!" << endl;
    return NULL;
  }

  Int_t evId;
  Int_t pdg;
  Double_t time;
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("t", &time);

  TCanvas *c = new TCanvas("c", "c");
  //c->Divide(1, 2);

  Int_t nBins = 120;
  TH1F *h = new TH1F("base", "base", nBins, 0., 120.);
  TH1F *new_h = new TH1F("normed", "normed", nBins, 0., 120.);

  // t*0.0001 потому что я ошибся при инициализации maxStartTime,
  //вместо 62 500 000 nanoseconds, я указал 625 000 000.
  // В выходной файл симуляции hero.root time пишется в наносекундах,
  // а мы хотим видеть микросекунды.
  c->cd();
  tree->Draw("t*0.001 >> base", "pdg == 1000020040");

  Double_t norm = nBaseEvents / nEvents;

  for (Int_t i=0; i<nBins; i++) {
    Double_t oldBinContent = (Double_t)h->GetBinContent(i);
    Double_t newBinContent = oldBinContent * norm;
    cout << oldBinContent << ", " << newBinContent << ", " << norm << endl;
    new_h->SetBinContent(i, newBinContent);
  }

  delete c;
  return new_h;
}
