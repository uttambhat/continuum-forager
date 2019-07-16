#ifndef FORAGER_H
#define FORAGER_H

#include "resource_map.h"
/*
Lattice:
Functionality:
-> size
-> boundary condition
-> contents
-> 

*/

class random_forager
{
	private:
	
	public:
	double _step_size, _sensing_radius, _metabolic_rate, _state, _initial_state, _reproductive_threshold, _reproductive_aftermath_state, _velocity, _lifetime, _handling_time;
	
	std::vector<double> _position;
	
	random_forager(double metabolic_rate, double initial_state, double reproductive_threshold, double reproductive_aftermath_state, double velocity, double sensing_radius, double handling_time=0.L);
	~random_forager();
	double distance(std::vector<double> &x, std::vector<double> &y);
	void move_forager(double time=0.1L);
	void consume_resource(resource_map_base &Map);
	void custom_linear_index_to_nD_index(int linear_index, int *nD_index, int nD_index_max);
};

#endif

