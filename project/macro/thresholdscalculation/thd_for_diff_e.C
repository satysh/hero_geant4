void thd_for_diff_e()
{
  TCanvas* canv = new TCanvas("canv", "canv");

  Double_t stdDev = 85.58;
  TLine* stdDev_line = new TLine(0., stdDev, 10100., stdDev);

  Double_t markersSize = 2.;

// ----------- Fe -------------------------
/*
  TGraph* femean = new TGraph();
  femean->SetPoint(0, 1000., 9.12);
  femean->SetPoint(1, 10000., 91.72);
  femean->SetMarkerColor(6);
  femean->SetLineColor(6);
  femean->SetLineStyle(9);
  femean->SetMarkerSize(markersSize);
  femean->SetMarkerStyle(kFullCircle);
*/

  TGraph* femax = new TGraph();
  femax->SetPoint(0, 1000., 143.);
  femax->SetPoint(1, 10000., 1407.);
  femax->SetMarkerColor(6);
  femax->SetLineColor(6);
  femax->SetLineStyle(9);
  femax->SetMarkerSize(markersSize);
  femax->SetMarkerStyle(34);
// ----------------------------------------

// ----------- He -------------------------
/*
  TGraph* hemean = new TGraph();
  hemean->SetPoint(0, 1000., 9.04);
  hemean->SetPoint(1, 10000., 98.78);
  hemean->SetMarkerColor(4);
  hemean->SetLineColor(4);
  hemean->SetLineStyle(9);
  hemean->SetMarkerSize(1.5);
  hemean->SetMarkerStyle(kFullCircle);
*/

  TGraph* hemax = new TGraph();
  hemax->SetPoint(0, 1000., 141.);
  hemax->SetPoint(1, 10000., 1520.);
  //hemax->SetPoint(2, 100., 1049.);
  hemax->SetMarkerColor(7);
  hemax->SetLineColor(7);
  hemax->SetLineStyle(9);
  hemax->SetMarkerSize(1.5);
  hemax->SetMarkerStyle(34);
// ----------------------------------------

// ----------- proton -------------------------
  /*
  TGraph* protonmean = new TGraph();
  protonmean->SetPoint(0, 1000., 199.22);
  protonmean->SetMarkerColor(3);
  protonmean->SetLineColor(3);
  protonmean->SetLineStyle(9);
  protonmean->SetMarkerSize(markersSize);
  protonmean->SetMarkerStyle(kFullCircle);*/

  TGraph* protonmax = new TGraph();
  protonmax->SetPoint(0, 1000., 1866.);
  //protonmax->SetPoint(1, 10., 36.);
  //protonmax->SetPoint(2, 100., 249.);
  protonmax->SetMarkerColor(3);
  protonmax->SetLineColor(3);
  protonmax->SetLineStyle(9);
  protonmax->SetMarkerSize(markersSize);
  protonmax->SetMarkerStyle(34);
// ----------------------------------------


// ------- Draw ----------------------------------------------------
  TMultiGraph* mg = new TMultiGraph();
  //mg->Add(femean);
  mg->Add(femax);
  //mg->Add(hemean);
  mg->Add(hemax);
  //mg->Add(protonmean);
  mg->Add(protonmax);
  mg->SetTitle("thresholds for 1 usec counts");
  mg->GetXaxis()->SetTitle("E [GeV/n]");
  mg->GetYaxis()->SetTitle("alpha counts");
  mg->Draw("AP");
  stdDev_line->Draw("SAME");
  stdDev_line->SetLineColor(kRed);
  stdDev_line->SetLineWidth(2);
  stdDev_line->SetLineStyle(9);

  TLegend* legend = new TLegend(0.1, 0.7, 0.48, 0.9);
  legend->AddEntry(stdDev_line,"background","l");
  //legend->AddEntry(femean, "electron mean count", "p");
  legend->AddEntry(femax, "electron", "p");
  //legend->AddEntry(hemean, "gamma mean count", "p");
  legend->AddEntry(hemax, "gamma", "p");
  //legend->AddEntry(protonmean, "proton mean count", "p");
  legend->AddEntry(protonmax, "proton", "p");
  legend->Draw();
}
