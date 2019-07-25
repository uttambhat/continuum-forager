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
#define D 2


/*
	TO BE ADDED!
	Change resource_map to resource_map_base. Add common functions and quantities here
	Create friend classes resource_map_uniform, resource_map_Brownian and resource_map_fractal
	Make the code compatible with 3D
	
*/



class resource_map_base
{
	friend class random_forager;
	friend class forager_population;
	protected:
	
	public:
	int seed=1;
	double _size=1.L;
	double _volume;
	std::vector<std::vector <double> > _position; 
	std::vector<double> _resource_quantity;
	std::vector<int> _empty_sites;
	void print_map();
	void print_map_to_file(char *outputfilename);
	double measure_zeta(std::mt19937& randomness_generator, int configurations=100);
	double log_linear_fit_slope(std::vector<std::vector<double> > data);

	void move_n_random_points(std::mt19937& randomness_generator, int n);
	double set_zeta(std::mt19937& randomness_generator, double zeta_target, double zeta_tolerance=0.1L, int output=1);

	int _mesh_linear_density;
	std::vector <std::vector <int> > _mesh;
	int nD_index_to_linear_index(int nD_index[]);
	void linear_index_to_nD_index(int linear_index, int nD_index[]);
	void map_to_mesh(int mesh_linear_density);
	void print_mesh_to_file(char *outputfilename);

	double total_resources();
	double _resource_count=0.L;
	void fill_resources(double quantity=1.L);	
	int regrow(std::mt19937& randomness_generator, int number_of_sites_to_regrow);

};

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

class resource_map_Brownian : public resource_map_base
{
	protected:
	
	public:
	double _step_size_resource=0.01L;
	int _num_patches;
	resource_map_Brownian(std::mt19937& randomness_generator, int num_patches=10000, double step_size_resource=0.01L);
	~resource_map_Brownian();
	
};

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

