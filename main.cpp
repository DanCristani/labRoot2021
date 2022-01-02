#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TRandom.h"

#include "Particle.h"
#include "ParticleType.h"
#include "ResonanceType.h"

R__LOAD_LIBRARY(ParticleType_cpp.so);
R__LOAD_LIBRARY(ResonanceType_cpp.so);
R__LOAD_LIBRARY(Particle_cpp.so);

using namespace simulation;

int main()
{
  try {
    // adding the needed particle types to the array
    Particle::AddParticleType("null particle", 0., 0., 0.);
    Particle::AddParticleType("π+", 0.13957, 1, 0.);
    Particle::AddParticleType("π-", 0.13957, -1, 0.);
    Particle::AddParticleType("K+", 0.49367, 1, 0.);
    Particle::AddParticleType("K-", 0.49367, -1, 0.);
    Particle::AddParticleType("P+", 0.93827, 1, 0.);
    Particle::AddParticleType("P-", 0.93827, -1, 0.);
    Particle::AddParticleType("K*", 0.89166, 0, 0.050);

    // variables needed in the for loops
    int const N = 120;
    double phi = 0;
    double theta = 0;
    double momentum = 0;

    // histograms
    TH1F* h[14];
    h[0] = new TH1F("hParticle", "Generated particles", 9, 0, 9);
    h[1] = new TH1F("hPhi", "Azimuth angle (Phi)", 200, 0, 2 * TMath::Pi());
    h[2] = new TH1F("hTheta", "Polar angle (Theta)", 200, 0, TMath::Pi());
    h[3] = new TH1F("hMomentum", "Momentum modulus", 200, 0, 7);
    h[4] = new TH1F("hTMomentum", "Transverse momentum", 200, 0, 7);
    h[5] = new TH1F("hEnergy", "Energy", 200, 0, 7);
    h[6] = new TH1F("hInvMass1", "Invariant masses", 300, 0, 5);
    h[7] =
        new TH1F("hInvMass2", "Invariant masses (opposite charges)", 300, 0, 2);
    h[8] = new TH1F("hInvMass3", "Invariant masses (same charges)", 300, 0, 2);
    h[9] = new TH1F("hInvMass4",
                    "Invariant masses pion/kaon (opposite charges)",
                    300,
                    0,
                    2);
    h[10] = new TH1F(
        "hInvMass5", "Invariant masses pion/kaon (same charges)", 300, 0, 2);
    h[11] = new TH1F("hInvMass6", "Invariant masses decay", 300, 0, 2);
    h[12] = new TH1F(
        "hDiff45",
        "Difference invariant masses pion/kaon (opposite-same charges)",
        300,
        0,
        2);
    h[13] = new TH1F("hDiff23",
                     "Difference invariant masses (opposite-same charges)",
                     300,
                     0,
                     2);

    // calling Sumw2 to correctly evaluate the errors
    for (int i = 0; i < 14; ++i) {
      h[i]->Sumw2();
    }

    // creating a TFile
    TFile* file = new TFile("particle.root", "RECREATE");

    // generating events
    for (int i = 0; i < 1e5; ++i) {
      Particle particles[N];
      int counter = 100;

      for (int j = 0; j < 100; ++j) {
        // generating angles and momentum
        phi = gRandom->Uniform(0, 2 * TMath::Pi());
        theta = gRandom->Uniform(0, TMath::Pi());
        momentum = gRandom->Exp(1);
        particles[j].SetP(momentum * sin(theta) * cos(phi),
                          momentum * sin(theta) * sin(phi),
                          momentum * cos(theta));

        // adding the particles in the defined proportions
        double prob = gRandom->Rndm();
        if (prob < 0.4) {
          particles[j].SetParticle("π+");
        } else if (prob >= 0.4 && prob < 0.8) {
          particles[j].SetParticle("π-");
        } else if (prob >= 0.8 && prob < 0.85) {
          particles[j].SetParticle("K+");
        } else if (prob >= 0.85 && prob < 0.9) {
          particles[j].SetParticle("K-");
        } else if (prob >= 0.9 && prob < 0.945) {
          particles[j].SetParticle("P+");
        } else if (prob >= 0.945 && prob < 0.99) {
          particles[j].SetParticle("P-");
        } else {
          particles[j].SetParticle("K*");
        }

        // filling histograms whithout derived particles
        h[0]->Fill(particles[j].GetIndex());
        h[1]->Fill(phi);
        h[2]->Fill(theta);
        h[3]->Fill(momentum);
        double tMomentum = sqrt(particles[j].GetPx() * particles[j].GetPx() +
                                particles[j].GetPy() * particles[j].GetPy());
        h[4]->Fill(tMomentum);
        h[5]->Fill(particles[j].Energy());

        // decay of K*
        double prob2 = gRandom->Rndm();
        if (particles[j].GetIndex() == 7) {
          if (prob2 < 0.5) {
            particles[counter].SetParticle("π+");
            particles[counter + 1].SetParticle("K-");
          } else {
            particles[counter].SetParticle("π-");
            particles[counter + 1].SetParticle("K+");
          }
          particles[j].Decay2body(particles[counter], particles[counter + 1]);
          h[11]->Fill(particles[counter].InvariantMass(particles[counter + 1]));
          counter += 2;
        }

      }  // for j

      // filling invariant mass histograms
      for (int k = 0; k < counter; ++k) {
        for (int l = k + 1; l < counter; ++l) {
          if (particles[k].GetIndex() != 7 &&
              particles[l].GetIndex() != 7 &&  // K*
              particles[k].GetIndex() != 0 &&
              particles[l].GetIndex() != 0) {  // null particle
            h[6]->Fill(particles[k].InvariantMass(particles[l]));

            // first particle positive
            if (particles[l].GetCharge() == 1) {
              // filling opposite charges histogram
              if (particles[k].GetCharge() == -1) {
                h[7]->Fill(particles[k].InvariantMass(particles[l]));
              } else {
                // filling same charges histogram
                h[8]->Fill(particles[k].InvariantMass(particles[l]));
              }
            } else {  // first particle negative
              // filling opposite charges histogram
              if (particles[k].GetCharge() == 1) {
                h[7]->Fill(particles[k].InvariantMass(particles[l]));
              } else {
                // filling same charges histogram
                h[8]->Fill(particles[k].InvariantMass(particles[l]));
              }
            }

            // filling pion/kaon histograms
            if (particles[k].GetIndex() == 1) {    // π+
              if (particles[l].GetIndex() == 4) {  // K-
                h[9]->Fill(particles[k].InvariantMass(particles[l]));
              } else if (particles[l].GetIndex() == 3) {  // K+
                h[10]->Fill(particles[k].InvariantMass(particles[l]));
              }
            }

            if (particles[k].GetIndex() == 2) {    // π-
              if (particles[l].GetIndex() == 3) {  // K+
                h[9]->Fill(particles[k].InvariantMass(particles[l]));
              } else if (particles[l].GetIndex() == 4) {  // K-
                h[10]->Fill(particles[k].InvariantMass(particles[l]));
              }
            }
          }

        }  // for l
      }    // for k
    }      // for i

    // filling the difference histograms
    h[12]->Add(h[9], h[10], 1, -1);
    h[13]->Add(h[7], h[8], 1, -1);

    // writing the filled histograms on a root file
    for (int i = 0; i < 14; ++i) {
      h[i]->Write();
    }
  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << '\n';
  } catch (...) {
    std::cerr << "Unknown exception caught" << '\n';
  }
}