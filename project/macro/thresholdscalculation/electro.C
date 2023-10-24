void electro()
{
  TFile *file = new TFile("input/128us_2010_2505ev.root", "READ");
  if (file->IsZombie()) {
    cerr << "Can't open input file!" << endl;
    return;
  }

  TTree *tree = (TTree*)file->Get("HERO");
  if (!tree) {
    cerr << "Can't find input tree!" << endl;
    return;
  }

  Int_t pdg;
  Double_t deposit_E;
  Double_t time;
  tree->SetBranchAddress("pdg", &pdg);
  tree->SetBranchAddress("deposit_E", &deposit_E);
  tree->SetBranchAddress("t", &time);


  Int_t binN = 160;
  TH1F* thr = new TH1F("thr", "thr", binN, 0., (Double_t)binN);

  for (Int_t i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if (pdg == 1000020040)
      thr->Fill(time*0.001, deposit_E);
  }

  thr->Draw("hist");
  thr->SetTitle("ionization edep distribution all");
  thr->GetXaxis()->SetTitle("time moment [usec]");
  thr->GetYaxis()->SetTitle("summ deposit_E [MeV]");


  TH1F* hist = new TH1F("hist", "hist", 100, 0., 2000.);

  for (Int_t i=0; i<binN; i++) {
    Int_t thd_count = thr->GetBinContent(i);
    if (thd_count) {
      hist->Fill(thd_count);
    }
  }

  TCanvas* canv = new TCanvas("canv", "canv");
  hist->Draw();
  hist->GetXaxis()->SetTitle("summ edep counts");
  hist->GetYaxis()->SetTitle("frequency");
  hist->SetTitle("background distribution");
}
