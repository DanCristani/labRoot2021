#include <iostream>

#include "ResonanceType.h"

namespace simulation {

ResonanceType::ResonanceType(const char* name, double mass, int charge, double width)
    : ParticleType(name, mass, charge), fWidth(width) {}

ResonanceType::ResonanceType() = default;

double ResonanceType::GetWidth() const { return fWidth; }

void ResonanceType::Print() const
{
  ParticleType::Print();
  std::cout << "Width = " << GetWidth() << '\n';
}

}  // namespace simulation