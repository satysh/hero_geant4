void testStartTime()
{
  TH1F *histo = new TH1F("histo", "histo", 100, 0., 1e3+100);
  TString inFileName = "input/start_times.txt";
  //inFileName.Form("../output/err_%d.txt", i);
  fstream fin(inFileName.Data());
  if (!fin.is_open()) {
    cerr << "Can't read " << inFileName << endl;
    return;
  }

  while (true) {
    string curStr;
    Double_t time;
    fin >> curStr >> time; // skip
    if (fin.eof()) break;
    histo->Fill(time);
  }
  fin.close();

  cout << "nEntries: " << histo->GetEntries() << endl;
  histo->Draw();
}
