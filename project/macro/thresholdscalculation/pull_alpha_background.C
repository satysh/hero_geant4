// Макрос вытаскиевает из root фаила информацию только по альфа 
// и записывает ее в новый root файл
void pull_alpha_background(TString input_file_name = "helium_4000_GeV_100_events.root")
{
	TFile *file = new TFile("input/" + input_file_name);
	if (file->IsZombie()) {	
		cerr << "Can't open input file!" << endl;
		return;
	}

	Int_t eventId;
	Int_t pdg;
	Int_t trackID;
	Double_t deposit_E;
	Double_t kin_E;
	Double_t pre_x;
	Double_t pre_y;
	Double_t pre_z;
	Double_t post_x;
	Double_t post_y;
	Double_t post_z;
	Double_t t;
	Double_t track_time;

	TTree *tree = (TTree*)file->Get("HERO");
	if (!tree) {
		cerr << "Can't find tree!" << endl;
		return;
	}

	tree->SetBranchAddress("eventId", &eventId);
	tree->SetBranchAddress("pdg", &pdg);
	tree->SetBranchAddress("trackID", &trackID);
	tree->SetBranchAddress("deposit_E", &deposit_E);
	tree->SetBranchAddress("kin_E", &kin_E);
	tree->SetBranchAddress("pre_x", &pre_x);
	tree->SetBranchAddress("pre_y", &pre_y);
	tree->SetBranchAddress("pre_z", &pre_z);
	tree->SetBranchAddress("post_x", &post_x);
	tree->SetBranchAddress("post_y", &post_y);
	tree->SetBranchAddress("post_z", &post_z);
	tree->SetBranchAddress("t", &t);
	tree->SetBranchAddress("track_time", &track_time);


	TFile *alpha_file = new TFile("output/fixedstarttime/alpha_" + input_file_name, "RECREATE");
	TTree *alpha_tree = new TTree("alpha_tree", "alpha_tree");

	alpha_tree->Branch("eventId", &eventId, "eventId/I");
	alpha_tree->Branch("pdg", &pdg, "pdg/I");
	alpha_tree->Branch("trackID", &trackID, "trackID/I");
	alpha_tree->Branch("deposit_E", &deposit_E, "deposit_E/D");
	alpha_tree->Branch("kin_E", &kin_E, "kin_E/D");
	alpha_tree->Branch("pre_x", &pre_x, "pre_x/D");
	alpha_tree->Branch("pre_y", &pre_y, "pre_y/D");
	alpha_tree->Branch("pre_z", &pre_z, "pre_z/D");
	alpha_tree->Branch("post_x", &post_x, "post_x/D");
	alpha_tree->Branch("post_y", &post_y, "post_y/D");
	alpha_tree->Branch("post_z", &post_z, "post_z/D");
	alpha_tree->Branch("t", &t, "t/D");
	alpha_tree->Branch("track_time", &track_time, "track_time/D");

	for (Int_t i=0; i<tree->GetEntries(); i++) {
		tree->GetEntry(i);
		if (pdg == 1000020040) {
			alpha_tree->Fill();
		}
	}

	alpha_tree->Write();
}