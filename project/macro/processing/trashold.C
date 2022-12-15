Bool_t readAndFill(TH1F*);
void trashold()
{
  Int_t binN = 130;
  Double_t maxBinV = 130.;
  TH1F *histo = new TH1F("histo", "histo", binN, 0., maxBinV);

  readAndFill(histo);

  histo->Draw();
  gPad->SetGrid(2,2);
  histo->SetTitle("alpha edep vs. R ; proton E = 20.76 GeV");
  histo->GetXaxis()->SetTitle("R [cm]");
  histo->GetYaxis()->SetTitle("edep [MeV]");
  histo->SetLineWidth(3);
  histo->SetLineColor(1);

}

Bool_t readAndFill(TH1F *histo)
{
  TString inFileName = "../output/hero.root";
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
  Double_t radius;
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("deposit_e", &edep);
  tree->SetBranchAddress("radius", &radius);
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
    if (pdg != 1000020040) { // only for alpha
      Int_t curBinId = 125-((Int_t)(1250./radius)-1); // 1 bin = 1 cm
      Double_t curBinContent = histo->GetBinContent(curBinId);
      curBinContent+=edep;
      //cout << radius << ", " << curBinId << ", " << curBinContent << endl;
      histo->SetBinContent(curBinId, curBinContent);
    }
  }
  std::map<int, int> :: iterator it=mapEvId.end();
  cout << "nEvents: " << it->first << ", nEntries=" << nEntries << endl;
  return kTRUE;
}
