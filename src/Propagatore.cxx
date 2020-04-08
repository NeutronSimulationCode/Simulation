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
#include "Propagatore.h"



ClassImp(Propagatore)
using namespace std;


Propagatore::Propagatore(TH1 *Cscatt, TH1 *Hscatt, TH1 *Cabs, TH1 *Habs, double target_side, double target_thickness, double hdensity, double cdensity):TObject(),
  Cscattp(Cscatt),
  Hscattp(Hscatt),
  Cabsp(Cabs),
  Habsp(Habs),
  ptarget_side(target_side),
  ptarget_thickness(target_thickness),
  hpdensity(hdensity),
  cpdensity(cdensity){
}


Propagatore::~Propagatore(){
  // distruttore
  delete Cscattp;
  delete Hscattp;;
  delete Cabsp;
  delete Habsp;
}



//_____________________________________________________________________________
//_____________________________________________________________________________




double Propagatore::x_interaction(double En){

  TAxis xaxis0=*(this->Cscattp->GetXaxis());
  int binx0=xaxis0.FindBin(En);
  double sigma_csc=this->Cscattp->GetBinContent(binx0)*this->cpdensity*(1e-24);

  TAxis xaxis1=*(this->Hscattp->GetXaxis());
  int binx1=xaxis1.FindBin(En);
  double sigma_hsc=this->Hscattp->GetBinContent(binx1)*this->hpdensity*(1e-24);

  TAxis xaxis2=*(this->Cabsp->GetXaxis());
  int binx2=xaxis2.FindBin(En);
  double sigma_cab=this->Cabsp->GetBinContent(binx2)*this->cpdensity*(1e-24);

  TAxis xaxis3=*(this->Habsp->GetXaxis());
  int binx3=xaxis3.FindBin(En);
  double sigma_hab=this->Habsp->GetBinContent(binx3)*this->hpdensity*(1e-24);


  double sigma_tot=sigma_csc+sigma_hsc+sigma_cab+sigma_hab;

  //double xint=-(1./sigma_tot)*log(1-gRandom->Rndm())*10;
  //cout<<xint<<endl;


  return -(1./sigma_tot)*log(1-gRandom->Rndm())*10; //moltiplico per 10 per passare da cm a mm (la sigma macroscopica è in 1/cm )


}



//_____________________________________________________________________________



Neutron* Propagatore::scattering(Neutron *n){ //Ricevo un warning perchè potrei non ritornare nulla, ma non dovrebbe essere un problema

  TAxis xaxis0=*(this->Cscattp->GetXaxis());
  int binx0=xaxis0.FindBin(n->GetEnergy());
  double sigma_csc=this->Cscattp->GetBinContent(binx0)*this->cpdensity*(1e-24);

  TAxis xaxis1=*(this->Hscattp->GetXaxis());
  int binx1=xaxis1.FindBin(n->GetEnergy());
  double sigma_hsc=this->Hscattp->GetBinContent(binx1)*this->hpdensity*(1e-24);

  TAxis xaxis2=*(this->Cabsp->GetXaxis());
  int binx2=xaxis2.FindBin(n->GetEnergy());
  double sigma_cab=this->Cabsp->GetBinContent(binx2)*this->cpdensity*(1e-24);

  TAxis xaxis3=*(this->Habsp->GetXaxis());
  int binx3=xaxis3.FindBin(n->GetEnergy());
  double sigma_hab=this->Habsp->GetBinContent(binx3)*this->hpdensity*(1e-24);

  double sigma_tot=sigma_csc+sigma_hsc+sigma_cab+sigma_hab;
  double sigma_abs=sigma_cab+sigma_hab;

  double absorption_limit=sigma_abs/sigma_tot;
  double hidrogen_scatt=sigma_hsc/sigma_tot;

  double alfa_cm=TMath::ACos(1.-2*gRandom->Rndm());  //angolo theta nel CM, uniforme tra -1 ed 1

  double interaction_type=gRandom->Rndm();

  /*cout<<sigma_csc<<endl;
  cout<<sigma_hsc<<endl;
  cout<<sigma_cab<<endl;
  cout<<sigma_hab<<endl;
  cout<<" "<<endl;*/

  //if(sigma_csc==0 || sigma_hsc==0 || sigma_cab==0 || sigma_hab==0) cout<<"pippo"<<endl;

  double new_energy;
  double delta_th;
  double new_theta;
  double delta_phi;
  double new_phi;



  if(interaction_type<=absorption_limit){
    n->SetAbsorption();
    return n;
  }

  if(interaction_type>absorption_limit && interaction_type<=(absorption_limit+hidrogen_scatt)){  //il nucleo scelto è l'idrogeno

    new_energy=n->GetEnergy()*(0.5)*(1.+TMath::Cos(alfa_cm));

    //new_phi=2*TMath::Pi()*gRandom->Rndm();
    //new_theta=delta_th+n->GetTheta();
    delta_th=TMath::ACos((TMath::Cos(alfa_cm)+1.)/(TMath::Sqrt(2.+(2.)*TMath::Cos(alfa_cm))));
    delta_phi=2*TMath::Pi()*gRandom->Rndm();
    //delta_phi=0;

    //if(pippo>0.5) delta_th=-delta_th;

    double cd[3];
    double cdp[3];
    double mr[3][3];

    double phi=n->GetPhi();
    double theta=n->GetTheta();

    mr[0][0]=-TMath::Sin(phi);
    mr[1][0]=TMath::Cos(phi);
    mr[2][0]=0.;
    mr[0][1]=-TMath::Cos(phi)*TMath::Cos(theta);
    mr[1][1]=-TMath::Cos(theta)*TMath::Sin(phi);
    mr[2][1]=TMath::Sin(theta);
    mr[0][2]=TMath::Sin(theta)*TMath::Cos(phi);
    mr[1][2]=TMath::Sin(theta)*TMath::Sin(phi);
    mr[2][2]=TMath::Cos(theta);


    cdp[0]=TMath::Sin(delta_th)*TMath::Cos(delta_phi);
    cdp[1]=TMath::Sin(delta_th)*TMath::Sin(delta_phi);
    cdp[2]=TMath::Cos(delta_th);

    for(int i=0;i<3;i++){
      cd[i]=0.;
      for(int j=0;j<3;j++){
        cd[i]+=mr[i][j]*cdp[j];
      }
    }


    new_theta=TMath::ACos(cd[2]);

    new_phi=TMath::ATan2(cd[1],cd[0]);  //new_phi=TMath::ATan2(cd[1]/cd[0]);
    //new_phi=2*TMath::Pi()*gRandom->Rndm();

    //if(cd[0]>0 && cd[1]<0) new_phi+=2*TMath::Pi();
    //if(cd[0]<0)  new_phi=TMath::Pi()-new_phi;   //new_phi+=TMath::Pi();

    n->SetEnergia(new_energy);
    n->SetTheta(new_theta);
    n->SetPhi(new_phi);


    return n;
  }

  if(interaction_type>(absorption_limit+hidrogen_scatt)){  //il nucleo scelto è il carbonio

    new_energy=n->GetEnergy()*(1./169.)*(145.+24*TMath::Cos(alfa_cm));

    //new_phi=2*TMath::Pi()*gRandom->Rndm();
    //new_theta=delta_th+n->GetTheta();
    delta_th=TMath::ACos(((12.)*TMath::Cos(alfa_cm)+1.)/(TMath::Sqrt(145.+(24.)*TMath::Cos(alfa_cm))));
    delta_phi=2*TMath::Pi()*gRandom->Rndm();
    //delta_phi=0;

    // if(pippo>0.5) delta_th=-delta_th;

    double cd[3];
    double cdp[3];
    double mr[3][3];

    double phi=n->GetPhi();
    double theta=n->GetTheta();

    mr[0][0]=-TMath::Sin(phi);
    mr[1][0]=TMath::Cos(phi);
    mr[2][0]=0.;
    mr[0][1]=-TMath::Cos(phi)*TMath::Cos(theta);
    mr[1][1]=-TMath::Cos(theta)*TMath::Sin(phi);
    mr[2][1]=TMath::Sin(theta);
    mr[0][2]=TMath::Sin(theta)*TMath::Cos(phi);
    mr[1][2]=TMath::Sin(theta)*TMath::Sin(phi);
    mr[2][2]=TMath::Cos(theta);


    cdp[0]=TMath::Sin(delta_th)*TMath::Cos(delta_phi);
    cdp[1]=TMath::Sin(delta_th)*TMath::Sin(delta_phi);
    cdp[2]=TMath::Cos(delta_th);


    for(int i=0;i<3;i++){
      cd[i]=0.;
      for(int j=0;j<3;j++){
        cd[i]+=mr[i][j]*cdp[j];
      }
    }


    new_theta=TMath::ACos(cd[2]);
    new_phi=TMath::ATan2(cd[1],cd[0]);  //new_phi=TMath::ATan2(cd[1]/cd[0]);
    //new_phi=2*TMath::Pi()*gRandom->Rndm();

    //if(cd[0]>0 && cd[1]<0) new_phi+=2*TMath::Pi();
    //if(cd[0]<0 )new_phi=TMath::Pi()-new_phi;   //new_phi+=TMath::Pi();


    n->SetEnergia(new_energy);
    n->SetTheta(new_theta);
    n->SetPhi(new_phi);

    //cout<<new_theta<<" "<<n->GetRetta().GetTheta()<<endl;
    //cout<<new_phi<<" "<<n->GetRetta().GetPhi()<<endl;

    return n;
  }

  cout << "ATTENZIONE: TIPO DI INTERAZIONE NON RICONOSCIUTO" << endl;
  return n;
}




void Propagatore::Propagation(Neutron *n){

  bool contr=kTRUE;
  bool verbosity=kFALSE;
  double n_coll=0;

  while(!(n->GetAbsorption()) && contr){

    if(verbosity){ cout<<"x "<<n->GetX()<<endl;
    cout<<"y "<<n->GetY()<<endl;
    cout<<"z "<<n->GetZ()<<endl;
    cout<<"th "<<n->GetTheta()<<endl;
    cout<<"ph "<<n->GetPhi()<<endl;
    cout<<"Energia "<<n->GetEnergy()<<endl;
    cout<<" "<<endl;}


      n->SetNuovoPunto(this->x_interaction(n->GetEnergy()));


      if( n->GetZ()>=0 && n->GetZ()<=this->ptarget_thickness && n->GetY()>=-(0.5)*this->ptarget_side &&  n->GetY()<=(0.5)*this->ptarget_side && n->GetX()>=-(0.5)*this->ptarget_side && n->GetX()<=(0.5)*this->ptarget_side ){

	this->scattering(n);
	this->pncollision+=1;
	n_coll+=1;

      }else{contr=kFALSE; }


 }

  if(n->GetAbsorption()){

    this->abs_collision+=n_coll;
    this->assorbiti+=1;

  }

 return;


}


//_____________________________________________________________________________


//matrice per passare da CM a LAB
/*void Rivelatore::Rotate(double theta, double phi, double theta0, double phi0){


  double mr[3][3];

  mr[0][0]=-TMath::Sin(phi);
  mr[1][0]=TMath::Cos(phi);
  mr[2][0]=0.;
  mr[0][1]=-TMath::Cos(phi)*TMath::Cos(theta);
  mr[1][1]=-TMath::Cos(theta)*TMath::Sin(phi);
  mr[2][1]=TMath::Sin(theta);
  mr[0][2]=TMath::Sin(theta)*TMath::Cos(phi);
  mr[1][2]=TMath::Sin(theta)*TMath::Sin(phi);
  mr[2][2]=TMath::Cos(theta);

  double cdp[3];
  cdp[0]=TMath::Sin(theta0)*TMath::Cos(phi0);
  cdp[1]=TMath::Sin(theta0)*TMath::Sin(phi0);
  cdp[2]=TMath::Cos(theta0);

  for(int i=0;i<3;i++){
    cd[i]=0.;
    for(int j=0;j<3;j++){
      cd[i]+=mr[i][j]*cdp[j];
    }
    }




    }*/



//dir[0]=TMath::Sin(teta)*TMath::Cos(phi);
//dir[1]=TMath::Sin(teta)*TMath::Sin(phi);
//dir[2]=TMath::Cos(teta);
