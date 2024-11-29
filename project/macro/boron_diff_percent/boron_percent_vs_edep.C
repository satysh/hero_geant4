
void boron_percent_vs_edep()
{
	Int_t NEvents = 10000;
	Int_t PDG = 2212;
	Int_t R = 125;
	Int_t E = 1000; // MeV
	TString BORON = "b";

	Int_t NPoints = 7;
	Double_t vMeanEdep[NPoints];
	Double_t vMeanEdepError[NPoints];
	Double_t vPerCent[NPoints];

	TString canvTitle;
	canvTitle.Form("canv_E_%d", E / 1000);
	TCanvas *c = new TCanvas(canvTitle, canvTitle);
	c->Divide(3, 6);

	Int_t PerCent = -1;
	for (Int_t i=0; i<NPoints; i++) {
		PerCent++;
		
		if (PerCent == 0) BORON = "-b";
		else BORON = "b";

		TString fileName;
		fileName.Form("data/hero_nevents_%d_pdg_%d_R_%d_E_%d_bron_%s_percent_%d.root", NEvents, PDG, R, E, BORON.Data(), PerCent);
		TFile *file = new TFile(fileName, "READ");

		if (file->IsZombie()) {
			cerr << "Can't open file " << fileName << "!" << endl;
			return;
		}

		TTree *tree = (TTree*)file->Get("HERO");
		if (!tree) {
			cerr << "Can't open tree HERO in file " << fileName << "!" << endl;
			return;
		}

		c->cd(i + 1);
		tree->Draw("edep * 0.001"); // MeV to GeV

		TH1F *htemp = (TH1F*)gPad->FindObject("htemp");
		htemp->Scale(Double_t(1. / NEvents)); 
		htemp->Draw("HIST");
		htemp->SetLineWidth(3);
		htemp->SetLineColor(kBlue);
		gPad->SetGrid(2, 2);

		TString title;
		title.Form("boron %d percent", i);
		htemp->SetTitle(title);
		htemp->GetXaxis()->SetTitle("edep [GeV]");
		htemp->GetYaxis()->SetTitle("counts / nevents");

		Double_t mean = 1000 * htemp->GetMean() / E; // norm
		Double_t meanError = 1000 * htemp->GetMeanError() / E;
		Double_t stdDev = htemp->GetStdDev();

		vMeanEdep[i] = mean;
		vMeanEdepError[i] = meanError;
		vPerCent[i] = Double_t(i);
		file->Close();
	}

	TCanvas *canv = new TCanvas("result", "result");
	canv->cd();
	TGraphErrors *gr = new TGraphErrors(NPoints, vPerCent, vMeanEdep, NULL, vMeanEdepError);
	gr->SetMarkerStyle(20);
	gr->SetMarkerColor(kRed);
	gr->SetMarkerSize(2);
	gr->SetLineColor(kBlue);
	gr->SetLineWidth(5);
	TString name;
	name.Form("gr_%d", E / 1000);
	gr->SetName(name);
	gr->Draw("ACP");
	TString title;
	title.Form("mean edep vs boron percent for primary proton E=%d GeV", E / 1000);
	gr->SetTitle(title);	
	gr->GetXaxis()->SetTitle("boron percent");
	gr->GetYaxis()->SetTitle("mean_edep / primary_energy");
	gPad->SetGrid(2, 2);

	TFile *file = new TFile("gr_out.root", "UPDATE");
	c->Write();
	gr->Write();
}