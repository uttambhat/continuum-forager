#ifndef FORAGER_POPULATION_H
#define FORAGER_POPULATION_H

#include "forager.h"

class forager_population
{
	private:
	
	public:
	std::vector<random_forager> _member;
	
	forager_population();
	forager_population(random_forager Forager_Template);
	~forager_population();
	void initialize(random_forager Forager_Template);

	void move(double time);
	void consume_resource(resource_map_base &Map);
	void reproduce();
	void starve();

	void move_consume_reproduce_starve(resource_map_base &Map, double time);
};

#endif

