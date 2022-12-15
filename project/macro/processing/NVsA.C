std::vector<Double_t> vNeutron();
std::vector<Double_t> vAlpha();
void NVsA()
{
  std::vector<Double_t> vN = vNeutron();
  std::vector<Double_t> vA = vAlpha();
  if (vA.size() == 0 || vN.size() == 0) return;

  TString title = "";
  Int_t nx = 250;
  Int_t ny = 250;
  TH2D* histo = new TH2D("histo", title.Data(), nx, 0., 5., ny, 0., 5.);
  Int_t nEntries = min(vN.size(), vA.size());
  cout << "nEntries=" << nEntries << endl;
  for (Int_t i=0; i<nEntries; i++) {
    histo->Fill(vN.at(i), vA.at(i));
  }

  histo->GetXaxis()->SetTitle("neutron mean time moment [usec]");
  histo->GetYaxis()->SetTitle("alpha mean time moment [usec]");
  histo->Draw("COLZ");
  //histo->SetStats(kFALSE);
  gPad->SetGrid(2, 2);
}
// -------------------------------------------------
std::vector<Double_t> vNeutron() {
  std::vector <Double_t> vOut;
  TFile *file = new TFile("../output/hero.root", "READ");
  if (file->IsZombie()) {
    cerr << "Can't open input file!" << endl;
    return vOut;
  }

  TTree *tree = (TTree*)file->Get("bindHERO");
  if (!tree) {
    cerr << "Can't find input tree!" << endl;
    return vOut;
  }

  // < evId, vect<time> >
  std::map<int, std::vector<double>> mapEvNeutron;
  Int_t evId;
  Int_t pdg;
  Double_t time;
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("time", &time);
  Int_t nEntries = tree->GetEntries();
  cout << "nEntries=" << nEntries << endl;
  for (Int_t i=0; i<nEntries; i++) {
    tree->GetEntry(i);
    //cout << evId << ", " << time << endl;
    if (pdg == 2112) // only for neutrons
        mapEvNeutron[evId].push_back(time);
  }

  file->Close();

  Double_t minMean = 1000.;
  Double_t maxMean = 0.;
  Int_t binN = 250;
  std::map <int, std::vector<double>> :: iterator it = mapEvNeutron.begin();
  for (; it != mapEvNeutron.end(); it++) {
    //cout << it->first << ", " << it->second.size() << endl;
    Double_t meanTime = 0.;
    for (Int_t i=0; i<it->second.size(); i++) {
      meanTime += it->second[i];
    }
    meanTime /= it->second.size();
    meanTime *= 0.001; // to usec
    minMean = min(minMean, meanTime);
    maxMean = max(maxMean, meanTime);
    vOut.push_back(meanTime);
  }
  cout << "vNeutron: minMean=" << minMean << ", maxMean=" << maxMean << endl;
  return vOut;
}
// ------------------------------------------------------------------------------
std::vector<Double_t> vAlpha() {
  std::vector <Double_t> vOut;
  TFile *file = new TFile("../output/hero.root", "READ");
  if (file->IsZombie()) {
    cerr << "Can't open input file!" << endl;
    return vOut;
  }

  TTree *tree = (TTree*)file->Get("bindHERO");
  if (!tree) {
    cerr << "Can't find input tree!" << endl;
    return vOut;
  }

  // < evId, vect<time> >
  std::map<int, std::vector<double>> mapEvAlpha;
  Int_t evId;
  Int_t pdg;
  Double_t time;
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("time", &time);
  Int_t nEntries = tree->GetEntries();
  cout << "nEntries=" << nEntries << endl;
  for (Int_t i=0; i<nEntries; i++) {
    tree->GetEntry(i);
    //cout << evId << ", " << time << endl;
    if (pdg == 1000020040) // only for alpha
        mapEvAlpha[evId].push_back(time);
  }

  file->Close();

  Double_t minMean = 1000.;
  Double_t maxMean = 0.;
  Int_t binN = 250;
  std::map <int, std::vector<double>> :: iterator it = mapEvAlpha.begin();
  for (; it != mapEvAlpha.end(); it++) {
    //cout << it->first << ", " << it->second.size() << endl;
    Double_t meanTime = 0.;
    for (Int_t i=0; i<it->second.size(); i++) {
      meanTime += it->second[i];
    }
    meanTime /= it->second.size();
    meanTime *= 0.001; // to usec
    minMean = min(minMean, meanTime);
    maxMean = max(maxMean, meanTime);
    vOut.push_back(meanTime);
  }
  cout << "vAlpha: minMean=" << minMean << ", maxMean=" << maxMean << endl;
  return vOut;
}
// -------------------------------------------------
