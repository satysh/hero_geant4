void bindrootfiles()
{
  fstream fin(".files_list.txt");
  if (!fin.is_open()) {
    cerr << "Can't find .files_list.txt!" << endl;
    return;
  }

  std::vector<Int_t> vEvId;
  std::vector<Double_t> vTime;
  while(!fin.eof()) {
    TString fileName;
    fin >> fileName;
    if (fileName == "") continue; // skip empty strings
    //cout << fileName << endl;
    TFile *file = new TFile(fileName, "READ");
    if (file->IsZombie()) {
      cerr << "Can't read " << fileName << "!" << endl;
      return;
    }
    Int_t curEvId;
    Double_t curTime;
    TTree *curTree = (TTree*)file->Get("HERO");
    if (!curTree) {
      cerr << "Can't find tree in " << fileName << " file!" << endl;
      return;
    }
    curTree->SetBranchAddress("eventId", &curEvId);
    curTree->SetBranchAddress("t", &curTime);
    Int_t nEntries = curTree->GetEntries();
    cout << fileName << " has " << nEntries << " entries!" << endl;
    for (Int_t i=0; i<nEntries; i++) {
      curTree->GetEntry(i);
      vEvId.push_back(curEvId);
      vTime.push_back(curTime);
    }
    delete curTree;
    file->Close();
  }

  TFile *file = new TFile("hero.root", "RECREATE");
  Int_t evId;
  Double_t time;
  TTree *tree = new TTree("bindHERO", "data");
  tree->Branch("eventId", &evId);
  tree->Branch("time", &time);

  cout << "Resulting entries number is " << vEvId.size() << endl;
  for (Int_t i=0; i<vEvId.size(); i++) {
    evId=vEvId[i];
    time=vTime[i];
    tree->Fill();
  }
  tree->Write();
}
