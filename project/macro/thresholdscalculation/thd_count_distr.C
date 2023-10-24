void thd_count_distr()
{
  TFile* file = new TFile("input/12800us_2014_64987ev.root", "READ");
  if (file->IsZombie()) {
    cerr << "Can't read a file!" << endl;
    return;
  }

  TTree* tree = (TTree*)file->Get("HERO");
  if (!tree) {
    cerr << "Can't find a tree!" << endl;
    return;
  }

  Int_t binN = 13000;
  Double_t binMin = 0.;
  Double_t binMax = (Double_t)binN;
  TH1F* thd_h = new TH1F("thd_h", "thd_h", binN, binMin, binMax);

  tree->Draw("t*0.001 >> thd_h", "pdg == 1000020040");

/*
  TH1F *new_h = new TH1F("normed", "normed", binN, binMin, binMax);
  new_h->SetTitle("alpha threshold distribution per 1 [usec] in range the [0:128] [usec]");
  new_h->GetXaxis()->SetTitle("time moment [nanosec]");
  new_h->GetYaxis()->SetTitle("counts");

  Double_t numOfPrimaryEvents = 2505.;
  Double_t numOfPrimaryEventsIn1pre16sec = 2505.;
  Double_t norm = numOfPrimaryEventsIn1pre16sec / numOfPrimaryEvents;

  for (Int_t i=0; i<binN; i++) {
    Double_t oldBinContent = (Double_t)thd_h->GetBinContent(i);
    Double_t newBinContent = oldBinContent * norm;
    new_h->SetBinContent(i, newBinContent);
  }

  new_h->Draw();
*/

  TTree* thd_count_tree = new TTree("thd_count_tree", "thd_count_tree");
  Int_t thd_count;
  thd_count_tree->Branch("thd_count", &thd_count, "thd_count/I");

  for (Int_t i=11; i<12800; i++) {
    thd_count = thd_h->GetBinContent(i);
    cout << thd_count << endl;
    thd_count_tree->Fill();
  }

  TCanvas* canv = new TCanvas("canv", "canv");
  thd_count_tree->Draw("thd_count");
}
