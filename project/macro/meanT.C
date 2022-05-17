void meanT()
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
  Double_t time;
  tree->SetBranchAddress("eventId", &evId);
  tree->SetBranchAddress("time", &time);
  Int_t nEntries = tree->GetEntries();
  cout << "nEntries=" << nEntries << endl;
  for (Int_t i=0; i<nEntries; i++) {
    tree->GetEntry(i);
    //cout << evId << ", " << time << endl;
    mapEvAlpha[evId].push_back(time);
  }

  file->Close();

  TTree *meanTree = new TTree("meanTree", "meanTree");
  Int_t meanTime;
  meanTree->Branch("meanTime", &meanTime);
  std::map <int, std::vector<double>> :: iterator it = mapEvAlpha.begin();
  for (; it != mapEvAlpha.end(); it++) {
    //cout << it->first << ", " << it->second.size() << endl;
    meanTime = 0;
    for (Int_t i=0; i<it->second.size(); i++) {
      meanTime += it->second[i];
    }
    meanTime /= it->second.size();
    //meanTime *= 0.001; // to usec
    meanTree->Fill();
  }
  meanTree->Draw("meanTime*0.001");
  TH1F *hist = (TH1F*)gROOT->FindObject("htemp");
  hist->GetXaxis()->SetTitle("mean time moment [usec]");
  hist->SetTitle("mean time moment for alpha");
  gPad->SetGrid(2,2);
}
