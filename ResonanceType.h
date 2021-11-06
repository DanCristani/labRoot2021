#ifndef RESONANCETYPE_H
#define RESONANCETYPE_H

#include "ParticleType.h"

namespace simulation {

class ResonanceType : public ParticleType
{
  double fWidth;

 public:
  ResonanceType(const char* name, double mass, int charge, double width);
  double GetWidth() const;
  void Print() const;
};

}  // namespace simulation

#endif
