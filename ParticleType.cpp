#include <iostream>

#include "ParticleType.h"

namespace simulation {

ParticleType::ParticleType(const char* name, double mass, int charge)
    : fName(name), fMass(mass), fCharge(charge)
{
}

const char* ParticleType::GetName() const
{
  return fName;
}

double ParticleType::GetMass() const
{
  return fMass;
}

int ParticleType::GetCharge() const
{
  return fCharge;
}

double ParticleType::GetWidth() const
{
  return 0.;
}

void ParticleType::Print() const
{
  std::cout << "Particle name: " << GetName() << '\n';
  std::cout << "Mass = " << GetMass() << '\n';
  std::cout << "Charge = " << GetCharge() << '\n';
}

}  // namespace simulation