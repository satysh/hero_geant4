void thd_for_diff_e()
{
  TCanvas* canv = new TCanvas("canv", "canv");

  Double_t stdDev = 9.;
  TLine* stdDev_line = new TLine(0., stdDev, 10100., stdDev);
  TLine* stdDev_line_2 = new TLine(0., 2.*stdDev, 10100., 2.*stdDev);
  TLine* stdDev_line_3 = new TLine(0., 3.*stdDev, 10100., 3.*stdDev);

  Double_t markersSize = 2.;

// ----------- Fe ------------------------
  TGraph* femax = new TGraph();
  femax->SetPoint(0, 1., 29.184);
  femax->SetPoint(1, 10., 736.11);
  femax->SetMarkerColor(2);
  femax->SetLineColor(2);
  femax->SetLineStyle(9);
  femax->SetMarkerSize(markersSize);
  femax->SetMarkerStyle(34);
// ----------------------------------------

// ----------- He4 -------------------------
  TGraph* he4max = new TGraph();
  he4max->SetPoint(0, 1., 6.5811);
  he4max->SetPoint(1, 10., 50.4148);
  he4max->SetMarkerColor(3);
  he4max->SetLineColor(3);
  he4max->SetLineStyle(9);
  he4max->SetMarkerSize(markersSize);
  he4max->SetMarkerStyle(34);
// ----------------------------------------

// ----------- He3 -------------------------
  TGraph* he3max = new TGraph();
  he3max->SetPoint(0, 1., 4.66);
  he3max->SetPoint(1, 10., 38.565);
  he3max->SetMarkerColor(6);
  he3max->SetLineColor(6);
  he3max->SetLineStyle(9);
  he3max->SetMarkerSize(markersSize);
  he3max->SetMarkerStyle(34);
// ----------------------------------------

// ----------- proton -------------------------
  TGraph* protonmax = new TGraph();
  protonmax->SetPoint(0, 1., 1.6586);
  protonmax->SetPoint(1, 10., 13.0329);
  protonmax->SetMarkerColor(4);
  protonmax->SetLineColor(4);
  protonmax->SetLineStyle(9);
  protonmax->SetMarkerSize(markersSize);
  protonmax->SetMarkerStyle(kFullCircle);

/*
  TVectorD x(16);
  TVectorD y(16);
  ifstream fin("input/counts.txt");
  for (Int_t i=0; i<16; i++) {
    TString str;
    fin >> str;
    fin >> x(i);
    fin >> str;
    fin >> y(i);

  }
  TGraph* protonmax = new TGraph(x, y);
  protonmax->SetMarkerColor(3);
  protonmax->SetLineColor(3);
  protonmax->SetLineStyle(9);
  protonmax->SetMarkerSize(markersSize);
  protonmax->SetMarkerStyle(34);
*/
// ----------------------------------------


// ------- Draw ----------------------------------------------------
  TMultiGraph* mg = new TMultiGraph();
  mg->Add(femax);
  mg->Add(he4max);
  mg->Add(he3max);
  mg->Add(protonmax);
  mg->SetTitle("thresholds for 1 usec counts");
  mg->GetXaxis()->SetTitle("E [GeV/n]");
  mg->GetYaxis()->SetTitle("alpha counts");
  mg->Draw("AP");
  stdDev_line->Draw("SAME");
  stdDev_line->SetLineColor(kRed);
  stdDev_line->SetLineWidth(2);
  stdDev_line->SetLineStyle(9);
  stdDev_line_2->Draw("SAME");
  stdDev_line_2->SetLineColor(kRed);
  stdDev_line_2->SetLineWidth(2);
  stdDev_line_2->SetLineStyle(2);
  stdDev_line_3->Draw("SAME");
  stdDev_line_3->SetLineColor(kRed);
  stdDev_line_3->SetLineWidth(2);
  stdDev_line_3->SetLineStyle(10);

  TLegend* legend = new TLegend(0.1, 0.7, 0.48, 0.9);
  legend->AddEntry(stdDev_line,"stdDev","l");
  legend->AddEntry(stdDev_line_2,"2*stdDev","l");
  legend->AddEntry(stdDev_line_3,"3*stdDev","l");
  legend->AddEntry(femax, "Fe", "p");
  legend->AddEntry(he4max, "He4", "p");
  legend->AddEntry(he3max, "He3", "p");
  legend->AddEntry(protonmax, "proton", "p");
  legend->Draw();
}
