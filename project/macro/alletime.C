Bool_t time(TH1F*, Int_t index);
void alletime()
{
  Int_t binN = 1000;
  TH1F *histo = new TH1F("histo", "histo", binN, 0., 65000.);

  for (Int_t i=1; i<71; i++) {
    if ( !time(histo, i) ) return;
  }

  histo->Draw();
  gPad->SetGrid(2,2);
  histo->SetTitle("alpha distribution. 1/16 sec. pE[0.082 : 20.76] GeV");
  histo->GetXaxis()->SetTitle("(1bin-3usec)time moment [usec]");
  histo->SetLineWidth(3);
  histo->SetLineColor(1);

}
// ----------------------------------------------------------
Bool_t time(TH1F *histo, Int_t index)
{
  TString inFileName;
  inFileName.Form("../archive/0.0625sec/hero_%d.root", index);
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
  std::map<int, int> mapEvId;
  for (Int_t i=0; i<nEntries; i++) {
    //cout << i << endl;
    tree->GetEntry(i);
    mapEvId[evId]++;
    //cout << evId << ", " << time << endl;
    // 2112
    // 1000020040
    if (pdg == 1000020040) // only for alpha
      histo->Fill(time*1e-3);
  }
  std::map<int, int> :: iterator it=mapEvId.end();
  cout << index << ": nEvents: " << it->first << ", nEntries=" << nEntries << endl;
  return kTRUE;
}

