#ifndef RETTA_H
#define RETTA_H

#include "TObject.h"
#include "Punto.h"


class Retta : public TObject
{

 public:

  Retta();
  Retta(const Retta&);
  Retta(const Retta *r);
  Retta(Punto p, double th, double ph);
  Retta(Punto *p, double th, double ph);
 

  ~Retta();

  Punto GetPunto() const {return fp;}
  double GetX() const {return fp.GetX();} 
  double GetY() const {return fp.GetY();}  
  double GetZ() const {return fp.GetZ();}
  double GetTheta() const {return fth;}
  double GetPhi() const {return fph;}
  void SetTheta(double theta){this->fth=theta;}
  void SetPhi(double phi){this->fph=phi;}

  
 private:
  
  Punto fp;
  double fth;
  double fph;

  ClassDef(Retta,1)

};

#endif
