void Icalc()
{
  fstream fin("../../input/IntPam2009.txt");
  if (!fin.is_open()) {
    cerr << "Can't open IntPam2009.txt!" << endl;
    return;
  }

  // Read and out the file's header
  TString info="";
  for (Int_t i=0; i<7; i++) { fin >> info; cout << info << " "; } cout << endl;

  Double_t I=0.;
  Double_t prevE=0.;
  while (!fin.eof()) {
    Double_t curE; // GeV
    Double_t curFlux; // particles/m^2 sr s
    fin >> curE >> curFlux;
    I += (curE-prevE)*curFlux;
    if (fin.eof()) break;
    //cout << curE-prevE << ", " << curFlux << endl;
    prevE=curE;
  }

  cout << "Integral=" << I << endl;
  fin.close();
}
