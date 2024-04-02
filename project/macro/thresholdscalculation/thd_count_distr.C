// Максрос для построения распределения фоновго счета
// из гистограммы фоновго счета

void thd_count_distr()
{
  TFile* file = new TFile("output/background/hists/hists_background_2014_207512_ev_1_sec.root", "READ");
  if (file->IsZombie()) {
    cerr << "Can't read a file!" << endl;
    return;
  }


  TH1F* thd_h = (TH1F*)file->Get("a_b_hist"); // a is only alpha, but b_hist for all particles
  if (!thd_h) {
    cerr << "Can't find hist!" << endl;
    return;
  }

  cout << "hist info:\n";
  Int_t nbins = thd_h->GetNbinsX();
  cout << "nbins is " << nbins << endl;
  cout << "left board bin content is " << thd_h->GetBinContent(1) << ", right one is " << thd_h->GetBinContent(nbins - 1) << endl;
  cout << "left board bin center is " << thd_h->GetBinCenter(1) << ", right one is " << thd_h->GetBinCenter(nbins - 1) << endl;

  TFile *tmp_file = new TFile("tmp.root", "RECREATE");
  TTree* thd_count_tree = new TTree("thd_count_tree", "thd_count_tree");
  Int_t thd_count;
  thd_count_tree->Branch("thd_count", &thd_count, "thd_count/I");

  Int_t z_cnt = 0;
  for (Int_t i=0; i<nbins; i++) {
    thd_count = thd_h->GetBinContent(i);
    //cout << thd_count << endl;
    //if (thd_count != 0) // skip zeros
      thd_count_tree->Fill();
    //else {
    //  z_cnt++;
    //}

  }

  cout << "cnt of zeros is " << z_cnt << endl;
  TCanvas* canv = new TCanvas("canv", "canv");
  thd_count_tree->Draw("thd_count");
}
