//#if !defined(__CINT__) || defined(__MAKECINT__))
#include "Generatore.h"
#include "Neutron.h"
#include "Propagatore.h"
#include "Punto.h"
#include "Retta.h"
#include "Rivelatore.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TStopwatch.h"
#include <Riostream.h>
#include <TBranch.h>
#include <TObject.h>
#include <TSystem.h>
#include <TTree.h>

#include <chrono>
#include <omp.h>
#include <thread>
#define MAX omp_get_max_threads()

// Function that prints a status bar for done/total status
void printStatusBar(int done, int total) {
  std::cout.flush();
  std::cout << "[";
  bool symbol = true;
  for (int k = 1; k <= 50; k++) {
    if (static_cast<float>(done) / total > k / 50.)
      std::cout << "=";
    else {
      if (symbol) {
        std::cout << ">";
        symbol = false;
        continue;
      } else
        std::cout << " ";
    }
  }
  std::cout << "] " << std::trunc((static_cast<float>(done) * 100) / total)
            << "%\r";
}

// Simulates the propagation of neutrons
int main() {

  using namespace std;
  omp_set_nested(1);

  int threadsToUse = 2;
  cout << "Working on " << threadsToUse << " threads" << endl;

  TStopwatch *watch = new TStopwatch();
  double time = 0;
  watch->Start();

  TFile *f0 = new TFile("CrossSections/Cabs.root", "READ");
  TFile *f1 = new TFile("CrossSections/Habs.root", "READ");
  TFile *f2 = new TFile("CrossSections/Cscat.root", "READ");
  TFile *f3 = new TFile("CrossSections/Hscat.root", "READ");

  TH1F *h_Cabs = dynamic_cast<TH1F *>(f0->Get("C_absorption"));
  TH1F *h_Habs = dynamic_cast<TH1F *>(f1->Get("H_absorption"));
  TH1F *h_Cscatt = dynamic_cast<TH1F *>(f2->Get("C_scattering"));
  TH1F *h_Hscatt = dynamic_cast<TH1F *>(f3->Get("H_scattering"));

  // Define if the input parameters should be taken from std::cin
  bool inputFromCommandLine = false;

  // Polyethilene atomic densities
  const double hydrogen_density = 8e22;
  const double carbon_density = 4e22;

  // Simulation Parameters
  double Estart = 1000000;
  double Nstart = 10000000;
  double lateral_size = 1000;
  double thick = 10;
  double beam_size = 100;
  double y_start = 0; // y_start e x_start sono le posizioni del centro del
                      // fascio, che per ora lascio a zero
  double x_start = 0;
  double radius = 50;
  double shield_sphere_dist = 100;

  double seed = 0.1;
  gRandom->SetSeed(seed); // questo chiaramente non sta funzionando ...

  if (inputFromCommandLine) {
    cout << "Initial Beam Energy (eV)" << endl;
    cin >> Estart;
    cout << "Number of neutrons " << endl;
    cin >> Nstart;
    cout << "Shield lateral size (mm) " << endl;
    cin >> lateral_size;
    cout << "Shield thickness (mm) " << endl;
    cin >> thick;
    cout << "Beam lateral size (mm) " << endl;
    cin >> beam_size;
    cout << "Radius of the spherical detector (mm) " << endl;
    cin >> radius;
    cout << "Distance shield-sphere centre (mm) " << endl;
    cin >> shield_sphere_dist;
  }

  // Propagatore, Generatore and Rivelatore are the objects that take care of
  // the simulation
  Propagatore *prop =
      new Propagatore(h_Cscatt, h_Hscatt, h_Cabs, h_Habs, lateral_size, thick,
                      hydrogen_density, carbon_density);
  Generatore *gen =
      new Generatore(Nstart, Estart, beam_size, beam_size, x_start, y_start);
  Rivelatore *riv =
      new Rivelatore(radius, 0, 0, shield_sphere_dist + prop->GetTargetThick());

  double x_vec[461]; // vettore di bin energetici di diverse dimensioni

  // Custom binning for energy histograms
  for (int i = 0; i <= 100; i++) {
    x_vec[i] = i;
  }

  for (int j = 1; j <= 90; j++) {
    x_vec[j + 100] = 100 + j * 10;
  }

  for (int j = 1; j <= 90; j++) {
    x_vec[j + 190] = 1000 + j * 100;
  }

  for (int j = 1; j <= 90; j++) {
    x_vec[j + 280] = 10000 + j * 1000;
  }

  for (int j = 1; j <= 90; j++) {
    x_vec[j + 370] = 100000 + j * 10000;
  }

  // TFile *outFile = new TFile("neutronTree.root", "RECREATE");
  // TTree *tree = new TTree("tree", "tree di neutroni uscenti");

  // Read k-coefficients for dose computation from file
  ifstream myReadFile;
  myReadFile.open("hist_k");
  // double k_coeff[461];  //i k_coeff son in pSv*mm^2   (Valeria ce li ha dati
  // in pSv*cm^2)

  double x_energy[41];
  double y_kcoeff[41];

  for (int i = 0; i < 41; i++) {

    double x;
    double y;
    myReadFile >> x >> y;
    x_energy[i] = x;
    y_kcoeff[i] = y * 100; // passo da pSv*cm^2 a pSv*mm^2
  }

  TH1D *hist_k = new TH1D("hist_k", "hist_k", 40, x_energy); // 41

  for (int i = 0; i < 40; i++) {

    hist_k->Fill(x_energy[i], y_kcoeff[i]);
  }

  // tree->Branch("n_in", n_in, 32000, 2);
  // tree->Branch("n_out", &n_out, 32000, 2);

  // Book histograms
  TH1D *spectrum = new TH1D("spectrum", "spectrum", 460,
                            x_vec); // x_vec deve avere dimensione nbins+1 !!!!
  TH1D *abs_spectrum = new TH1D("abs_spectrum", "abs_spectrum", 460, x_vec);

  // Perform the simulation
  double dose = 0;
  int simulatedEvents = 0;
  bool done = false;
#pragma omp parallel sections shared(simulatedEvents, done) num_threads(2)
  {
    // Perform some monitoring of the process
#pragma omp section
    {
      if (omp_get_thread_num() == 0) {
        while (!done) {
          printStatusBar(simulatedEvents, gen->GetParticles());
          std::this_thread::sleep_for(chrono::microseconds(50));
        }
      }
    }
#pragma omp section
    {
#pragma omp parallel num_threads(threadsToUse)
      {
        // Take care of the simulation of the neutrons
#pragma omp for
        for (int i = 0; i < gen->GetParticles(); i++) {
          Neutron *n_in = new Neutron();
          Neutron *n_out = new Neutron();
          gen->Genera_neutrone(n_in);
          *n_out = Neutron(n_in);
          prop->Propagation(n_out);

          double length = riv->Intersezione(n_out);

// Save the simulation results in histograms
#pragma omp critical
          {
            // Not parallelizable
            if (n_out->GetAbsorption() && (n_out->GetEnergy() > 0))
              abs_spectrum->Fill(n_out->GetEnergy());
            if ((n_out->GetEnergy() > 0) && !(n_out->GetAbsorption()) &&
                length != 0) {
              int bin = spectrum->FindBin(n_out->GetEnergy());
              int bin2 = hist_k->FindBin(n_out->GetEnergy());
              double deltaE = spectrum->GetBinWidth(bin);
              double kc = hist_k->GetBinContent(bin2);
              spectrum->Fill(
                  n_out->GetEnergy(),
                  ((length / riv->GetVolume()) / deltaE) /
                      Nstart); // fluence spectrum per starting particle
              dose += (((length / riv->GetVolume())) / Nstart) * kc;
              // tree->Fill();
            }
            simulatedEvents++;
            delete n_in;
            delete n_out;
          }
        }
      }
      done = true;
    }
  }

  watch->Stop();
  time = watch->RealTime();

  /* double dose;

   for(int i=0;i<461;i++){

     dose +=k_coeff[i]*spectrum->GetBinContent(i)*spectrum->GetBinWidth(i);

   }*/

  // tree->Write();

  // Display histograms and save them in png files
  TCanvas *c1 = new TCanvas("c1", "c1", 1200, 800);
  c1->cd();
  spectrum->GetXaxis()->SetTitle("Energy (eV)");
  spectrum->GetYaxis()->SetTitle("Fluence Spectrum #Delta#phi/#Delta E");
  spectrum->Draw("HIST");
  c1->SetLogx();
  c1->SetLogy();
  c1->Print("FluenceSpectrum.png");

  TCanvas *c2 = new TCanvas("c2", "c2", 1200, 800);
  c2->cd();
  abs_spectrum->SetTitle("Energy Spectrum of Neutrons Absorbed in PE");
  abs_spectrum->GetXaxis()->SetTitle("Energy (eV)");
  abs_spectrum->GetYaxis()->SetTitle("# counts");
  abs_spectrum->Draw("HIST");
  c2->SetLogx();
  c2->SetLogy();
  c2->Print("EnergySpectrumAbsorbed.png");

  delete c1;
  delete c2;

  // myReadFile.close();
  // outFile->Close();

  // Print some simulation results & performances
  cout << " " << endl;
  cout << "Fluence per Starting Particle: " << riv->GetFluence() / Nstart
       << endl;
  cout << " " << endl;
  cout << "Dose: " << dose << endl;
  cout << " " << endl;
  cout << "Mean number of collisions: " << prop->GetNcoll() / Nstart << endl;
  cout << " " << endl;
  cout << "Mean number of collisions before Absorption: "
       << prop->GetAbscoll() / prop->GetAssorbiti() << endl;
  cout << " " << endl;
  cout << "Mean CPU time per neutron: " << time / Nstart << endl;
  cout << " " << endl;
  cout << "Total CPU time: " << time << endl;
  cout << " " << endl;

  return 0;
}