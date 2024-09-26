  TH1F* get_normed_hist(TString inputFileName="", Int_t nEvents=0);

void diff_particles_alpha_count()
{

  TLegend* legend = new TLegend(0.1,0.7,0.48,0.9);
  legend->SetHeader("","C"); // option "C" allows to center the header
  TH1F* normed_hist = get_normed_hist("output/fixedstarttime/alpha_Fe_560_GeV_100_events.root", 100);

  if (!normed_hist) return;
  cout << "10 GeV/n Fe56 2usec alhpa counts: " << normed_hist->GetBinContent(2) << endl;

  TCanvas* canv = new TCanvas("canv", "canv");
  canv->cd();
  normed_hist->Draw();
  normed_hist->SetStats(0);
  normed_hist->SetLineWidth(5);
  normed_hist->SetLineColor(1);
  normed_hist->SetLineStyle(1);
  normed_hist->SetTitle("Mean alpha counts per 1 primary particle(1 bin ~ 1 usec)");
  normed_hist->GetXaxis()->SetTitle("time moment [usec]");
  normed_hist->GetYaxis()->SetTitle("alpha counts");
  legend->AddEntry(normed_hist,"Fe ion","l");

/*  normed_hist = get_normed_hist("output/fixedstarttime/alpha_Fe_56_GeV_1000_events.root", 1000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(3);
  normed_hist->SetLineColor(2);
  normed_hist->SetLineStyle(9);
  legend->AddEntry(normed_hist,"Fe ion E=1 GeV/n","l");

  cout << "1 GeV/n Fe56 2usec alhpa counts: " << normed_hist->GetBinContent(2) << endl;
*/
  normed_hist = get_normed_hist("output/fixedstarttime/alpha_helium_40_GeV_10000_events.root", 10000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(5);
  normed_hist->SetLineColor(1);
  normed_hist->SetLineStyle(2);
  legend->AddEntry(normed_hist,"He4 ion","l");

  //cout << "10 GeV/n He4 2usec alhpa counts: " << normed_hist->GetBinContent(2) << endl;

/*
  normed_hist = get_normed_hist("output/fixedstarttime/alpha_helium_4_GeV_10000_events.root", 10000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(3);
  normed_hist->SetLineColor(3);
  normed_hist->SetLineStyle(9);
  legend->AddEntry(normed_hist,"He4 ion E=1 GeV/n","l");

  cout << "1 GeV/n He4 2usec alhpa counts: " << normed_hist->GetBinContent(2) << endl;

  normed_hist = get_normed_hist("output/fixedstarttime/alpha_helium3_30_GeV_1000_events.root", 1000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(3);
  normed_hist->SetLineColor(6);
  legend->AddEntry(normed_hist,"He3 ion E=10 GeV/n","l");

  cout << "10 GeV/n He3 2usec alhpa counts: " << normed_hist->GetBinContent(2) << endl;

  normed_hist = get_normed_hist("output/fixedstarttime/alpha_helium3_3_GeV_1000_events.root", 1000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(3);
  normed_hist->SetLineColor(6);
  normed_hist->SetLineStyle(9);
  legend->AddEntry(normed_hist,"He3 ion E=1 GeV/n","l");

  cout << "1 GeV/n He3 2usec alhpa counts: " << normed_hist->GetBinContent(2) << endl;
*/

  normed_hist = get_normed_hist("output/fixedstarttime/alpha_proton_10_GeV_10000_events.root", 10000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(5);
  normed_hist->SetLineColor(1);
  normed_hist->SetLineStyle(8);
  legend->AddEntry(normed_hist,"proton","l");

/*
  cout << "10 GeV/n proton 2usec alhpa counts: " << normed_hist->GetBinContent(2) << endl;

  normed_hist = get_normed_hist("output/fixedstarttime/alpha_proton_1_GeV_10000_events.root", 10000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(3);
  normed_hist->SetLineColor(4);
  normed_hist->SetLineStyle(9);
  legend->AddEntry(normed_hist,"proton E=1 GeV/n","l");

  cout << "1 GeV/n proton 2usec alhpa counts: " << normed_hist->GetBinContent(2) << endl;
*/
  normed_hist = get_normed_hist("output/fixedstarttime/alpha_e_10_GeV_10000_events.root", 10000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(5);
  normed_hist->SetLineColor(1);
  normed_hist->SetLineStyle(10);
  legend->AddEntry(normed_hist,"gamma","l");

/*
  cout << "10 GeV/n e- 2usec alhpa counts: " << normed_hist->GetBinContent(2) << endl;

  normed_hist = get_normed_hist("output/fixedstarttime/alpha_e_1_GeV_10000_events.root", 10000);
  canv->cd();
  normed_hist->Draw("SAME");
  normed_hist->SetLineWidth(3);
  normed_hist->SetLineColor(1);
  normed_hist->SetLineStyle(9);
  legend->AddEntry(normed_hist,"e- E=1 GeV","l");

  cout << "1 GeV/n e- 2usec alhpa counts: " << normed_hist->GetBinContent(2) << endl;
*/
  legend->SetHeader("primary E=10 [Gev/n]");
  legend->Draw();
}

TH1F* get_normed_hist(TString inputFileName, Int_t nEvents) {
  TFile* file = new TFile(inputFileName, "READ");
  if (file->IsZombie()) {
    cerr << "Can't read file:" << inputFileName << endl;
    return NULL;
  }
  else {
    cout << inputFileName << " is processing" << endl;
  }

  TTree* tree = (TTree*)file->Get("HERO");
  if (!tree) {
    tree = (TTree*)file->Get("alpha_tree");
    if (!tree) {
      cerr << "Can't find tree!" << endl;
      return NULL;
    }
  }

  Int_t binN = 50;
  Double_t minBin = 0.;
  Double_t maxBin = 50.;
  TH1F* hist = new TH1F("hist", "hist", binN, minBin, maxBin);
/*
  Double_t t;
  tree->SetBranchAddress("t", &t);
  for (Int_t i=0; i<tree->GetEntries(); i++) {
    tree->GetEntry(i);
    hist->Fill(t * 0.001); // to usec
  }
*/
  TCanvas* tmp = new TCanvas("tmp", "tmp");
  tmp->cd();
  tree->Draw("t*0.001 >> hist", "pdg==1000020040 && t < 128000");
  TH1F* normed_hist = new TH1F("normed_hist", "normed_hist", binN, minBin, maxBin);

  for (Int_t i=0; i<binN; i++) {
    normed_hist->SetBinContent(i, hist->GetBinContent(i) / nEvents);
  }

  delete tmp;
  return normed_hist;
}
