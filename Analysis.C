void Analysis()
{
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(57);
  gStyle->SetOptStat(112210);
  gStyle->SetOptFit(111);

  // taking the histograms from the root file
  TFile* file = new TFile("particle.root");
  TH1F* h[14];
  h[0] = (TH1F*)file->Get("hParticle");
  h[1] = (TH1F*)file->Get("hPhi");
  h[2] = (TH1F*)file->Get("hTheta");
  h[3] = (TH1F*)file->Get("hMomentum");
  h[4] = (TH1F*)file->Get("hTMomentum");
  h[5] = (TH1F*)file->Get("hEnergy");
  h[6] = (TH1F*)file->Get("hInvMass1");
  h[7] = (TH1F*)file->Get("hInvMass2");
  h[8] = (TH1F*)file->Get("hInvMass3");
  h[9] = (TH1F*)file->Get("hInvMass4");
  h[10] = (TH1F*)file->Get("hInvMass5");
  h[11] = (TH1F*)file->Get("hInvMass6");
  h[12] = (TH1F*)file->Get("hDiff45");
  h[13] = (TH1F*)file->Get("hDiff23");

  // defining the fitting functions
  TF1* f[6];
  f[0] = new TF1("fPhi", "pol0", 0, 2 * TMath::Pi());
  f[1] = new TF1("fTheta", "pol0", 0, TMath::Pi());
  f[2] = new TF1("fMomentum", "expo", 0, 10);
  f[3] = new TF1("fGauss1", "gaus", 0.6, 1.2);
  f[4] = new TF1("fGauss2", "gaus", 0.6, 1.2);
  f[5] = new TF1("fGauss3", "gaus", 0.6, 1.2);

  for (int i = 0; i < 6; ++i) {
    f[i]->SetLineColor(kRed);
    f[i]->SetLineWidth(2);
  }

  // fitting the histograms to the functions
  h[1]->Fit("fPhi", "R");
  h[2]->Fit("fTheta", "R");
  h[3]->Fit("fMomentum", "R");
  h[11]->Fit("fGauss1", "R");
  h[12]->Fit("fGauss2", "R");
  h[13]->Fit("fGauss3", "R");

  TCanvas* c1 = new TCanvas("c1", "generated distributions", 200, 10, 600, 400);
  c1->Divide(2, 2);

  h[0]->GetXaxis()->SetTitle("Particle types");
  h[0]->GetXaxis()->SetTitleOffset(1.5);
  h[0]->GetYaxis()->SetTitle("Entries");
  h[0]->GetYaxis()->SetTitleOffset(1.5);
  h[0]->SetMarkerColor(kBlack);
  h[0]->SetLineColor(kBlack);
  h[1]->GetXaxis()->SetTitle("Phi (rad)");
  h[2]->GetXaxis()->SetTitle("Theta (rad)");
  h[3]->GetXaxis()->SetTitle("Momentum (GeV)");
  h[4]->GetXaxis()->SetTitle("Transverse Momentum (GeV)");
  h[5]->GetXaxis()->SetTitle("Energy (GeV)");

  // drawing h[0]
  c1->cd(1);
  h[0]->Draw("histo");
  h[0]->Draw("E, same");

  // drawing h[1], h[2], h[3]
  for (int i = 1; i < 4; ++i) {
    c1->cd(i + 1);
    h[i]->GetXaxis()->SetTitleOffset(1.5);
    h[i]->GetYaxis()->SetTitle("Entries");
    h[i]->GetYaxis()->SetTitleOffset(1.5);
    h[i]->SetMarkerColor(kBlack);
    h[i]->SetLineColor(kBlack);
    h[i]->Draw("E, same");
  }

  c1->Print("generated_distributions.pdf");
  c1->Print("generated_distributions.C");
  c1->Print("generated_distributions.root");

  TCanvas* c2 = new TCanvas("c2", "invariant masses", 200, 10, 600, 400);
  c2->Divide(1, 3);

  // drawing h[11], h[12], h[13]
  for (int i = 0; i < 3; ++i) {
    c2->cd(i + 1);
    h[i + 11]->SetMarkerColor(kBlack);
    h[i + 11]->SetLineColor(kBlack);
    h[i + 11]->GetXaxis()->SetTitle("Invariant mass (GeV)");
    h[i + 11]->GetXaxis()->SetTitleOffset(1);
    h[i + 11]->GetYaxis()->SetTitle("Entries");
    h[i + 11]->GetYaxis()->SetTitleOffset(1.3);
    h[i + 11]->Draw("E, same");
  }

  c2->Print("invariant_masses.pdf");
  c2->Print("invariant_masses.C");
  c2->Print("invariant_masses.root");

  // printing the needed informations
  cout << "Number of π+: " << h[0]->GetBinContent(2) << " +/- "
       << h[0]->GetBinError(2) << '\n';
  cout << "Number of π-: " << h[0]->GetBinContent(3) << " +/- "
       << h[0]->GetBinError(3) << '\n';
  cout << "Number of K+: " << h[0]->GetBinContent(4) << " +/- "
       << h[0]->GetBinError(4) << '\n';
  cout << "Number of K-: " << h[0]->GetBinContent(5) << " +/- "
       << h[0]->GetBinError(5) << '\n';
  cout << "Number of P+: " << h[0]->GetBinContent(6) << " +/- "
       << h[0]->GetBinError(6) << '\n';
  cout << "Number of P-: " << h[0]->GetBinContent(7) << " +/- "
       << h[0]->GetBinError(7) << '\n';
  cout << "Number of K*: " << h[0]->GetBinContent(8) << " +/- "
       << h[0]->GetBinError(8) << '\n';
  cout << "Phi distribution parameter: " << f[0]->GetParameter(0) << " +/- "
       << f[0]->GetParError(0) << '\n';
  cout << "Phi ChiSquare/NDF: " << f[0]->GetChisquare() / f[0]->GetNDF()
       << '\n';
  cout << "Theta distribution parameter: " << f[1]->GetParameter(0) << " +/- "
       << f[1]->GetParError(0) << '\n';
  cout << "Theta ChiSquare/NDF: " << f[1]->GetChisquare() / f[1]->GetNDF()
       << '\n';
  cout << "Momentum distribution mean: " << f[2]->GetParameter(1) << " +/- "
       << f[2]->GetParError(1) << '\n';
  cout << "Momentum ChiSquare/NDF: " << f[2]->GetChisquare() / f[2]->GetNDF()
       << '\n';
  cout << "Real K* invariant mass mean: " << f[3]->GetParameter(1) << " +/- "
       << f[3]->GetParError(1) << '\n';
  cout << "Real K* invariant mass RMS: " << f[3]->GetParameter(2) << " +/- "
       << f[3]->GetParError(2) << '\n';
  cout << "Real K* invariant mass amplitude: " << f[3]->GetParameter(0)
       << " +/- " << f[3]->GetParError(0) << '\n';
  cout << "Real K* invariant mass ChiSquare/NDF: "
       << f[3]->GetChisquare() / f[3]->GetNDF() << '\n';
  cout << "Diff opposite/same π/K invariant mass mean: "
       << f[4]->GetParameter(1) << " +/- " << f[4]->GetParError(1) << '\n';
  cout << "Diff opposite/same π/K invariant mass RMS: " << f[4]->GetParameter(2)
       << " +/- " << f[4]->GetParError(2) << '\n';
  cout << "Diff opposite/same π/K invariant mass amplitude: "
       << f[4]->GetParameter(0) << " +/- " << f[4]->GetParError(0) << '\n';
  cout << "Diff opposite/same π/K invariant mass ChiSquare/NDF: "
       << f[4]->GetChisquare() / f[4]->GetNDF() << '\n';
  cout << "Diff opposite/same invariant mass mean: "
       << f[5]->GetParameter(1) << " +/- " << f[5]->GetParError(1) << '\n';
  cout << "Diff opposite/same invariant mass RMS: " << f[5]->GetParameter(2)
       << " +/- " << f[5]->GetParError(2) << '\n';
  cout << "Diff opposite/same invariant mass amplitude: "
       << f[5]->GetParameter(0) << " +/- " << f[5]->GetParError(0) << '\n';
  cout << "Diff opposite/same invariant mass ChiSquare/NDF: "
       << f[5]->GetChisquare() / f[5]->GetNDF() << '\n';
}