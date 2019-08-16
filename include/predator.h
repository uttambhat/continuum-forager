#ifndef PREDATOR_H
#define PREDATOR_H

#include "forager.h"
/*
Lattice:
Functionality:
-> size
-> boundary condition
-> contents
-> 

*/

class predator
{
	private:
	
	public:
	double _step_size, _sensing_radius, _metabolic_rate, _state, _initial_state, _reproductive_threshold, _reproductive_aftermath_state, _velocity, _lifetime;
	
	std::vector<double> _position;
	
	predator(std::mt19937& randomness_generator, double metabolic_rate, double initial_state, double reproductive_threshold, double reproductive_aftermath_state, double velocity, double sensing_radius);
	~predator();
	double distance(std::vector<double> &x, std::vector<double> &y);
	void move_predator(std::mt19937& randomness_generator, double time=0.1L);
	void consume_prey(resource_map_base &Map);
	void custom_linear_index_to_nD_index(int linear_index, int *nD_index, int nD_index_max);
};

#endif

