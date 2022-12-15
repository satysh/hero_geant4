void testStartTime()
{
  TH1F *histo = new TH1F("histo", "histo", 100, 0., (1./16.)*1e9);
  for (Int_t i=1; i<4; i++) {
    TString inFileName;
    inFileName.Form("../output/err_%d.txt", i);
    fstream fin(inFileName.Data());
    if (!fin.is_open()) {
      cerr << "Can't read " << inFileName << endl;
      return;
    }
    while (!fin.eof()) {
      TString curStr;
      fin >> curStr;
      if (curStr == "HEROPrimaryGenerator::GeneratePrimaries_start_time=") {
        Double_t time;
        fin >> time;
        histo->Fill(time);
      }
    }
    fin.close();
  }
  histo->Draw();
}
