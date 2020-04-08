#ifndef NEUTRON_H
#define NEUTRON_H

#include "TObject.h"
#include "Retta.h"
#include "Punto.h"

class Neutron : public TObject
{

 public:

  //eutron();
  //Neutron(const Neutron&);
  Neutron(const Neutron *n);
  //Neutron(Punto *p, Retta *r, double Energy);
  Neutron();
  



  ~Neutron();

  Punto GetPunto() const {return fp;}
  Retta GetRetta() const {return fr;}
  double GetX() const {return fp.GetX();}  //GetY
  double GetY() const {return fp.GetY();}  //GetX
  double GetZ() const {return fp.GetZ();}
  double GetTheta() const {return fr.GetTheta();}
  double GetPhi() const {return fr.GetPhi();}
  double GetEnergy() const {return En;}
  bool GetAbsorption() const {return Absorption;}
  void SetPunto(Punto fph){this->fp=fph;}
  void SetRetta(Retta frh){this->fr=frh;}
  void SetTheta(double theta){this->fr.SetTheta(theta);}
  void SetPhi(double phi){this->fr.SetPhi(phi);}
  void SetEnergia(double Enh){this->En=Enh;}
  void SetAbsorption(){this->Absorption=kTRUE;}
  void SetNuovoPunto(double x_int);
  void Reset();

  
 private:
  
  Punto fp;
  Retta fr;
  double En;
  bool Absorption;
  

  ClassDef(Neutron,1)

};

#endif
