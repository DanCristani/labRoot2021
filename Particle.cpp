#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "Particle.h"

namespace simulation {

// static members initialization
ParticleType* Particle::fParticleType[] = {};
int Particle::fNParticleType = 0;

// public methods
// function to create the list of possible particles
void Particle::AddParticleType(const char* name,
                               double const mass,
                               int const charge,
                               double width)
{
  if (FindParticle(name) != -1) {
    throw std::runtime_error("Particle already present");
  }
  if (fNParticleType == 10) {
    throw std::runtime_error("Particle array is full");
  }
  if (width == 0) {
    fParticleType[fNParticleType] = new ParticleType(name, mass, charge);
  } else {
    fParticleType[fNParticleType] =
        new ResonanceType(name, mass, charge, width);
  }
  ++fNParticleType;
}

// constructors
Particle::Particle(const char* name, double Px, double Py, double Pz)
    : fPx(Px), fPy(Py), fPz(Pz)
{
  const int index = FindParticle(name);
  if (index == -1) {
    throw std::runtime_error("There's no particle with this name");
  }
  fIndex = index;
}

// getters
const char* Particle::GetName() const
{
  return fParticleType[fIndex]->ParticleType::GetName();
}

int Particle::GetIndex() const
{
  return fIndex;
}

double Particle::GetPx() const
{
  return fPx;
}

double Particle::GetPy() const
{
  return fPy;
}

double Particle::GetPz() const
{
  return fPz;
}

double Particle::GetMass() const
{
  return fParticleType[fIndex]->ParticleType::GetMass();
}

int Particle::GetCharge() const
{
  return fParticleType[fIndex]->ParticleType::GetCharge();
}

double Particle::Energy() const
{
  double mass = GetMass();
  double P_squared = fPx * fPx + fPy * fPy + fPz * fPz;
  return sqrt(mass * mass + P_squared);
}

double Particle::InvariantMass(Particle& other) const
{
  double P1 = sqrt(fPx * fPx + fPy * fPy + fPz * fPz);
  double P2 =
      sqrt(other.GetPx() * other.GetPx() + other.GetPy() * other.GetPy() +
           other.GetPz() * other.GetPz());
  return sqrt((Energy() + other.Energy()) *
                  (Energy() + other.Energy()) -
              (P1 + P2) * (P1 + P2));
}

// setters
void Particle::SetParticle(int index)
{
  if (index < 0) {
    throw std::runtime_error("You can't have a particle with a negative index");
  }
  if (index >= fMaxNumParticleType) {
    throw std::runtime_error("You have a maximum of 10 elements in the array");
  }
  if (index >= fNParticleType) {
    throw std::runtime_error("There's no particle with this index");
  }
  fIndex = index;
}

void Particle::SetParticle(const char* name)
{
  int index = FindParticle(name);
  assert(index < fMaxNumParticleType);
  if (index == -1) {
    throw std::runtime_error("There's no particle with this name");
  }
  fIndex = index;
}

void Particle::SetP(double Px, double Py, double Pz)
{
  fPx = Px;
  fPy = Py;
  fPz = Pz;
}

// print
void Particle::PrintParticleTypes()
{
  for (int i = 0; i < fNParticleType; ++i) {
    fParticleType[i]->Print();
  }
}

void Particle::Print() const
{
  std::cout << "Particle name: " << fParticleType[fIndex]->GetName() << '\n';
  std::cout << "Particle type index = " << fIndex << '\n';
  std::cout << "Particle momentum = (" << fPx << ", " << fPy << ", " << fPz
            << ")" << '\n';
}

// decay function
int Particle::Decay2body(Particle& dau1, Particle& dau2) const
{
  assert(fParticleType[fIndex]->ParticleType::GetWidth() != 0);
  if (GetMass() == 0.0) {
    std::cerr << "Decayment cannot be preformed if mass is zero" << '\n';
    return 1;
  }

  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if (fIndex > -1) {  // add width effect

    // gaussian random numbers

    float x1, x2, w, y1, y2;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;
    y2 = x2 * w;

    massMot += fParticleType[fIndex]->GetWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    std::cerr << "Decayment cannot be preformed because mass is too low in "
                 "this channel"
              << '\n';
    return 2;
  }

  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi),
            pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi),
            -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy = sqrt(fPx * fPx + fPy * fPy + fPz * fPz + massMot * massMot);

  double bx = fPx / energy;
  double by = fPy / energy;
  double bz = fPz / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}

// private methods
int Particle::FindParticle(const char* name)
{
  for (int i = 0; i < fNParticleType; ++i) {
    // check if name is present in the fParticleType array and return its index
    if (strcmp(fParticleType[i]->GetName(), name) == 0) {
      return i;
    }
  }
  return -1;
}

void Particle::Boost(double bx, double by, double bz)
{
  double energy = Energy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * fPx + by * fPy + bz * fPz;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  fPx += gamma2 * bp * bx + gamma * bx * energy;
  fPy += gamma2 * bp * by + gamma * by * energy;
  fPz += gamma2 * bp * bz + gamma * bz * energy;
}

}  // namespace simulation