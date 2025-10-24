// Get all the simulation parameters from this file:
#include "constants.h"
#include <stdio.h>

int main()
{
	run();
}

event init(i=0){
double tbegin = 0. [0,1,0];
double tend = 1. [0,1,0];
double t1 =0. [0,1,0];
// time iterator
double tstep = 0.001 [0,1,0];
char filename[100];
char figurename[100];


//Box for calculating gobal values for a certain field
double x_min = 0.;
double x_max = 10.;
double y_max = 10.;

for (t1 = tbegin; t1 <= tend; t1 += tstep) //t++
	{ 	
	//Data is restored from the dump files
		printf("test1");
		sprintf(filename,"../coalescence/dump-%06.3f", t1);
		printf("test2");
		restore (file= filename);
	//OpenGL based viewing function ( view.h @ Basilisk )
		 view (fov = 10, tx = 0, ty = -0.7, quat = {0, 0, -0.707, 0.707}, width=2000,height=2000);
		 
		 // Clearing canvas and displaying a certain field with a minimal and maximal value, line width, and colors based on the "jet" color scheme
		//u.x
		                clear();
		                squares (color="u.x", min = -5, max = 5);
		                draw_vof (c = "f", lw = 4);
		                //box(notics = true);
		                
		 mirror({0,1}) {
    				//squares (color = "f", min = -1, max = 1);
    				squares (color="u.x", min = -5, max = 5);
    				draw_vof (c = "f", lw = 4);
    				//box(notics = true);
 				 }
		 // Saving said image
		                sprintf(figurename,"u.x_%06.3f.png", t1); 
		                save(figurename);
		 //u.y              
		                clear();
		                squares (color="u.y", min = -5, max = 5);
		                draw_vof (c = "f", lw = 4);
		                //box(notics = true);
		 
		 mirror({0,1}) {
    				//squares (color = "f", min = -1, max = 1);
    				squares (color="u.y", min = -5, max = 5);
    				draw_vof (c = "f", lw = 4);
    				//box(notics = true);
 				 }                
		                
		                sprintf(figurename,"u.y_%06.3f.png", t1); 
		                save(figurename);
		
		//omega             
		                clear();
		                squares (color="omega", min = -10, max = 10);
		                draw_vof (c = "f", lw = 4);
		                //box(notics = true);
		 
		 mirror({0,1}) {
    				//squares (color = "f", min = -1, max = 1);
    				squares (color="omega", min = -10, max = 10);
    				draw_vof (c = "f", lw = 4);
    				//box(notics = true);
 				 }                
		                
		                sprintf(figurename,"omega_%06.3f.png", t1); 
		                save(figurename);
		                
		//p             
		                clear();
		                squares (color="pressure", min = -10, max = 10);
		                draw_vof (c = "f", lw = 4);
		                //box(notics = true);
		 
		 mirror({0,1}) {
    				//squares (color = "f", min = -1, max = 1);
    				squares (color="pressure", min = -10, max = 10);
    				draw_vof (c = "f", lw = 4);
    				//box(notics = true);
 				 }               
		                sprintf(figurename,"pressure_%06.3f.png", t1); 
		                save(figurename);
		                */
		                
		 
		// Tracking Code
		double vb = 0.; // Total  volume
		double xb = 0.; // Bubble Centroid
		double velbx =0.; // Bubble Centroid Velocity
		double hbubblemax = 0.; // Top x Coordinate of the Bubble
		double hdropmax = 0.; // Top of Droplet
		double vbtop = 0.; // Velocity at the Top of the Bubble
		double vftop = 0.; // Velocity at the Top of the Drop
		foreach() {
			// selecting for bubble
			if (x >= x_min && x <= x_max && y <= y_max) {
				double dvb = (fbubble[])*dv(); // Bubble volume selection
				vb += dvb;
				xb += x * dvb;
				velbx += u.x[]*dvb;
			}
			// top selection
			if (x >= x_min && x <= 8.3 && y <= 0.01) {
				double hbtemp = (fbubble[])*x;
				double hdtemp = (fdrop[])*x;
				if (hdtemp > hdropmax){
					hdropmax = hdtemp;
					vftop = u.x[];
					
				}
				if (hbtemp > hbubblemax){
					hbubblemax = hbtemp;
					vbtop = u.x[];
				}
			}
		}
		// log file creation
		fprintf (stderr, "t x_centroid velocity_x_centroid hbubblemax hdropmax vbtop vftop \n");
		fprintf (stderr,"%g %g %g %g %g %g %g \n", t1, xb/vb , velbx/vb,hbubblemax,hdropmax,vbtop,vftop);
			
		}
}
