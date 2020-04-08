#include <Riostream.h>
#include "TObject.h"
#include "TMath.h"
#include "Punto.h"


ClassImp(Punto)

Punto::Punto():TObject(),
  fX(0.),
  fY(0.),
  fZ(0.){
}



Punto::Punto(const Punto &p):TObject(),
  fX(p.GetX()),
  fY(p.GetY()),
  fZ(p.GetZ()){
}


Punto::Punto(const Punto *p):TObject(),
  fX(p->GetX()),
  fY(p->GetY()),
  fZ(p->GetZ()){
}



Punto::Punto(double X, double Y, double Z):TObject(),
  fX(X),
  fY(Y),
  fZ(Z){
}



Punto::~Punto(){
  // distruttore
}






