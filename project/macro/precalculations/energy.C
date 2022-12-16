TGraph* energyCDFGr = NULL;
TGraph* energyInvCDFGr = NULL;

Double_t EnergyCDF(Double_t *x, Double_t *par) {
  return energyCDFGr->Eval(x[0]);
}

Double_t EnergyInvCDF(Double_t *x, Double_t *par) {
  return energyInvCDFGr->Eval(x[0]);
}

void energy()
{
  ifstream fin("../../input/IntPam2009.txt");
  if (!fin.is_open()) {
    cerr << "Can't open IntPam2009.txt!" << endl;
    return;
  }
  TString info="";
  for (Int_t i=0; i<7; i++) { fin >> info; cout << info << " "; } cout << endl;

  TVectorD energy(71);
  TVectorD flux(71);
  Int_t i=0;
  while (!fin.eof()) {
    Double_t curE; // GeV
    Double_t curFlux; // particles/m^2 sr s
    fin >> curE >> curFlux;
    energy(i) = curE;
    flux(i) = curFlux;
    if (fin.eof()) break;
    //cout << curE << ", " << curFlux << endl;
    i++;
  }
  fin.close();

  energyCDFGr = new TGraph(energy, flux);
  energyInvCDFGr = new TGraph(flux, energy);

  TF1* energyCDF = new TF1("thetaCDF", EnergyCDF, 0.082, 20.76, 0);
  TF1* energyInvCDF = new TF1("thetaInvCDF", EnergyInvCDF, 80.73, 7808.99, 0);
  Double_t param = 6600.23;
  Double_t res = energyInvCDF->Eval(param);
  cout << res << endl;
  cout << energyCDF->Eval(res) << endl;


  TH1F* hist = new TH1F("hist", "hist", 1000, 0., 21.);
  TRandom rnd;
  for (Int_t i=0; i<10000000; i++) {
    Double_t rndflux = rnd.Uniform(80.73, 7808.99);
    Double_t val = energyInvCDF->Eval(rndflux);
    hist->Fill(val);

  }


  TCanvas* canv = new TCanvas("canv", "canv");
  //canv->Divide(2, 1);
  //canv->cd(1);
  energyCDF->Draw();
  //energyInvCDF->Draw();
  //energyCDFGr->Draw();
  //energyInvCDFGr->Draw();
  //canv->cd(2);
  hist->Draw();

}
