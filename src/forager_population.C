#include "../include/forager_population.h"

forager_population::forager_population()
{
}

forager_population::forager_population(random_forager Forager_Template)
{
	_member.push_back(Forager_Template);
	
}


forager_population::~forager_population()
{
}

void forager_population::initialize(random_forager Forager_Template)
{
	_member.push_back(Forager_Template);
}

void forager_population::move(std::mt19937& randomness_generator, double time)
{
	for(int i=0; i<_member.size(); i++)
	{
		_member[i].move_forager(randomness_generator, time);
	}
}

void forager_population::consume_resource(resource_map_base &Map)
{
	for(int i=0; i<_member.size(); i++)
	{
		_member[i].consume_resource(Map);
	}

}
void forager_population::reproduce()
{
	for(int i=0; i<_member.size(); i++)
	{
		if(_member[i]._state>_member[i]._reproductive_threshold)
		{
			_member[i]._state=_member[i]._reproductive_aftermath_state;
			_member.push_back(_member[i]);
			_member.back()._lifetime=0.L;
		}
	}
}

void forager_population::kill(std::mt19937& randomness_generator, double time)
{
	for(int i=0; i<_member.size(); i++)
	{
		std::bernoulli_distribution bernoulli(time/_member[i]._lifespan);
		if(_member[i]._state<0.L||bernoulli(randomness_generator)==1)
		{
			_member.erase(_member.begin()+i);	
		}
	}
}

void forager_population::move_consume_reproduce_kill(std::mt19937& randomness_generator, resource_map_base &Map, double time)
{
	for(int i=0; i<_member.size(); i++)
	{
		_member[i].move_forager(randomness_generator, time);
		_member[i].consume_resource(Map);
		if(_member[i]._state>_member[i]._reproductive_threshold)
		{
			_member[i]._state=_member[i]._reproductive_aftermath_state;
			_member.push_back(_member[i]);
		}
		if(_member[i]._state<0.L||_member[i]._lifetime>_member[i]._lifespan)
		{
			_member.erase(_member.begin()+i);	
		}
	}
}


