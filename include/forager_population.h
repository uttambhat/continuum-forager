#ifndef FORAGER_POPULATION_H
#define FORAGER_POPULATION_H

#include "forager.h"

// This class uses the random_forager class to create a population of foragers. In addition to move and consume_resource functions (extended from a single forager), this class also has functions to reproduce and kill its members. Reproduction occurs when random_forager._state exceeds random_forager._reproductive_threshold. A member is killed when random_forager._state goes below zero (starvation). A member is also killed at a small rate specified by the random_forager._lifespan
class forager_population
{
	private:
	
	public:
	std::vector<random_forager> _member;
	
	forager_population();
	forager_population(random_forager Forager_Template);
	~forager_population();
	void initialize(random_forager Forager_Template);

	void move(std::mt19937& randomness_generator, double time);
	void consume_resource(resource_map_base &Map);
	void reproduce();
	void kill(std::mt19937& randomness_generator, double time);

	void move_consume_reproduce_kill(std::mt19937& randomness_generator, resource_map_base &Map, double time);
};

#endif

