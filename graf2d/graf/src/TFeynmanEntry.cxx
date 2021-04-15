#include <cstdio>
#include <iostream>
#include "../inc/TFeynmanEntry.h"

ClassImp(TFeynman);


TFeynmanEntry::TFeynmanEntry(const char* particleName, Double_t x1, Double_t y1, Double_t x2, Double_t y2) {
    fX1 = x1;
    fY1 = y1;
    fX2 = x2;
    fY2 = y2;
    fParticle = particleName;
}
void TFeynmanEntry::Paint() {
  // Get all the needed values:
  cout << "Paint method of TFeynmanEntry called" << endl;
  Double_t x1 = GetX1();
  Double_t y1 = GetY1();
  Double_t x2 = GetX2();
  Double_t y2 = GetY2();
  const char* particleName = GetParticleName();

  if (particleName == std::string("fermion")) {
    cout << "Fermion" << endl;
    TArrow *fermion = new TArrow(x1, y1, x2, y2, 0.05, "->-");
    fermion->Paint();
  }
  else{
    cout << "Nope" << endl;
  }
}
