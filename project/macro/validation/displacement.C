void displacement()
{
  TFile* file = new TFile("../thresholdscalculation/input/128us_2010.root", "READ");

  if (file->IsZombie()) {
    cerr << "Can't open file!" << endl;
    return;
  }

  //1000020040; // alpha

  TTree* tree = (TTree*)file->Get("HERO");

  if (!tree) {
    cerr << "Can't find tree!" << endl;
    return;
  }

  //TString dis = "(post_x-pre_x)*(post_x-pre_x) + (post_y-pre_y)*(post_y-pre_y) + (post_z-pre_z)*(post_z-pre_z)";
  //tree->Draw("TMath::Sqrt(" + dis + ")", "pdg == 2112", "");
  //tree->Draw("TMath::Sqrt(" + dis + ")", "pdg == 2112 && TMath::Sqrt(" + dis + ") > 300", "");
  //tree->Draw("pre_y/10:pre_z/10", "pdg == 2112 && TMath::Sqrt(" + dis + ") > 290", "");
  //tree->Draw("post_y/10:post_z/10:post_x/10", "pdg == 2112 && TMath::Sqrt(" + dis + ") > 250", "SAME");

  Int_t pdg;
  Double_t prex, postx;
  Double_t prey, posty;
  Double_t prez, postz;
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("pre_x", &prex);
  tree->SetBranchAddress("pre_y", &prey);
  tree->SetBranchAddress("pre_z", &prez);
  tree->SetBranchAddress("post_x", &postx);
  tree->SetBranchAddress("post_y", &posty);
  tree->SetBranchAddress("post_z", &postz);

  TMultiGraph* mg = new TMultiGraph();
  Int_t cnt = 0;
  for (Int_t i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    Double_t dx = (postx-prex);
    Double_t dy = (posty-prey);
    Double_t dz = (postz-prez);
    Double_t displacement = TMath::Sqrt(dx*dx + dy*dy + dz*dz);
    if (pdg == 2112 && displacement > 295) {
      TGraph* gr = new TGraph();
      gr->SetPoint(0, prez, prey);
      gr->SetPoint(1, postz, posty);
      gr->SetMarkerSize(2);
      gr->SetMarkerStyle(4);
      gr->SetMarkerColor(0);
      TGraph* pbeggr = new TGraph();
      pbeggr->SetPoint(0, prez, prey);
      pbeggr->SetMarkerSize(2);
      pbeggr->SetMarkerStyle(20);
      pbeggr->SetMarkerColor(kRed);
      TGraph* pendgr = new TGraph();
      pendgr->SetPoint(0, postz, posty);
      pendgr->SetMarkerSize(2);
      pendgr->SetMarkerStyle(20);
      pendgr->SetMarkerColor(kGreen);
      mg->Add(pbeggr);
      mg->Add(pendgr);
      mg->Add(gr);
      cnt++;
    }
  }

  cout << "cnt=" << cnt << endl;
  TCanvas* c = new TCanvas("c", "c");
  c->cd();
  mg->Draw("APC");
}
