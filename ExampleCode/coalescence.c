// Get all the simulation parameters from this file:
#include "constants.h"

int main() {
  // Parameters
  rho1 = vrho_w ;
  rho2 = vrho_g ;
  
  mu1 = vmu_w ; 
  mu2 = vmu_g ;
  
  //surface tension
  f.sigma = SIGMA ;
  
  //reduced gravity
  G.x = -Bo;
  
  size (length );
  TOLERANCE = 1e-4 [*]; //default is 1e-3

  origin(0.,0.); 
  init_grid (1 << minlevel); //Grid Initialisation
  
  run();     
}

// At start time
event init (t = tc) {
  if (!restore (file = "dump-restart")) {// Relaunching the simulation from a later time if we already have a dump file from a previous stage of the simulation. It needs to be renamed to the string typed above in order to continue with our simulation
  printf("Init");
  
  //Refine the grids of drop boundary
  refine( sq(x-hdrop_num)+sq(y)<sq(diameter/2+0.2) && sq(x-hdrop_num)+sq(y) > sq(diameter/2-0.2) && level < maxlevel);
  //Refine the grids of pool boundary
  refine(x<wheight_num+0.2 && x>wheight_num-0.2 && level < maxlevel);
  //Refine the grids of bubble boundary
  refine(bubble(x,y) > -0.1 && level < maxlevel);
  
  double kpool = 1.; // Trick to get the same units in the max

  fraction(f, max(kpool*pool(x,y), droplet(x, y))); //Defining our fluid fractions
  fraction(fdrop, droplet(x,y));
  fraction(fbubble, bubble(x,y));
  foreach() { 
    u.x[] = -0.;
   // The drop is deposed with zero initial velocity ( the vorticity field is already defined)
  }
}else{
 printf("Restoring dumpfile");
 }
}

event adapt (i++) { //Event adaptation of the data
  double uem = 1.e-3 ; // Velocity tolerance
  double fem = 1.e-10 ; // Fluid attribution tolerance
  
 
  adapt_wavelet({u, f, fdrop, fbubble}, (double[]){uem, uem, fem, fem, fem}, maxlevel, minlevel);
  
}

//Saving an image of the simulation at each time step
//Selecting the OpenGl based perspective
event viewing (t = tc; t += dtuser; t <= tmax) {
  view (fov = 10, tx = 0, ty = -0.7, quat = {0, 0, -0.707, 0.707}, width=2000,height=2000);


// Clearing canvas & defining the colors for the time snapshots	
  clear();
  squares (color = "f*(2*fdrop-1)*(1-(1/2)*fbubble)", min = -1, max = 1);
  draw_vof("f", lc = {0, 0, 0}, lw = 2);
  //box(notics=true);
  
  mirror({0,1}) {
  squares (color = "f*(2*fdrop-1)*(1-(1/2)*fbubble)", min = -1, max = 1);
  draw_vof("f", lc = {0, 0, 0}, lw = 2);
  //box(notics=true);
  }

  char png_filename[100];
  sprintf(png_filename, "time-%06.3f.png", t);

  save(png_filename);//Saving the image

 
  frame++;
}


// Saving the interface data 
event out_facets (t = tc; t += dtuser; t <= tmax) {
  char name[100];
  sprintf(name, "drop_t_%06.3f.dat", t);
  FILE * fp1 = fopen (name, "w");
  output_facets (f, fp1);
  
  fclose(fp1);
}

//Saving field data
event snapshots (t =tc; t +=dtuser; t<= tmax) {

  // SEPS - constant to avoid divisions by 0
  char name[100];
  foreach(){
  pressure[] = p[]-(vrho_w-vrho_g)*Bo*f[]*x; // Correction to the reduced pressure in order for our data to represent the "real" pressure in the system
  velocity[] = sqrt(sq(u.x[])+sq(u.y[]));
  // there are two ways of fixing the problem with the implementation of vorticity in Basilisk - we either fix utils.h or we redefine it in situ as we do here 
  omega[] = (u.y[1,0]-u.y[-1,0]+u.x[0,-1]-u.x[0,1]) / (2.*Delta + SEPS);
  }  
  sprintf (name, "dump-%06.3f", t);
  dump (name);
}




