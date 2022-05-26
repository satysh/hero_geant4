Bool_t radius_i(TH1F*, Int_t index);
void radius_draw()
{
  Int_t binN = 130;
  TH1F *histo = new TH1F("histo", "histo", binN, 0., 130.);

  for (Int_t i=1; i<71; i++) {
    if ( !radius_i(histo, i) ) return;
  }

  histo->Draw();
  gPad->SetGrid(2,2);
  histo->SetTitle("radius distribution. 128 usec. pE[0.082 : 20.76] GeV");
  histo->GetXaxis()->SetTitle("radius [cm]");
  histo->SetLineWidth(3);
  histo->SetLineColor(6);
}
// -------------------------------------------------------------------------------------
Bool_t radius_i(TH1F *histo, Int_t index)
{
  TString inFileName;
  inFileName.Form("../archive/128usec/radius/hero_%d.root", index);
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
  Double_t radius;
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("radius", &radius);
  Int_t nEntries = tree->GetEntries();
  std::map<int, int> mapEvId;
  for (Int_t i=0; i<nEntries; i++) {
    //cout << i << endl;
    tree->GetEntry(i);
    mapEvId[evId]++;
    //cout << evId << ", " << time << endl;
    // 2112
    // 1000020040
    if (pdg == 1000020040) // only for alpha
      histo->Fill(radius*1e-1);
  }
  std::map<int, int> :: iterator it=mapEvId.end();
  cout << index << ": nEvents: " << it->first << ", nEntries=" << nEntries << endl;
  return kTRUE;
}
