Double_t fullDepositE = 0.;
Bool_t getdEAlpha(TH1F*, Int_t index);

void depositEAlphaCount()
{
  Int_t binN = 100;
  Double_t maxBinVal = 20.;
  TH1F *histo = new TH1F("histo", "histo", binN, 0., maxBinVal);

  for (Int_t i=1; i<71; i++) {
    if ( !getdEAlpha(histo, i) ) return;
  }

  histo->Draw();
  gPad->SetGrid(2,2);
  histo->SetTitle("deposit E for alpha distribution. bor 12% 128 usec. pE[0.082 : 20.76] GeV");
  histo->GetXaxis()->SetTitle("depositE [MeV]");
  histo->SetLineWidth(3);
  histo->SetLineColor(1);

  cout << "fullDepositE=" << fullDepositE << endl;
}

Bool_t getdEAlpha(TH1F *histo, Int_t index)
{
  TString inFileName;
  inFileName.Form("../archive/bor_3prec_128usec/hero_%d.root", index);
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

  Int_t pdg;
  Double_t depE;
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("deposit_e", &depE);
  Int_t nEntries = tree->GetEntries();
  cout << index << ": nEntries=" << nEntries << endl;
  for (Int_t i=0; i<nEntries; i++) {
    tree->GetEntry(i);
    // 2112
    // 1000020040
    if (pdg == 1000020040) // only for alpha
      histo->Fill(depE); // MeV
      fullDepositE += depE;
  }

  return kTRUE;
}
