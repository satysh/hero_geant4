void alphaMeanT()
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
  TH1F *histo = new TH1F("histo", "histo", binN, 0., 5.);
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
    histo->Fill(meanTime);
  }
  cout << "minMean=" << minMean << ", maxMean=" << maxMean << endl;
  histo->Draw();
  gPad->SetGrid(2,2);
  histo->SetTitle("mean time moment distribution for alpha particles");
  histo->GetXaxis()->SetTitle("time moment [usec]");
  histo->SetLineWidth(3);
  histo->SetLineColor(2);
}
