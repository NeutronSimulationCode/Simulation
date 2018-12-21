#include <TMath.h>
#include <Riostream.h>
#include "TObject.h"
#include "Punto.h"
#include "Retta.h"
#include "Neutron.h"


ClassImp(Neutron)

Neutron::Neutron():TObject(),  //controllare, la direzine di partenza ha theta e phi uguali a zero!
  fp(0,0,0),
  fr(fp,0,0),
  En(0.),
  Absorption(kFALSE){
}

Neutron::Neutron(const Neutron&n):TObject(),
  fp(n.fp),
  fr(n.fr),
  Absorption(n.Absorption),
  En(n.En){
  
}

Neutron::Neutron(const Neutron *n):TObject(),
  fp(n->fp),
  fr(n->fr),
  Absorption(n->Absorption),
  En(n->En){
  
}

Neutron::Neutron(Punto p,Retta r,double Energy):TObject(),
  fp(p),
  fr(r),
  En(Energy),
  Absorption(kFALSE){

  }


Neutron::Neutron(Punto *p,Retta *r,double Energy):TObject(),
  fp(*p),
  fr(*r),
  En(Energy),
  Absorption(kFALSE){

  }


Neutron::~Neutron(){
  // distruttore
}



//---------------------------------------------------------------------------------------

void Neutron::SetNuovoPunto(double x_int){


  double xx=this->GetX()+(x_int*TMath::Sin(this->GetTheta())*TMath::Cos(this->GetPhi()));
  double yy=this->GetY()+(x_int*TMath::Sin(this->GetTheta())*TMath::Sin(this->GetPhi()));
  double zz=this->GetZ()+(x_int*TMath::Cos(this->GetTheta()));

  this->fp.SetX(xx);
  this->fp.SetY(yy);
  this->fp.SetZ(zz);


}