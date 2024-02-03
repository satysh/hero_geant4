//Макрос для расчета числа альфа частиц во 2-ю микросекунду для протонов

TH1F *get_normed_hist(TString inputFileName, Int_t nEvents);

void protons_alpha_count()
{
	TH1F *normed_hist = get_normed_hist("proton_pE_1_nEv_100000.root", 100000);
	cout << normed_hist->GetBinContent(2) << endl;

	normed_hist = get_normed_hist("proton_pE_10_nEv_10000.root", 10000);
	cout << normed_hist->GetBinContent(2) << endl;

	normed_hist = get_normed_hist("proton_pE_20_nEv_5000.root", 5000);
	cout << normed_hist->GetBinContent(2) << endl;

	normed_hist = get_normed_hist("proton_pE_30_nEv_3333.root", 3333);
	cout << normed_hist->GetBinContent(2) << endl;

	normed_hist = get_normed_hist("proton_pE_40_nEv_2500.root", 2500);
	cout << normed_hist->GetBinContent(2) << endl;

	normed_hist = get_normed_hist("proton_pE_50_nEv_2000.root", 2000);
	cout << normed_hist->GetBinContent(2) << endl;

	normed_hist = get_normed_hist("proton_pE_60_nEv_1666.root", 1666);
	cout << normed_hist->GetBinContent(2) << endl;

	normed_hist = get_normed_hist("proton_pE_70_nEv_1428.root", 1428);
	cout << normed_hist->GetBinContent(2) << endl;

}

TH1F* get_normed_hist(TString inputFileName, Int_t nEvents) {
  TFile* file = new TFile("input/" + inputFileName, "READ");
  if (file->IsZombie()) {
    cerr << "Can't read file:" << inputFileName << endl;
    return NULL;
  }
  else {
    //cout << inputFileName << " is processing" << endl;
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