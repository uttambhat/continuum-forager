#include <iostream>
#include <fstream>
#include "../include/forager_population.h"
#include "../include/resource_map.h"
#include "../include/generic/output_functions.h"
#include "../include/foraging_output_functions.h"

#define VISUALIZE 0

#if VISUALIZE
#include "GL/freeglut.h"
#include "GL/gl.h"
#endif

#include <thread>
#include <chrono>

using namespace std;

void run_simulation()
{
	// Declare resource_map. Args: (number of points on the map, Brownian step-size (default set to 0.01))
	resource_map_Brownian MapA(100000,0.01L);
	
	// Set regrowth rate of map
	double regrowth_rate=1000.L;
	cout << MapA.measure_zeta(1000) << endl;
	cout << MapA.total_resources() << endl;

	// Uncomment to set value of zeta
	//MapA.set_zeta(1.L);
	
	// set mesh density (does not discretize the actual process. The mesh is only for increasing simulation speed and for visualization)
	MapA.map_to_mesh(100);

	// resolution of delta-time for simulation
	const double dtime=0.1L;
	
	double time=0.L;

	// Initialize Population vector
	int number_of_species=3;
	vector<forager_population> Forager_Population;
	Forager_Population.resize(number_of_species);
	
	// Initialize individual species populations
	for(int n=0; n<number_of_species; n++)
	{
		// set physiological parameters
		double M=pow(2.L,n);
		double metabolic_rate=pow(M,0.75), initial_state=30.L*M, reproductive_threshold=100.L*M, reproduction_aftermath_state=30.L*M, velocity=0.1L*pow(M,0.25), incorporation_radius=0.01L*pow(M,0.75);
		
		// Initialize population
		random_forager Forager_Template(metabolic_rate, initial_state, reproductive_threshold, reproduction_aftermath_state, velocity, incorporation_radius);
		Forager_Population[n].initialize(Forager_Template);
	}

	// set output file for population summary
	ofstream output_population_size; output_population_size.open("data/population_size_2");

#if VISUALIZE
	// Background color of visualization
	glClearColor(1.0, 1.0, 1.0, 1.0);
#if D==3
	glRotatef(30.,0.0,1.0,.0);
	glRotatef(30.,1.0,.0,0.0);
#endif
#endif

	// Loop around for maximum number of iterations
	for(int i=0; i<1000000; i++)
	{
		// Loop over each species
		for(int n=0; n<Forager_Population.size(); n++)
		{
			Forager_Population[n].move(dtime);
			Forager_Population[n].consume_resource(MapA);
			Forager_Population[n].reproduce();
			Forager_Population[n].starve();
		}

		// Output population summary to file
		print_to_file(time, MapA, Forager_Population, output_population_size);
		
		// Output population summary to screen
		if(i%100==0) print(time, MapA, Forager_Population);

		// If all species are extinct, exit for loop
		bool all_extinct=0;
		for(int n=0; n<Forager_Population.size(); n++) all_extinct=all_extinct&&(Forager_Population[n]._member.size()==0);
		if(all_extinct) break;

		// Regrow resource
		MapA.regrow((int)(regrowth_rate*dtime));
		
		// update time
		time += dtime;

		// Uncomment line below to slow the simulation down without changing dtime (debug tool). However, for changing simulation time resolution, use dtime
		//this_thread::sleep_for(chrono::milliseconds(0));

#if VISUALIZE
		// The next segment till the end of the run_simulation() function is only for the visualization. Ignore if visualization is switched off.
		// update visualization. Use the if condition to adjust time-resolution of visualization
		if(i%1==0)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glPointSize(10.0f);
			glBegin(GL_POINTS);
			for(int i=0; i<MapA._mesh.size(); i++)
			{
				int nD_index[D]={0}; MapA.linear_index_to_nD_index(i, nD_index);
				double position[D]={0.L};
				for(int d=0; d<D; d++) position[d] = 2.L*((double)nD_index[d]+0.5L)/(double)MapA._mesh_linear_density-1.L;
				double total_resource_in_cell=0.L;
				for(int j=0; j<MapA._mesh[i].size(); j++) total_resource_in_cell += MapA._resource_quantity[MapA._mesh[i][j]];
				double scaled_total_resource_in_cell=total_resource_in_cell/10.L;
				if(scaled_total_resource_in_cell>1.L) scaled_total_resource_in_cell=1.L;
				glColor4f(1.L-scaled_total_resource_in_cell, 1.L-0.4*scaled_total_resource_in_cell,1.L-scaled_total_resource_in_cell,0.);
#if D==2
				glVertex2f(position[0],position[1]);
#elif D==3
				glVertex3f(position[0],position[1],position[2]);
#endif
			}
			glEnd();

			for(int n=0; n<Forager_Population.size(); n++)
			{
				glPointSize((((float)n+1.))*3.);
				glBegin(GL_POINTS);
				glColor4f((0.2+0.8*(float)(number_of_species-n)/(float)number_of_species),0.,0.,0.2);
				for(int i=0; i<Forager_Population[n]._member.size(); i++)
				{
#if D==2
					glVertex2f(2.L*Forager_Population[n]._member[i]._position[0]-1.L,2.L*Forager_Population[n]._member[i]._position[1]-1.L);
#elif D==3
					glVertex3f(2.L*Forager_Population[n]._member[i]._position[0]-1.L,2.L*Forager_Population[n]._member[i]._position[1]-1.L, 2.L*Forager_Population[n]._member[i]._position[2]-1.L);
#endif
				}
				glEnd();
			}
			glFlush();
		}
#endif
	}
	output_population_size.close();


}

int main(int argc, char **argv)
{
#if VISUALIZE
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Foraging simulation");
	glutDisplayFunc(run_simulation);
	glutMainLoop();
#else
	run_simulation();
#endif
	return 0;
}

