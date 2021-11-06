#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Particle.h"
#include "ParticleType.h"
#include "ResonanceType.h"

using namespace simulation;

TEST_CASE("testing ParticleType family")
{
  // testing ParticleType
  ParticleType particle{"particle", 3, 1};
  CHECK(particle.GetName() == "particle");
  CHECK(particle.GetMass() == 3.);
  CHECK(particle.GetCharge() == 1.);
  CHECK(particle.GetWidth() == 0.);

  // testing ResonanceType
  ResonanceType resonance{"resonance", 3, 1, 2};
  CHECK(resonance.GetName() == "resonance");
  CHECK(resonance.GetMass() == 3.);
  CHECK(resonance.GetCharge() == 1.);
  CHECK(resonance.GetWidth() == 2.);
}

TEST_CASE("testing Particle")
{
  Particle::AddParticleType("positivePion", 0.13957, 1, 0);
  Particle::AddParticleType("negativePion", 0.13957, -1, 0);
  Particle positivePion{"positivePion", 1., 2., 3.};
  Particle negativePion{"negativePion", 1., 2., 3.};

  // testing getters and setters
  CHECK(positivePion.GetPx() == 1.);
  CHECK(positivePion.GetPy() == 2.);
  CHECK(positivePion.GetPz() == 3.);
  CHECK(positivePion.GetMass() == doctest::Approx(0.13957));
  CHECK(positivePion.GetCharge() == 1);
  CHECK(positivePion.Energy() == doctest::Approx(3.744259578));
  CHECK(positivePion.InvariantMass(negativePion) == doctest::Approx(0.27914));
  CHECK(positivePion.GetName() == "positivePion");
  CHECK(positivePion.GetIndex() == 0);
  positivePion.SetParticle(1);
  positivePion.SetP(2., 3., 4.);
  CHECK(positivePion.GetName() == "negativePion");
  CHECK(positivePion.GetIndex() == 1);
  CHECK(positivePion.GetPx() == 2.);
  CHECK(positivePion.GetPy() == 3.);
  CHECK(positivePion.GetPz() == 4.);
  CHECK(positivePion.GetCharge() == -1);

  Particle::AddParticleType("k*", 0.89166, 0., 0.050);
  Particle resonance{"k*", 0, 0, 0};
  resonance.SetParticle("negativePion");
  CHECK(resonance.GetName() == "negativePion");
  CHECK(resonance.GetIndex() == 1);

  // testing exceptions
  CHECK_THROWS_AS(negativePion.SetParticle(-1), std::runtime_error);
  CHECK_THROWS_AS(negativePion.SetParticle(5), std::runtime_error);
  CHECK_THROWS_AS(negativePion.SetParticle(10), std::runtime_error);
  CHECK_THROWS_AS(negativePion.SetParticle("positron"), std::runtime_error);
  Particle::AddParticleType("3", 0, 0, 0);
  Particle::AddParticleType("4", 0, 0, 0);
  Particle::AddParticleType("5", 0, 0, 0);
  Particle::AddParticleType("6", 0, 0, 0);
  Particle::AddParticleType("7", 0, 0, 0);
  Particle::AddParticleType("8", 0, 0, 0);
  Particle::AddParticleType("9", 0, 0, 0);
  CHECK_THROWS_AS(Particle::AddParticleType("10", 0, 0, 0), std::runtime_error);
}