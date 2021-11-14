#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

namespace simulation {

class ParticleType
{
 public:
  ParticleType(const char* name, double mass, int charge);
  ParticleType();
  const char* GetName() const;
  double GetMass() const;
  int GetCharge() const;
  virtual double GetWidth() const;
  virtual void Print() const;

 private:
  const char* fName = "null particle";
  double const fMass = 0.;
  int const fCharge = 0;
};

}  // namespace simulation

#endif