#ifndef PARTICLE_H
#define PARTICLE_H

#include "ParticleType.h"
#include "ResonanceType.h"

namespace simulation {

class Particle
{
 public:
  // function to create the list of possible particles
  static void AddParticleType(const char* name,
                              double const mass,
                              int const charge,
                              double width = 0);

  // constructors
  Particle(const char* name, double Px = 0, double Py = 0, double Pz = 0);

  // getters
  const char* GetName() const;
  int GetIndex() const;
  double GetPx() const;
  double GetPy() const;
  double GetPz() const;
  double GetMass() const;
  int GetCharge() const;
  double Energy() const;
  double InvariantMass(Particle& other) const;

  // setters
  void SetParticle(int index);
  void SetParticle(const char* name);
  void SetP(double Px, double Py, double Pz);

  // print
  static void PrintParticleTypes();
  void Print() const;

  // decay function
  int Decay2body(Particle& dau1, Particle& dau2) const;

 private:
  // static data members
  static const int fMaxNumParticleType = 10;
  static ParticleType* fParticleType[fMaxNumParticleType];
  static int fNParticleType;

  // data members
  int fIndex;
  double fPx;
  double fPy;
  double fPz;

  // methods
  static int FindParticle(const char* name);
  void Boost(double bx, double by, double bz);
};

}  // namespace simulation

#endif