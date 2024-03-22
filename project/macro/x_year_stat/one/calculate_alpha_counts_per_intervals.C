const Int_t INTERVALS_CNT=45;
void create_empty_outfile(TString out_file_name);
Double_t *read_array_from_pre_out_file(TString out_file_name);
void print_array(Double_t *arr);
void write_array_to_post_out_file(Double_t *arr, TString out_file_name);
Double_t get_full_protons_number_from_flux(Int_t interval_id);
TString get_in_file_name(Int_t interval_id);
Double_t get_nevents(TString in_file_name);
void bind_parallel_calculations(TString path, TString out_file_name);
Double_t get_alpha_counts(Int_t interval_id);

void calculate_alpha_counts_per_intervals(Int_t interval_id=0, TString path="", Bool_t is_parallel=false)
{
	TString out_file_name = "calculate_alpha_counts_per_intervals_out.txt";
	create_empty_outfile(path + out_file_name);
	
	Double_t *out_array = read_array_from_pre_out_file(path + out_file_name);
	
	//cout << "pre values: " << endl;
	//print_array(out_array);
	
	Double_t ni = get_full_protons_number_from_flux(interval_id);
	//cout << "ni=" << ni << endl;

	TString in_file_name = get_in_file_name(interval_id);
	cout << "in_file_name=" << in_file_name << endl;

	Double_t Ni = get_nevents(in_file_name);
	//cout << "Ni=" << Ni << endl;


	/* Calculations */
	out_array[interval_id] = get_alpha_counts(interval_id) * ni / Ni;
	cout << out_array[interval_id] << endl;


	//cout << "post values: " << endl;
	//print_array(out_array);
	TString prefix = "";
	if (is_parallel) {
		prefix.Form("E0_%d_E1_%d_", interval_id, interval_id+1);
		//cout << out_file_name << endl;
	}
	write_array_to_post_out_file(out_array, path + prefix + out_file_name);

	if (is_parallel) {
		if (path == "") {
			path = "./";
		}
		bind_parallel_calculations(path, out_file_name);
	}
}
// ----------------------------------------------------------------------
void create_empty_outfile(TString out_file_name)
{
	ifstream check(out_file_name.Data());
	if (check.is_open()) {
		cout << out_file_name << " is found!" << endl;
		check.close();
		return;
	}
	ofstream out(out_file_name.Data());
	if (!out.is_open()) {
		cerr << "Can't create empty out file!" << endl;
		return;
	}

	for (Int_t i=0; i<INTERVALS_CNT; i++) {
		out << i << " " << i+1 << " " << 0 << endl; 
	}

	out.close();
	cout << out_file_name << " is created!" << endl;
}
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
Double_t *read_array_from_pre_out_file(TString out_file_name)
{
	ifstream fin(out_file_name.Data());
	if (!fin.is_open()) {
		cerr << "Can't pre read out file!" << endl;
		return NULL; 
	}

	Double_t *result = new Double_t[INTERVALS_CNT];

	while (!fin.eof()) {
		Int_t E0, E1;
		Double_t value;
		fin >> E0 >> E1 >> value;
		result[E0] = value; // Тут слабое место при произвольных длинах интревала
	}
	fin.close();

	return result;
}
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
void print_array(Double_t *arr) 
{
	for (Int_t i=0; i<INTERVALS_CNT; i++) {
		cout << i << ", " << i+1 << ", " << arr[i] << endl;
	}
}
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
void write_array_to_post_out_file(Double_t *arr, TString out_file_name)
{
	ofstream out(out_file_name.Data());
	if (!out.is_open()) {
		cerr << "Can't recreate out file!" << endl;
		return;
	}

	for (Int_t i=0; i<INTERVALS_CNT; i++) {
		out << i << " " << i+1 << " " << arr[i] << endl;
	}

	out.close();
}
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
Double_t get_full_protons_number_from_flux(Int_t interval_id)
{
	Double_t result = -1;
	ifstream fin("../../../input/one_year_protons_number_7e-8%.txt");
	if (!fin.is_open()) {
		cerr << "Can't read protons full nubmber from flux!" << endl;
		return result;
	}


	for (Int_t i=0; i<=interval_id; i++) {
		Int_t E0, E1, Ni;
		Double_t ni;

		fin >> E0 >> E1 >> ni >> Ni;
		result = ni;
	}

	return result;
}
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
TString get_in_file_name(Int_t interval_id)
{
	void *dirp = gSystem->OpenDirectory("input");
	Char_t *afile;

	while ((afile = const_cast<Char_t *>(gSystem->GetDirEntry(dirp)))) {
		TString safile(afile);
		TString tmp;
		tmp.Form("hero_E0_%d_E1_%d", interval_id, interval_id+1);
		if (safile.Contains(tmp)) {
			break;
		}
	}

	TString in_file_name(afile);
	return in_file_name;
}
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
Double_t get_nevents(TString in_file_name) 
{
	Int_t left_index = in_file_name.Index("nevents_") + 8;
	Int_t right_index = in_file_name.Index(".root");
	
	return TString(in_file_name(left_index, right_index - left_index)).Atoi();
}
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
void bind_parallel_calculations(TString path, TString out_file_name)
{
	void *dirp = gSystem->OpenDirectory(path);
	Char_t *afile;

	std::vector<TString> v_files_to_process;
	while ((afile = const_cast<Char_t *>(gSystem->GetDirEntry(dirp)))) {
		TString safile(afile);
		if (safile.BeginsWith("E0")) {
			v_files_to_process.push_back(safile);
		}
	}

	if (v_files_to_process.size() == INTERVALS_CNT) {
		cout << "Bind is working!" << endl;
		
		Double_t *result = new Double_t[INTERVALS_CNT];

		for (UInt_t i=0; i<INTERVALS_CNT; i++) {
			TString file_to_pocess;
			file_to_pocess.Form("E0_%d_E1_%d_calculate_alpha_counts_per_intervals_out.txt", i, i+1);
			file_to_pocess = path + file_to_pocess;
			
			ifstream now_fin(file_to_pocess.Data());
			if (!now_fin.is_open()) {
				cerr << "Can't read " << file_to_pocess << endl;
				return;
			}

			Int_t E0, E1;
			Double_t value;

			for (UInt_t j=0; j<=i; j++) {
				now_fin >> E0 >> E1 >> value;
			}

			result[i] = value;
			now_fin.close();
		}

		ofstream out((path + out_file_name).Data());
		if (!out.is_open()) {
			cerr << "Can't create " << path + out_file_name << endl;
			return;
		}

		for (UInt_t i=0; i<INTERVALS_CNT; i++) {
			out << i << " " << i+1 << " " << result[i] << endl;
		}

		out.close();
	}
}
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
Double_t get_alpha_counts(Int_t interval_id) 
{
	Double_t result = -1.;
	void *dirp = gSystem->OpenDirectory("input");
	Char_t *afile;

	TString tmp;
	tmp.Form("hero_E0_%d_E1_%d_nevents_", interval_id, interval_id+1);
	while ((afile = const_cast<Char_t *>(gSystem->GetDirEntry(dirp)))) {
		TString safile(afile);
		if (safile.Contains(tmp)) {
			break;
		}
	}

	TString data_file_name(afile);
	data_file_name = "input/" + data_file_name;

	TFile *file = new TFile(data_file_name.Data(), "READ");
	if (file->IsZombie()) {
		cerr << "Can't read " << data_file_name << endl;
		return result;
	}

	TTree *tree = (TTree*)file->Get("HERO");
	if (!tree) {
		cerr << "Can't find tree in file " << data_file_name << endl;
		return result;
	}

	Int_t track_id;
	Int_t pdg;
	tree->SetBranchAddress("trackID", &track_id);
	tree->SetBranchAddress("pdg", &pdg);

	std::unordered_set<Int_t> uniq_track_id_set;
	for (Int_t i=0; i<tree->GetEntries(); i++) {
		tree->GetEntry(i);

		if (pdg == 1000020040) {
			uniq_track_id_set.insert(track_id);
		}
	}

	result = Double_t(uniq_track_id_set.size());
	uniq_track_id_set.clear();

	return result;
}