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
  CHECK(strcmp(particle.GetName(), "particle") == 0);
  CHECK(particle.GetMass() == 3.);
  CHECK(particle.GetCharge() == 1);
  CHECK(particle.GetWidth() == 0.);

  // testing ParticleType default constructor
  ParticleType null_particle;
  CHECK(strcmp(null_particle.GetName(), "null particle") == 0);
  CHECK(null_particle.GetMass() == 0.);
  CHECK(null_particle.GetCharge() == 0);
  CHECK(null_particle.GetWidth() == 0.);

  // testing ResonanceType
  ResonanceType resonance{"resonance", 3, 1, 2};
  CHECK(resonance.GetName() == "resonance");
  CHECK(resonance.GetMass() == 3.);
  CHECK(resonance.GetCharge() == 1);
  CHECK(resonance.GetWidth() == 2.);

  // testing ResonanceType default contructor
  ResonanceType null_resonance;
  CHECK(strcmp(null_resonance.GetName(), "null particle") == 0);
  CHECK(null_resonance.GetMass() == 0.);
  CHECK(null_resonance.GetCharge() == 0);
  CHECK(null_resonance.GetWidth() == 0.);
}

TEST_CASE("testing Particle")
{
  // testing default constructor
  Particle::AddParticleType("null particle", 0., 0., 0.);
  Particle nullParticle;

  CHECK(nullParticle.GetPx() == 0.);
  CHECK(nullParticle.GetPy() == 0.);
  CHECK(nullParticle.GetPz() == 0.);
  CHECK(nullParticle.GetMass() == 0.);
  CHECK(nullParticle.GetCharge() == 0);
  CHECK(nullParticle.Energy() == 0.);
  CHECK(strcmp(nullParticle.GetName(), "null particle") == 0);
  CHECK(nullParticle.GetIndex() == 0);

  // testing getters and setters
  Particle::AddParticleType("positivePion", 0.13957, 1, 0);
  Particle::AddParticleType("negativePion", 0.13957, -1, 0);
  Particle positivePion{"positivePion", 1., 2., 3.};
  Particle negativePion{"negativePion", 1., 2., 3.};

  CHECK(positivePion.GetPx() == 1.);
  CHECK(positivePion.GetPy() == 2.);
  CHECK(positivePion.GetPz() == 3.);
  CHECK(positivePion.GetMass() == doctest::Approx(0.13957));
  CHECK(positivePion.GetCharge() == 1);
  CHECK(positivePion.Energy() == doctest::Approx(3.744259578));
  CHECK(positivePion.InvariantMass(negativePion) == doctest::Approx(0.27914));
  CHECK(strcmp(positivePion.GetName(), "positivePion") == 0);
  CHECK(positivePion.GetIndex() == 1);
  positivePion.SetParticle(2);  // negativePion
  positivePion.SetP(2., 3., 4.);
  CHECK(strcmp(positivePion.GetName(), "negativePion") == 0);
  CHECK(positivePion.GetIndex() == 2);
  CHECK(positivePion.GetPx() == 2.);
  CHECK(positivePion.GetPy() == 3.);
  CHECK(positivePion.GetPz() == 4.);
  CHECK(positivePion.GetCharge() == -1);

  Particle::AddParticleType("k*", 0.89166, 0., 0.050);
  Particle resonance{"k*", 0, 0, 0};
  resonance.SetParticle("negativePion");
  CHECK(strcmp(resonance.GetName(), "negativePion") == 0);
  CHECK(resonance.GetIndex() == 2);

  // testing exceptions
  CHECK_THROWS_AS(negativePion.SetParticle(-1), std::runtime_error);
  CHECK_THROWS_AS(negativePion.SetParticle(5), std::runtime_error);
  CHECK_THROWS_AS(negativePion.SetParticle(10), std::runtime_error);
  CHECK_THROWS_AS(negativePion.SetParticle("positron"), std::runtime_error);
  Particle::AddParticleType("4", 0, 0, 0);
  Particle::AddParticleType("5", 0, 0, 0);
  Particle::AddParticleType("6", 0, 0, 0);
  Particle::AddParticleType("7", 0, 0, 0);
  Particle::AddParticleType("8", 0, 0, 0);
  Particle::AddParticleType("9", 0, 0, 0);
  CHECK_THROWS_AS(Particle::AddParticleType("10", 0, 0, 0), std::runtime_error);
}