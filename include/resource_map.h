#ifndef RESOURCE_MAP_H
#define RESOURCE_MAP_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <algorithm>
#include <vector>
#include <omp.h>
#include <limits>
#define D 2


//  Base class for resource maps. Contains all the common functions and variables.

class resource_map_base
{
	// The forager classes are given access to all the methods and variables of resource_map. Currently 'friend' declaration is not required as all the members are public.	
	friend class random_forager;
	friend class forager_population;
	protected:
	
	public:
	//---------------------------------------------------------------------
	// _size and _volume not utilized yet. All maps are created in a [0.L,1.L)^D universe
	double _size=1.L;
	double _volume;
	//---------------------------------------------------------------------

	// Important resource_map_base data. vector<vector <double> > _position (dimensions (N,D)) contains all the points on the map where resource can potentially exist.
	// vector<double> _resource_quantity (dimension (N)) stores the amount of resource at each point matching its index with _position. Currently implemented to have either 0.L or 1.L
	std::vector<std::vector <double> > _position; 
	std::vector<double> _resource_quantity;
	//---------------------------------------------------------------------

	// vector<int> _empty_sites stores indices of empty resource positions. Currently not being used
	std::vector<int> _empty_sites;
	//---------------------------------------------------------------------

	// Output functions
	void print_map();
	void print_map_to_file(char *outputfilename);
	//---------------------------------------------------------------------

	// double measure_zeta(mt19937&, int) measures the 'clustering' or 'variance scaling' parameter zeta of the resource map. Outputs a value between 1 and 2. 1 is homogeneous (essentially uniform distribution), and 2 is the most clustered. Values above 2 are possible.
	// double log_linear_fit_slope(vector<vector<double> >) returns slope of straight line fit to log-transformed data. This function is used in measure_zeta
	double measure_zeta(std::mt19937& randomness_generator, int configurations=100);
	double log_linear_fit_slope(std::vector<std::vector<double> > data);
	//---------------------------------------------------------------------

	// void move_n_random_points(mt19937&, int n) moves n random points in the resource_map. This function is used to homogenize a clustered resource_map. 
	// double set_zeta(mt19937&, double, double, int) uses move_n_random_points to reduce an initially high value of zeta to the desired value (zeta_target) within the desired tolerance (zeta_tolerance). the 'output' variable is used to cout the final measured value of zeta after the function has completed or to output error if desired zeta is higher than initial.
	void move_n_random_points(std::mt19937& randomness_generator, int n);
	double set_zeta(std::mt19937& randomness_generator, double zeta_target, double zeta_tolerance=0.1L, int output=1);
	//---------------------------------------------------------------------

	// The variables and functions in the following block is used to set up and use a mesh to speed up distance comparisons, by limiting them to local resource positions
	// vector<vector <int> > _mesh stores the indices of entries of _position that are inside a cell. Dimensions - (pow(_mesh_linear_density,D), varying: depends on number of members inside a grid-cell)
	// nD_index_to_linear_index and linear_index_to_nD_index are used to change between an D-dimensional array value for the index of a grid-cell in the mesh, and a 1-dimensional value
	// void map_to_mesh(int) function creates a mesh with user-defined mesh_linear_density. This must be called in main.C before forager::consume() function can be called.
	int _mesh_linear_density;
	std::vector <std::vector <int> > _mesh;
	int nD_index_to_linear_index(int nD_index[]);
	void linear_index_to_nD_index(int linear_index, int nD_index[]);
	void map_to_mesh(int mesh_linear_density);
	void print_mesh_to_file(char *outputfilename);
	//---------------------------------------------------------------------

	// double total_resources() totals the number of resources and assigns to _resource_count. Needs to be called in main.C before foraging process starts. The dynamics will be the same even if the function isn't called, but the resource total will not be correct.
	// void fill_resources(quantity) adds quantity amount of resources to all the resource positions.
	// int regrow(mt19937&, int) chooses 'number_of_sites_to_regrow' random sites among all the positions and changes the value to 1.L (independent of its initial value)
	double total_resources();
	double _resource_count=0.L;
	void fill_resources(double quantity=1.L);	
	int regrow(std::mt19937& randomness_generator, int number_of_sites_to_regrow);
	//---------------------------------------------------------------------

};

// Derived class. This class is the base class with added constructor and destructors.  There are three ways to initiate members,
// 1) With just the random number generator. This will create a map with 100 resource points
// 2) With number of patches, size (not currently implemented), and the distribution desired (currently just implemented for a uniform distribution
// 3) By passing the vector of positions and resource quantities, i.e., fully specified by external code.
class resource_map : public resource_map_base
{
	protected:
	
	public:
	int _num_patches;
	char _distribution='u';
	
	resource_map(std::mt19937& randomness_generator);
	resource_map(std::mt19937& randomness_generator, int num_patches, double size=1.L, char distribution='u');
	resource_map(int num_patches, double size, std::vector<std::vector <double> > &position, std::vector<double> &resource_quantity);

	~resource_map();
	
};

// Derived class. This class has positions generated by the set of visited sites by a Brownian particle. A member is constructed by specifying the total number of 
// patches desired and the step-size of the Brownian particle
class resource_map_Brownian : public resource_map_base
{
	protected:
	
	public:
	double _step_size_resource=0.01L;
	int _num_patches;
	resource_map_Brownian(std::mt19937& randomness_generator, int num_patches=10000, double step_size_resource=0.01L);
	~resource_map_Brownian();
	
};

// Derived class. This class generates a box-fractal resource-map. A member is constructed by specifying the number of fractal-levels and fill-fraction (i.e., the pair - fill number and subdivision number 
class resource_map_fractal : public resource_map_base
{
	protected:
	
	public:
	int _levels, _fill, _subdivide;
	void fill_fractal(std::mt19937& randomness_generator, int levels, int fill, int subdivide, std::vector <std::vector <double> > &position, double x_start[], double x_end[], int seed);
	resource_map_fractal(std::mt19937& randomness_generator, int levels, int fill, int subdivide, double size=1.L);
	~resource_map_fractal();
};


#endif

