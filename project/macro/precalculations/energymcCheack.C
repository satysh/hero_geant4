void energymcCheack()
{
  TH1F *hist = new TH1F("hist", "hist", 100, 0., 21.);

  fstream fin("input/mcenergy.txt");
  if (!fin.is_open()) {
    cerr << "Can't open txt file!" << endl;
    return;
  }

  while (!fin.eof()) {
    Double_t curE;
    fin >> curE;;
    cout << curE << endl;
    hist->Fill(curE);
  }

  hist->Draw();
}
