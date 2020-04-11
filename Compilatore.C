#include <Riostream.h>
#include <TObject.h>
#include <TSystem.h>
#include "TStopwatch.h"
#include "TMath.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TCanvas.h"
#include <TTree.h>
#include <TBranch.h>
#include "TClonesArray.h"
#include "TRandom3.h"
#include "Punto.h"
#include "Retta.h"
#include "Neutron.h"
#include "Propagatore.h"
#include "Generatore.h"
#include "Rivelatore.h"


void Compilatore(){
  gSystem->CompileMacro("Punto.cxx","kg");
  gSystem->CompileMacro("Retta.cxx","kg");
  gSystem->CompileMacro("Neutron.cxx","kg");
  gSystem->CompileMacro("Generatore.cxx","kg");
  gSystem->CompileMacro("Propagatore.cxx","kg");
  gSystem->CompileMacro("Rivelatore.cxx","kg");
  gSystem->CompileMacro("Simulatore.C","kg");

}
