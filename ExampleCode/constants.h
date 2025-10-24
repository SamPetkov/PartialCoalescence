// All of the simulation parameters are included in this file
// Using a separate file provides us with the opportunity of easily importing it into a post-processing script

#include "axi.h" // Axisymetric simulations
#include "navier-stokes/centered.h" // NS solver

#include "vof.h" 
scalar f[], fdrop[], fbubble[],* interfaces = {f,fdrop,fbubble}; // Adding passive tracers for visualization
#include "two-phase-generic.h" // Two phase flow


#include "tension.h"

#include "view.h" //OpenGL based image saving module

#include "reduced.h" // Gravity is represented as an interfacial force

#include "tag.h"

// Geometry of the domain

#define diameter 1. // Numerical diameter
#define length 10. //  Numerical size of the domain

// Define all physical parameters

#define D 1.07e-3 // (m)      diameter of the droplet
#define Dbubble 0.1875*D // (m)   diameter of the bubble


#define wheight 7.*D // (m)    water layer's thickness
#define hdrop (wheight+D/2) //  (m)    position of the droplet's center

#define hdisplacement (0.5000 - (0.175+0.00625))*D // distance between centroids of bubble and drop

#define hbubble (hdrop + hdisplacement) // (m)   position of the bubble's center
#define V sqrt(sigma12/(rho_w*D)) //    (m/s)  Initial imapct velocity

#define Mu_w 0.00105 //      (Pa.s) // viscosity
#define Mu_g 1.85e-5 //     (Pa.s)
#define rho_w 786. //     (kg/m^3)  // density
#define rho_g 1.184 //       (kg/m^3)
#define sigma12 22e-3 //  (N/m)  Surface tension for an ethanol/air system at 
#define g 9.81//  	   (m^2/s)

//Dimensionless numbers

#define Oh (Mu_w/sqrt(rho_w*sigma12*D))
#define Bo (rho_w * g * sq(D) / sigma12)
#define We 1.

#define vrho_w 1.
#define vrho_g (rho_g/rho_w)
#define vmu_w Oh
#define vmu_g Oh*(Mu_g/Mu_w)
#define SIGMA 1./We
#define FR 1/sqrt(Bo)

//convert ratio between physical and numerical values

#define ratio diameter/D // convert all geometrical lengths in numerical value


// Convert all real values in numerical values

#define wheight_num wheight*ratio
#define hdrop_num hdrop*ratio
#define dbubble Dbubble*ratio 
#define hbubble_num hbubble*ratio


// Geometry of the system


// Droplet
#define droplet(x, y) (min( \
  -sq(x - hdrop_num) - sq(y) + sq(diameter/2), \
   sq(x - hbubble_num) + sq(y) - sq(dbubble/2) \
))

// Bubble
#define bubble(x,y) (-sq(x-hbubble_num)-sq(y)+sq(dbubble/2))

// Pool
#define pool(x,y) (7.-x)

#define epsilon 0.1*D
#define initwater(x,y) (-sq(x-hdrop_num)-sq(y)+sq(diameter/2+epsilon))

// Simulation parameters

// default = 12
int maxlevel = 12;
int minlevel = 6;


double dtuser = 0.001 [0,1,0];
double tmax = 1.0 [0,1,0];
double tc = 0. [0,1,0];// restart point

int frame = 0; // Frame counter

double tframe = 0. [0,1,0];// time counter 

// Boundary conditions

//u.t[bottom] = neumann(0.);
//u.n[bottom] = neumann(0);
//f4[right] = dirichlet(0.);

// Define global variable fields for to be added to the dump file
scalar omega[];
scalar pressure[];
scalar velocity[];


