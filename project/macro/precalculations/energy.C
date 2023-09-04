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
  ifstream fin("../../input/IntPam2010.txt");
  if (!fin.is_open()) {
    cerr << "Can't open IntPam2009.txt!" << endl;
    return;
  }

  Int_t nPoints = std::count(std::istreambuf_iterator<char>(fin),
                               std::istreambuf_iterator<char>(), '\n');
  fin.seekg(0, std::ios::beg);

  TString info="";
  for (Int_t i=0; i<7; i++) { fin >> info; cout << info << " "; } cout << endl;

  nPoints -= 1;
  cout << "nPoints=" << nPoints << endl;
  TVectorD energy(nPoints);
  TVectorD flux(nPoints);
  Int_t i=0;
  Double_t fluxS = 0.;
  while (!fin.eof()) {
    Double_t curE; // GeV
    Double_t curFlux; // particles/m^2 sr s
    fin >> curE >> curFlux;
    energy(i) = curE;
    flux(i) = curFlux;  // 15175.
    fluxS += curFlux;
    if (fin.eof()) break;
    //cout << curE << ", " << curFlux << endl;
    i++;
  }
  fin.close();

  Double_t fMinFlux = flux(nPoints - 1);
  Double_t fMaxFlux = flux(0);

  cout << "min max" << fMinFlux << ", " << fMaxFlux << endl;
  TGraph* tmpGr = new TGraph(flux, energy);
  for (Int_t i=0; i<nPoints; i++) {
    flux(i) = flux(i) / tmpGr->Integral();
  }

  energyCDFGr = new TGraph(energy, flux);
  energyInvCDFGr = new TGraph(flux, energy);


  cout << "energyCDFGr.Integral = " << energyCDFGr->Integral() << endl;
  cout << "energyInvCDFGr.Integral = " << energyInvCDFGr->Integral() << endl;

  TF1* energyCDF = new TF1("thetaCDF", EnergyCDF, 0.082, 44.37, 0);
  TF1* energyInvCDF = new TF1("thetaInvCDF", EnergyInvCDF, 0., 1., 0);
  Double_t param = 80.73;
  Double_t res = energyInvCDF->Eval(param);
  cout << res << endl;
  cout << energyCDF->Eval(res) << endl;

  cout << "energyCDF.Integral = " << energyCDF->Integral(0., 44.37) << endl;
  cout << "energyInvCDF.Integral = " << energyInvCDF->Integral(0., 1.) << endl;


  TH1F* hist = new TH1F("hist", "hist", 5000, 0., 50.);
  TRandom rnd;
  Int_t nEvents = 10000000;
  for (Int_t i=0; i<nEvents; i++) {
    Double_t rndflux = rnd.Uniform();
    Double_t val = energyInvCDF->Eval(rndflux);
    hist->Fill(val);
  }

  Double_t binSum = 0.;
  for (Int_t i=0; i<hist->GetNbinsX(); i++) {
    binSum += hist->GetBinContent(i);
  }

  Double_t Integral = hist->Integral();
  cout << Integral << endl;
  for (Int_t i=0; i<hist->GetNbinsX(); i++) {
    hist->SetBinContent(i, hist->GetBinContent(i) / (Double_t)nEvents / 0.01);
  }

  cout << "hist->Integral() = " << hist->Integral() << endl;
  TCanvas* canv = new TCanvas("canv", "canv");
  //canv->Divide(2, 1);
  //canv->cd(1);
  //energyInvCDF->Draw();
  energyCDF->Draw();
  //energyCDFGr->Draw();
  //energyInvCDFGr->Draw();
  //canv->cd(2);
/*
  TFile* file = new TFile("../../hero.root", "READ");

  TTree* tree = (TTree*)file->Get("HERO");

  tree->Draw("kin_E/1000","pdg==2212 && trackID==1 && pre_z == -1250 && kin_E > 500","");
  TH1F* htemp = (TH1F*)gPad->FindObject("htemp");


  for (Int_t i=0; i<htemp->GetNbinsX(); i++) {
    htemp->SetBinContent(i, htemp->GetBinContent(i) / 6993.);
  }
*/
  hist->Draw("SAME");
}
