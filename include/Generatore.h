#ifndef GENERATORE_H
#define GENERATORE_H

#include "TObject.h"
#include "TRandom3.h"
#include "TFile.h"
#include "TH1.h"
#include "Retta.h"
#include "Punto.h"
#include "Neutron.h"





class Generatore : public TObject
{

 public:

  //ho definito un generatore con x0 già settato a zero
  Generatore(int N_particles,double E_iniziale,double dx_beam, double dy_beam, double x0, double y0);
  

  ~Generatore();

  
  void Genera_neutrone(Neutron *n);
  int GetParticles() const {return Np;}
  double GetEn() const {return Ep;}
  double Getdy() const {return dy_p;}
  double Getdx() const {return dx_p;}
  double Gety0() const {return y0p;}
  double Getx0() const {return x0p;}
  
  
 private:

  int Np;
  double Ep;
  double dy_p;
  double dx_p;
  double y0p;
  double x0p;
 

  
  ClassDef(Generatore,1)
  
};

#endif
