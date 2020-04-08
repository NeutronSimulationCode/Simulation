#ifndef PUNTO_H
#define PUNTO_H

#include "TObject.h"

class Punto : public TObject
{

 public:

  Punto();
  Punto(const Punto &p);
  Punto(const Punto *p);
  Punto(double X, double Y, double Z);

    ~Punto();

  double GetX() const {return fX;}
  double GetY() const {return fY;}
  double GetZ() const {return fZ;}
  void SetX(double xx){this->fX=xx;}
  void SetY(double yy){this->fY=yy;}
  void SetZ(double zz){this->fZ=zz;}
  
 private:

  double fX;
  double fY;
  double fZ;
    

  ClassDef(Punto,1)
    
};

#endif
