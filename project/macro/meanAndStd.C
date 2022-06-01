void getMeanAndStd(Double_t&, Double_t&, Int_t index);
void meanAndStd()
{
  Int_t nEnergies = 70;
  //TString opt = "mean";
  TString opt = "std";
  TVectorD vX(nEnergies);
  fstream fin("../input/IntPam2009.txt");
  if (!fin.is_open()) {
    cerr << "Can't open IntPam2009.txt!" << endl;
    return;
  }
  TString info="";
  for (Int_t i=0; i<7; i++) { fin >> info; }

  Int_t it = 0;
  while (!fin.eof()) {
    Double_t curE; // GeV
    Double_t curFlux; // particles/m^2 sr s
    fin >> curE >> curFlux;
    if (fin.eof()) break;
    vX(it) = curE;
    it++;
    if (it >= nEnergies) break;
  }
/*
  for (Int_t i=0; i<vX.size(); i++) {
    cout << vX[i] << endl;
  }
*/

  TVectorD vY(nEnergies);
  for (Int_t i=0; i<nEnergies; i++) {
    Double_t curMean = 0.;
    Double_t curStd = 0.;
    getMeanAndStd(curMean, curStd, i+1);
    if (opt == "mean")
      vY(i) = curMean;
    else
      vY(i) = curStd;
  }

  TGraph *graph = new TGraph(vX, vY);
  graph->Draw("AL*");
  if (opt == "mean")
    graph->SetTitle("meanTime(E)");
  else
    graph->SetTitle("StdDevOfTime(E)");
  graph->SetMarkerStyle(8);
  graph->SetMarkerColor(2);
  graph->GetXaxis()->SetTitle("E [GeV]");
  if (opt == "mean")
    graph->GetYaxis()->SetTitle("meanTime moment for alpha [usec]");
  else
    graph->GetYaxis()->SetTitle("StdDev of the mean time moment for alpha [usec]");
}

void getMeanAndStd(Double_t &curMean, Double_t &curStd, Int_t index)
{
  TString inFileName;
  inFileName.Form("../archive/withouttimespread/hero_%d.root", index);
  TFile *file = new TFile(inFileName, "READ");
  if (file->IsZombie()) {
    cerr << "Can't open input file " << inFileName << "!" << endl;
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
  for (Int_t i=0; i<nEntries; i++) {
    tree->GetEntry(i);
    //cout << evId << ", " << time << endl;
    if (pdg == 1000020040) // only for alpha
        mapEvAlpha[evId].push_back(time);
  }

  delete tree;
  file->Close();

  Int_t binN = 250;
  TH1F *histo = new TH1F("histo", "histo", binN, 0., 16.);
  std::map <int, std::vector<double>> :: iterator it = mapEvAlpha.begin();
  for (; it != mapEvAlpha.end(); it++) {
    //cout << it->first << ", " << it->second.size() << endl;
    Double_t meanTime = 0.;
    for (Int_t i=0; i<it->second.size(); i++) {
      meanTime += it->second[i];
    }
    meanTime /= it->second.size();
    meanTime *= 0.001; // to usec
    histo->Fill(meanTime);
  }
  curMean = histo->GetMean();
  curStd = histo->GetStdDev();
  cout << index << ": curMean=" << curMean << ", curStd=" << curStd << ", nEntries=" << nEntries << endl;

  delete histo;
}
