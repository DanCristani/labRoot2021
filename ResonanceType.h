#ifndef RESONANCETYPE_H
#define RESONANCETYPE_H

#include "ParticleType.h"

namespace simulation {

class ResonanceType : public ParticleType
{
 public:
  ResonanceType(const char* name, double mass, int charge, double width);
  ResonanceType();
  double GetWidth() const;
  void Print() const;

 private:
  double fWidth = 0;
};

}  // namespace simulation

#endif
