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
#include "Rivelatore.h"



ClassImp(Rivelatore)



Rivelatore::Rivelatore(double raggio, double x_centro, double y_centro, double z_centro):TObject(),
  rp(raggio),
  xp(x_centro),
  yp(y_centro),
  zp(z_centro),
  tp(0){
  
}


Rivelatore::~Rivelatore(){
  // distruttore
 
}



//-------------------------------------------




double Rivelatore::Intersezione(Neutron *n){

  
      if( n->GetZ()>=0 ){
    
          double vx=TMath::Sin(n->GetTheta())*TMath::Cos(n->GetPhi());
	  double vy=TMath::Sin(n->GetTheta())*TMath::Sin(n->GetPhi());
	  double vz=TMath::Cos(n->GetTheta());

	  double beq=2*(vx*(n->GetX()-this->xp)+vy*(n->GetY()-this->yp)+vz*(n->GetZ()-this->zp));
	  double ceq=TMath::Power(n->GetX()-this->xp,2)+TMath::Power(n->GetY()-this->yp,2)+TMath::Power(n->GetZ()-this->zp,2)-(this->rp*this->rp);

	  double delta=beq*beq-4*ceq;
	  double t_1=(0.5)*(-beq+TMath::Sqrt(delta));
	  double t_2=(0.5)*(-beq-TMath::Sqrt(delta));

	  
	   if(delta>0){ this->tp+=TMath::Abs(t_1-t_2);
	     return TMath::Abs(t_1-t_2); }else{return 0;}
	   
		      

  }
  return 0;
}
