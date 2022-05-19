void time()
{
  TFile *file = new TFile("../output/hero.root", "READ");
  if (file->IsZombie()) {
    cerr << "Can't open input file!" << endl;
    return;
  }

  TTree *tree = (TTree*)file->Get("bindHERO");
  if (!tree) {
    cerr << "Can't find input tree!" << endl;
    return;
  }

  Int_t evId;
  Int_t pdg;
  Double_t time;
  Int_t binN = 125;
  TH1F *histo = new TH1F("histo", "histo", binN, 0., 1e6/15.);
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("time", &time);

  Int_t nEntries = tree->GetEntries();
  cout << "nEntries=" << nEntries << endl;
  for (Int_t i=0; i<nEntries; i++) {
    //cout << i << endl;
    tree->GetEntry(i);
    //cout << evId << ", " << time << endl;
    // 2112
    // 1000020040
    if (pdg == 1000020040) // only for alpha
      histo->Fill(time*1e-3);
  }

  histo->Draw();
  //tree->Draw("time >> histo", "pdg == 1000020040");
  gPad->SetGrid(2,2);
  histo->SetTitle("alpha distribution");
  histo->GetXaxis()->SetTitle("time moment [usec]");
  histo->SetLineWidth(3);
  histo->SetLineColor(1);
}
