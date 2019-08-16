#ifndef FORAGER_H
#define FORAGER_H

#include "resource_map.h"

// Basic forager class. Specified by physiological variables and contains function to move the forager, consumer resource from a resource_map and auxiliary functions to compute distance between two points, and change between D-dimensional index and one-dimensional index
class random_forager
{
	private:
	
	public:
	// Physiological parameters off the forager
	double _step_size, _sensing_radius, _metabolic_rate, _state, _initial_state, _reproductive_threshold, _reproductive_aftermath_state, _velocity, _lifetime, _lifespan;
	
	std::vector<double> _position;
	
	random_forager(std::mt19937& randomness_generator, double metabolic_rate, double initial_state, double reproductive_threshold, double reproductive_aftermath_state, double velocity, double sensing_radius, double lifespan=std::numeric_limits<double>::infinity());
	~random_forager();
	double distance(std::vector<double> &x, std::vector<double> &y);
	void move_forager(std::mt19937& randomness_generator, double time=0.1L);
	void consume_resource(resource_map_base &Map);
	void custom_linear_index_to_nD_index(int linear_index, int *nD_index, int nD_index_max);
};

#endif

