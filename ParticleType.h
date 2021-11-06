#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

namespace simulation {

class ParticleType
{
  const char* fName;
  double const fMass;
  int const fCharge;

 public:
  ParticleType(const char* name, double mass, int charge);
  const char* GetName() const;
  double GetMass() const;
  int GetCharge() const;
  virtual double GetWidth() const;
  virtual void Print() const;
};

}  // namespace simulation

#endif