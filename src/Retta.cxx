#include <TMath.h>
#include <Riostream.h>
#include "TObject.h"
#include "Punto.h"
#include "Retta.h"




ClassImp(Retta)


Retta::Retta():TObject(),
  fth(TMath::Pi()*0.5),
  fph(0.){
 
}



Retta::Retta(const Retta&r):TObject(),
  fp(r.fp),
  fth(r.fth),
  fph(r.fph){  
 
}



Retta::Retta(const Retta *r):TObject(),
  fp(r->fp),
  fth(r->fth),
  fph(r->fph){  
 
}



Retta::Retta(Punto p, double teta, double phi):TObject(),
  fp(p),
  fth(teta),
  fph(phi){
  
}



Retta::Retta(Punto *p, double teta, double phi):TObject(),
  fp(*p),
  fth(teta),
  fph(phi){
  
}




Retta::~Retta() {
  // distruttore
}



//_____________________________________________________________________________
//_____________________________________________________________________________







