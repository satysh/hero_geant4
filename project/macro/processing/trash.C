void trash()
{
  Int_t bin_n = 100;
  Double_t maxBinVal = 1.;
  TString inFileName;
  inFileName.Form("/opt/Data/hero/background/512usec/hero_%d.root", index);
  TFile *file = new TFile(inFileName, "READ");
  if (file->IsZombie()) {
    cerr << "Can't open input file " << inFileName << endl;
    return kFALSE;
  }

  TTree *tree = (TTree*)file->Get("bindHERO");
  if (!tree) {
    cerr << "Can't find input tree!" << endl;
    return kFALSE;
  }

  Int_t evId;
  Int_t pdg;
  Double_t time;
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("time", &time);
  Int_t nEntries = tree->GetEntries();
  TH1F *histo = new TH1F("histo", "histo", bin_n, 0., maxBinVal);
  for (Int_t i=0; i<nEntries; i++) {
    tree->GetEntry(i);
    // 2112
    // 1000020040
    if (pdg == 1000020040) // only for alpha
      histo->Fill(time*1e-3); // to usec
  }
}

