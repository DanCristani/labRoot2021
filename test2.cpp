#include <iostream>

#include "Particle.h"
#include "ParticleType.h"
#include "ResonanceType.h"

using namespace simulation;

int main()
{
  // testing ParticleType::Print() and ResonanceType::Print()
  ParticleType particleType{"particleType", 3, 1};
  particleType.Print();
  ResonanceType resonance{"resonance", 3, 1, 2};
  resonance.Print();

  // testing ParticleType::Print() virtual behaviour
  ParticleType* array[2];
  array[0] = new ParticleType("particle", 3, 1);
  array[1] = new ResonanceType("resonance", 3, 1, 2);
  for (int i = 0; i < 2; ++i) {
    array[i]->Print();
  }

  // adding the particle needed
  Particle::AddParticleType("π+", 0.13957, 1, 0.);
  Particle::AddParticleType("π-", 0.13957, -1, 0.);
  Particle::AddParticleType("K+", 0.49367, 1, 0.);
  Particle::AddParticleType("K-", 0.49367, -1, 0.);
  Particle::AddParticleType("P+", 0.932827, 1, 0.);
  Particle::AddParticleType("P-", 0.932827, -1, 0.);
  Particle::AddParticleType("K*", 0.89166, 0, 0.050);

  // testing Particle::PrintParticleTypes()
  Particle::PrintParticleTypes();

  // testing Particle::Print()
  Particle proton{"P+", 0, 0, 0};
  proton.Print();
}
