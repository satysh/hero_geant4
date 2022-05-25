void numberOfParticles()
{
  TFile *file = new TFile("../archive/E=20.76GeV/hero.root", "READ");
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

  Double_t minN = 1000.;
  Double_t maxN = 0.;
  Int_t binN = 500;
  TH1F *histo = new TH1F("histo", "histo", binN, 0., 500.);
  std::map <int, std::vector<double>> :: iterator it = mapEvAlpha.begin();
  for (; it != mapEvAlpha.end(); it++) {
    //cout << it->first << ", " << it->second.size() << endl;
    Double_t number = (Double_t)it->second.size();
    minN = min(minN, number);
    maxN = max(maxN, number);
    histo->Fill(number);
  }
  cout << "minN=" << minN << ", maxN=" << maxN << endl;
  histo->Draw();
  gPad->SetGrid(2,2);
  histo->SetTitle("The number of alpha particles distribution pE=20.76GeV");
  histo->GetXaxis()->SetTitle("number of alpha in event");
  histo->GetYaxis()->SetTitle("counts");
  histo->SetLineWidth(3);
  histo->SetLineColor(2);
}
