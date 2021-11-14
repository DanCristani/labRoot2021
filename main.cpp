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
    Particle::AddParticleType("P+", 0.932827, 1, 0.);
    Particle::AddParticleType("P-", 0.932827, -1, 0.);
    Particle::AddParticleType("K*", 0.89166, 0, 0.050);

    // variables needed in the for loops
    int const N = 120;
    Double_t phi = 0;
    Double_t theta = 0;
    Double_t momentum = 0;
    Double_t prob = 0;
    Double_t prob2 = 0;

    // histograms
    TH1F* h[12];
    h[0] = new TH1F("hParticle", "Generated particles", 9, -1, 8);
    h[1] = new TH1F("hPhi", "Azimuth angle", 360, -0.5, 7);
    h[2] = new TH1F("hTheta", "Polar angle", 180, -0.5, 4);
    h[3] = new TH1F("hMomentum", "Momentum modulus", 10000, -0.5, 10);
    h[4] = new TH1F("hTMomentum", "Transverse momentum", 10000, -0.5, 10);
    h[5] = new TH1F("hEnergy", "Energy", 10000, -0.5, 10);
    h[6] = new TH1F("hInvMass1", "Invariant mass", 10000, 0, 7);
    h[7] = new TH1F("hInvMass2", "Opposite invariant mass", 10000, 0, 7);
    h[8] = new TH1F("hInvMass3", "Same charghe invariant mass", 10000, 0, 7);
    h[9] = new TH1F(
        "hInvMass4", "Opposite pion and kaon invariant mass", 10000, 0, 7);
    h[10] = new TH1F(
        "hInvMass5", "Same charge pion and kaon invariant mass", 10000, 0, 7);
    h[11] = new TH1F("hInvMass6", "Decay invariant mass", 10000, 0, 7);

    // calling Sumw2 to correctly evaluate the errors
    for (int i = 0; i < 12; ++i) {
      h[i]->Sumw2();
    }

    // creating a TFile
    TFile* file = new TFile("particle.root", "RECREATE");

    // generating events
    for (int i = 0; i < 10E5; ++i) {
      Particle particles[N];
      int counter = 100;

      for (int j = 0; j < 100; ++j) {
        // generating angles and momentum
        Particle j_particle = particles[j];
        phi = gRandom->TRandom::Uniform(0, 2 * TMath::Pi());
        theta = gRandom->TRandom::Uniform(0, TMath::Pi());
        momentum = gRandom->TRandom::Exp(1);
        j_particle.SetP(momentum * sin(theta) * cos(phi),
                        momentum * sin(theta) * sin(phi),
                        momentum * cos(theta));

        // adding the particles in the defined proportions
        prob = gRandom->TRandom::Rndm();
        if (prob < 0.4) {
          j_particle.SetParticle("π+");
        } else if (prob >= 0.4 && prob < 0.8) {
          j_particle.SetParticle("π-");
        } else if (prob >= 0.8 && prob < 0.85) {
          j_particle.SetParticle("K+");
        } else if (prob >= 0.85 && prob < 0.9) {
          j_particle.SetParticle("K-");
        } else if (prob >= 0.9 && prob < 0.945) {
          j_particle.SetParticle("P+");
        } else if (prob >= 0.945 && prob < 0.99) {
          j_particle.SetParticle("P-");
        } else {
          j_particle.SetParticle("K*");
        }

        // filling histograms whithout derived particles
        h[0]->Fill(j_particle.GetIndex());
        h[1]->Fill(phi);
        h[2]->Fill(theta);
        h[3]->Fill(momentum);
        double tMomentum = sqrt(j_particle.GetPx() * j_particle.GetPx() +
                                j_particle.GetPy() * j_particle.GetPy());
        h[4]->Fill(tMomentum);
        h[5]->Fill(j_particle.Energy());

        // decay of K*
        prob2 = gRandom->TRandom::Rndm();
        if (strcmp(j_particle.GetName(), "K*") == 0) {
          if (prob2 < 0.5) {
            particles[counter].SetParticle("π+");
            particles[counter + 1].SetParticle("K-");
          } else {
            particles[counter].SetParticle("π-");
            particles[counter + 1].SetParticle("K+");
          }
          j_particle.Decay2body(particles[counter], particles[counter + 1]);
          h[11]->Fill(particles[counter].InvariantMass(particles[counter + 1]));
          counter += 2;
        }  // if
      }    // for j

      // writing object which are living inside the for loop in a root file
      file->Write();

      // filling invariant mass histograms
      for (int k = 0; k < counter; ++k) {
        for (int l = k + 1; l < counter; ++l) {
          h[6]->Fill(particles[l].InvariantMass(particles[k]));

          // first particle positive
          if (particles[l].GetCharge() == 1) {
            // filling opposite charge histogram
            if (particles[k].GetCharge() == -1) {
              h[7]->Fill(particles[l].InvariantMass(particles[k]));
            } else if (particles[k].GetCharge() == 1) {
              // filling same charge histogram
              h[8]->Fill(particles[l].InvariantMass(particles[k]));
            }
          }

          // first particle negative
          if (particles[l].GetCharge() == -1) {
            // filling opposite charge histogram
            if (particles[k].GetCharge() == 1) {
              h[7]->Fill(particles[l].InvariantMass(particles[k]));
            } else if (particles[k].GetCharge() == -1) {
              // filling same charge histogram
              h[8]->Fill(particles[l].InvariantMass(particles[k]));
            }
          }

          // filling pion/kaon histograms
          if (particles[l].GetIndex() == 1) {    // π+
            if (particles[k].GetIndex() == 4) {  // K-
              h[9]->Fill(particles[l].InvariantMass(particles[k]));
            } else if (particles[k].GetIndex() == 3) {  // K+
              h[10]->Fill(particles[l].InvariantMass(particles[k]));
            }
          }
          if (particles[l].GetIndex() == 2) {    // π-
            if (particles[k].GetIndex() == 3) {  // K+
              h[9]->Fill(particles[l].InvariantMass(particles[k]));
            } else if (particles[k].GetIndex() == 4) {  // K-
              h[10]->Fill(particles[l].InvariantMass(particles[k]));
            }
          }
        }  // for l
      }    // for k

    }  // for i

    // writing the filled histograms on a root file
    file->Write();

    // creating a Canvas (this part will go in a separate file)
    TCanvas* canvas = new TCanvas("canvas", "histograms", 200, 10, 600, 400);
    canvas->Divide(4, 3);
    h[0]->Draw("Histo");
    h[1]->Draw("Histo");
    h[2]->Draw("Histo");
    h[3]->Draw("Histo");
    h[4]->Draw("Histo");
    h[5]->Draw("Histo");
    h[6]->Draw("Histo");
    h[7]->Draw("Histo");
    h[8]->Draw("Histo");
    h[9]->Draw("Histo");
    h[10]->Draw("Histo");
    h[11]->Draw("Histo");

  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << '\n';
  } catch (...) {
    std::cerr << "Unknown excepion caught" << '\n';
  }
  return 0;
}