#ifndef RIVELATORE_H
#define RIVELATORE_H

#include "TObject.h"
#include "TRandom3.h"
#include "TFile.h"
#include "TH1.h"
#include "Retta.h"
#include "Punto.h"
#include "Neutron.h"
#include "TMath.h"





class Rivelatore : public TObject
{

 public:

  
  Rivelatore(double raggio, double x_centro, double y_centro, double z_centro);
  

  ~Rivelatore();

  double Intersezione(Neutron *n);
  double GetLength() const {return tp;}
  void SetLength(double ll){this->tp=ll;}
  double GetVolume() const {return (4/3)*TMath::Pi()*rp*rp*rp;}
  double GetFluence() const {return tp/this->GetVolume();}
  
 
 private:

  double rp;
  double xp;
  double yp;
  double zp;
  double tp;
 

  
  ClassDef(Rivelatore,1)
  
};

#endif
