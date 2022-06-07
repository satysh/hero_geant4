Bool_t readAndFill(TH1F*, Int_t index);
void sumedep()
{
  Int_t binN = 1000;
  Double_t maxBinV = 65000.;
  TH1F *histo = new TH1F("histo", "histo", binN, 0., maxBinV);

  for (Int_t i=1; i<71; i++) {
    if ( !readAndFill(histo, i) ) return;
  }

  histo->Draw();
  gPad->SetGrid(2,2);
  histo->SetTitle("alpha distribution. 1/16 sec. pE[0.082 : 20.76] GeV");
  histo->GetXaxis()->SetTitle("(1bin-3usec)time moment [usec]");
  histo->GetYaxis()->SetTitle("edep [MeV]");
  histo->SetLineWidth(3);
  histo->SetLineColor(1);

}

Bool_t readAndFill(TH1F *histo, Int_t index)
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
  Double_t edep;
  Double_t time;
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("deposit_e", &edep);
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
    if (pdg == 1000020040) { // only for alpha
      Double_t curTime = time*1e-3;
      Int_t curBinId = (Int_t)curTime/65.; // 1 bin = 4 usec
      Double_t curBinContent = histo->GetBinContent(curBinId);
      curBinContent+=edep;
      histo->SetBinContent(curBinId, curBinContent);
    }
  }
  std::map<int, int> :: iterator it=mapEvId.end();
  cout << index << ": nEvents: " << it->first << ", nEntries=" << nEntries << endl;
  return kTRUE;
}
