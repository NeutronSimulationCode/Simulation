#include <Riostream.h>
#include "TObject.h"
#include "TMath.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"
#include "Punto.h"
#include "Retta.h"
#include "Neutron.h"
#include "Generatore.h"



ClassImp(Generatore)



Generatore::Generatore(int N_particles,double E_iniziale,double dx_beam, double dy_beam, double x0, double y0):TObject(),
  Np(N_particles),
  Ep(E_iniziale),
  dy_p(dy_beam),
  dx_p(dx_beam),
  y0p(y0),
  x0p(x0){
}


Generatore::~Generatore(){
  // distruttore
 
}



//-------------------------------------------


void Generatore::Genera_neutrone(Neutron *n){

  double x0=this->x0p+(this->dx_p/2-this->dx_p*gRandom->Rndm()); 
  double y0=this->y0p+(this->dy_p/2-this->dy_p*gRandom->Rndm());
  double z0=0; 

  n->SetPunto(Punto(x0,y0,z0));
  n->SetRetta(Retta(Punto(x0,y0,z0),0,0));
  n->SetEnergia(this->Ep);
  
  return;


}
