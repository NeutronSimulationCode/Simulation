#ifndef PROPAGATORE_H
#define PROPAGATORE_H

#include "TObject.h"
#include "TRandom3.h"
#include "TFile.h"
#include "TH1.h"
#include "Retta.h"
#include "Punto.h"
#include "Neutron.h"





class Propagatore : public TObject
{

 public:

  //Propagatore();
//sto assumendo un target di sezione quadrata (ecco perchè solo un target_side)
  Propagatore(TH1 *Cscatt, TH1 *Hscatt, TH1 *Cabs, TH1 *Habs, double target_side, double target_thickness, double hdensity, double cdensity);
  

  ~Propagatore();

  
  double x_interaction(double En);
  Neutron* scattering(Neutron *n);
  void Propagation(Neutron *n); 
  double GetTargetThick() const {return ptarget_thickness;}
  double GetNcoll() const {return pncollision;}
  double GetAbscoll() const {return abs_collision;}
  double GetAssorbiti() const {return assorbiti;}

  
 private:

  const TH1 *Cscattp;
  const TH1 *Hscattp;
  const TH1 *Cabsp;
  const TH1 *Habsp;
  double ptarget_side;
  double ptarget_thickness;
  double hpdensity;
  double cpdensity;
  double pncollision;
  double abs_collision;
  double assorbiti;
  
  ClassDef(Propagatore,1)
  
};

#endif
