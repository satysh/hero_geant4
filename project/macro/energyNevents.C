void getEandNEvents(Int_t i, Double_t &enrgy, Int_t &nevents)
{
  fstream fin("../input/IntPam2009.txt");
  if (!fin.is_open()) {
    cerr << "Can't open IntPam2009.txt!" << endl;
    return;
  }
  TString info="";
  for (Int_t i=0; i<7; i++) { fin >> info; }

  Double_t pi = TMath::Pi();
  Double_t detectorR = 125.*0.01; // m
  Double_t time = 1./16.; // sec
  Double_t detectorS = 4.*pi*detectorR*detectorR; // m2
  Int_t it=0 ;
  while (!fin.eof()) {
    Double_t curE; // GeV
    Double_t curFlux; // particles/m^2 sr s
    fin >> curE >> curFlux;
    Double_t curNevents = curFlux*detectorS*4.*pi*time;
    if (it == i) {
      enrgy=curE; nevents=curNevents;
      return;
    }
    it++;
  }
}
void energyNevents()
{
  fstream fin("../input/IntPam2009.txt");
  if (!fin.is_open()) {
    cerr << "Can't open IntPam2009.txt!" << endl;
    return;
  }
  TString info="";
  for (Int_t i=0; i<7; i++) { fin >> info; cout << info << " "; } cout << endl;

  Double_t pi = TMath::Pi();
  Double_t detectorR = 125.*0.01; // m
  Double_t time = 1./16.; // sec
  Double_t detectorS = 4.*pi*detectorR*detectorR; // m2
  cout << "pi=" << pi << ", detectorR=" << detectorR << " [m] , detectorS=" << detectorS
       << " [m2], time=" << time << " [sec]" << endl;
  Int_t it=0 ;
  while (!fin.eof()) {
    Double_t curE; // GeV
    Double_t curFlux; // particles/m^2 sr s
    fin >> curE >> curFlux;
    //cout << curE << ", " << curFlux << endl;
    Double_t curNevents = curFlux*detectorS*4.*pi*time;
    cout << it << ": E=" << curE << " [GeV], nEvents=" << curNevents << endl;
    it++;
  }
  fin.close();
  cout << it << endl;
  Double_t exampleE = 0.;
  Int_t exampleNev = 0;
  Int_t i = 55;
  getEandNEvents(i, exampleE, exampleNev);
  cout << i << ": " << exampleE << ", " << exampleNev << endl;
}
