#include <TMath.h>
#include <Riostream.h>
#include "TObject.h"
#include "Punto.h"
#include "Retta.h"
#include "Neutron.h"


ClassImp(Neutron)

/*Neutron::Neutron():TObject(),  //controllare, la direzine di partenza ha theta e phi uguali a zero!
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
  
}*/

Neutron::Neutron(const Neutron *n):TObject(),
  fp(n->fp),
  fr(n->fr),
  En(n->En),
  Absorption(n->Absorption){}

/*Neutron::Neutron(Punto *p,Retta *r,double Energy):TObject(),
  fp(p),
  fr(r),
  En(Energy),
  Absorption(kFALSE){

  }*/


Neutron::Neutron():TObject(),
  fp(Punto(-999,-999,-999)),
  fr(Retta(Punto(-999,-999,-999),999,999)),
  En(-1),
  Absorption(kFALSE){

  }


Neutron::~Neutron(){
  // distruttore
}



//---------------------------------------------------------------------------------------


void Neutron::Reset(){

  this->fp=Punto(-999,-999,-999);
  this->fr=Retta(Punto(-999,-999,-999),999,999);
  this->En=-1;
  this->Absorption=kFALSE;

}



void Neutron::SetNuovoPunto(double x_int){


  double xx=this->GetX()+(x_int*TMath::Sin(this->GetTheta())*TMath::Cos(this->GetPhi()));
  double yy=this->GetY()+(x_int*TMath::Sin(this->GetTheta())*TMath::Sin(this->GetPhi()));
  double zz=this->GetZ()+(x_int*TMath::Cos(this->GetTheta()));

  /*cout<<xx<<endl;
  cout<<yy<<endl;
  cout<<" "<<endl;*/

  this->fp.SetX(xx);
  this->fp.SetY(yy);
  this->fp.SetZ(zz);


}
