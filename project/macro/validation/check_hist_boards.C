void check_hist_boards()
{
	const Int_t nbins = 10;
	Double_t min_bin_val = 0.;
	Double_t max_bin_val = 10.;

	TH1F *h = new TH1F("h", "h", nbins, min_bin_val, max_bin_val);

	TRandom rnd(12345);
	Double_t dict[nbins] = {0};

	UInt_t down = 0;
	UInt_t up = 0;
	for (UInt_t i=0; i<1000; i++) {
		Double_t rnd_val = rnd.Uniform(min_bin_val - 10., max_bin_val + 10.);
		h->Fill(rnd_val);


		if (rnd_val < min_bin_val) {
			//h->SetBinContent(0, h->GetBinContent(0) + 1);
			down ++;
		}
		else if (rnd_val >= max_bin_val) {
			//h->SetBinContent(Int_t(max_bin_val) + 1, h->GetBinContent(Int_t(max_bin_val) + 1) + 1);
			up++;
		}
		else {
			dict[Int_t(rnd_val)] += 1;
		}
	}

	cout << "down = " << down << ", up = " << up << endl;
	cout << "0 bin content = " << h->GetBinContent(0) << ", ";
	cout << "last bin content = " << h->GetBinContent(Int_t(max_bin_val) + 1) << endl;

	for (UInt_t i=Int_t(min_bin_val); i<Int_t(max_bin_val); i++) {
		cout << i << ": " << dict[i] << endl;
	}

	h->Draw();
	h->SetLineWidth(3);

	gPad->SetGrid(2, 2);
}