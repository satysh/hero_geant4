void drawTime()
{
  TFile *file = new TFile("input/10TeV.root", "READ");
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
  TH1F *h = new TH1F("h", "h", 1000, 31200., 31400.);
  tree->Draw("t*0.001 >> h", "pdg == 1000020040");
}
