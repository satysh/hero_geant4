void graph()
{
  fstream fin("../../input/IntPam2009.txt");
  if (!fin.is_open()) {
    cerr << "Can't open IntPam2009.txt!" << endl;
    return;
  }
  TString info="";
  for (Int_t i=0; i<7; i++) { fin >> info; cout << info << " "; } cout << endl;

  TVectorD x(71);
  TVectorD y(71);
  Int_t i=0;
  while (!fin.eof()) {
    Double_t curE; // GeV
    Double_t curFlux; // particles/m^2 sr s
    fin >> curE >> curFlux;
    x(i) = curE;
    y(i) = curFlux;
    if (fin.eof()) break;
    //cout << curE << ", " << curFlux << endl;
    i++;
  }
  fin.close();

  TGraph *gr = new TGraph(x,y);
  gr->Draw("AC");
  gr->SetLineWidth(3);
  gr->SetLineColor(2);
  //cout << gr->Integral(0, 70)*(TMath::Pi()*1.25)*(TMath::Pi()*1.25) << endl;
  gr->SetTitle("flux(E)");
  gr->GetXaxis()->SetTitle("E [GeV]");
  gr->GetYaxis()->SetTitle("flux [particles/m^2 sr s]");
  cout << "Integral_1=" << gr->Integral(0, 71) << endl;

  Double_t dE = 0.00000001;
  Double_t E=0.082;
  Double_t Eend=20.76;
  Double_t I=0.;
  while (E<=Eend) {
    I += gr->Eval(E)*dE;
    E += dE;
  }
  cout << "Integral_2=" << I << endl;
}
