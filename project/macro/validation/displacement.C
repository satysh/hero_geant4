void displacement()
{
  TFile* file = new TFile("input/hero.root", "READ");

  if (file->IsZombie()) {
    cerr << "Can't open file!" << endl;
    return;
  }

  Int_t pdg = 1000020040; // alpha

  TTree* tree = (TTree*)file->Get("HERO");

  if (!tree) {
    cerr << "Can't find tree!" << endl;
    return;
  }

  TString post_r = "TMath::Sqrt(post_x*post_x + post_y*post_y + post_z*post_z)";
  TString pre_r = "TMath::Sqrt(pre_x*pre_x + pre_y*pre_y + pre_z*pre_z)";
  tree->Draw("TMath::Abs(" + post_r + "-" + pre_r + "):track_time","pdg == 1000020040", "COLZ");
}
