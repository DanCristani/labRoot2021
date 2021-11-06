void SetStyle()
{
}

void Analysis()
{
  // taking the histograms from the root file
  TFile* file = new TFile("particle.root");
  TH1F* h[14] = {};
  TH1F* h[0] = (TH1F*)file->Get("hParticle");
  TH1F* h[1] = (TH1F*)file->Get("hPhi");
  TH1F* h[2] = (TH1F*)file->Get("hTheta");
  TH1F* h[3] = (TH1F*)file->Get("hMomentum");
  TH1F* h[4] = (TH1F*)file->Get("hTMomentum");
  TH1F* h[5] = (TH1F*)file->Get("hEnergy");
  TH1F* h[6] = (TH1F*)file->Get("hInvMass1");
  TH1F* h[7] = (TH1F*)file->Get("hInvMass2");
  TH1F* h[8] = (TH1F*)file->Get("hInvMass3");
  TH1F* h[9] = (TH1F*)file->Get("hInvMass4");
  TH1F* h[10] = (TH1F*)file->Get("hInvMass5");
  TH1F* h[11] = (TH1F*)file->Get("hInvMass6");
  TH1F* h[12] = new TH1F(
      "hDiff45", "Difference opposite and same pion/kaon", 10000, 0, 7);
  h[12] = h[9] - h[10];
  TH1F* h[13] =
      new TH1F("hDiff23", "Difference opposite and same charges", 10000, 0, 7);
  h[13] = h[7] - h[8];

  // defining the fitting functions
  TF1* fPhi = new TF1("fPhi", [0], 0, 2 * TMath::Pi());

  // drawing the histograms
  for (Int_t i = 0; i < 14; ++i) {
    h[i]->Draw("Histo");
  }
}