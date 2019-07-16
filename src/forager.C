#include "../include/forager.h"

std::mt19937 gen_forager(2);

random_forager::random_forager(double metabolic_rate, double initial_state, double reproductive_threshold, double reproductive_aftermath_state, double velocity, double sensing_radius, double handling_time)
{
	_sensing_radius = sensing_radius;
	_metabolic_rate = metabolic_rate;
	_initial_state = initial_state;
	_reproductive_threshold = reproductive_threshold;
	_reproductive_aftermath_state = reproductive_aftermath_state;
	_state = _initial_state;
	_velocity = velocity;
	_handling_time = handling_time;
	_lifetime = 0.L;
	std::uniform_real_distribution<double> uniform(0.L, 1.L);
	for(int d=0; d<D; d++) _position.push_back(uniform(gen_forager));
}

random_forager::~random_forager()
{
}

double random_forager::distance(std::vector<double> &x, std::vector<double> &y)
{
	double result=0.L;
	for(int d=0; d<D; d++) result += pow(x[d]-y[d],2);
	return sqrt(result);
}


void random_forager::move_forager(double time)
{
	_step_size=_velocity*time;
	std::normal_distribution<double> single_step(0.L,_step_size);
	for(int d=0; d<D; d++)
	{
		_position[d] += single_step(gen_forager);
		
		if(_position[d]<0.L) _position[d]=0.L; // TO BE CHANGED TO ACCEPT DIFFERENT MAP SIZES
		if(_position[d]>1.L) _position[d]=1.L-0.000000001L;
	}
	_state -= _metabolic_rate*time;
	_lifetime += time;
}

void random_forager::consume_resource(resource_map_base &Map)
{
	int sensing_radius_in_mesh_pixels=(int)(ceil(_sensing_radius/(Map._size/(double)Map._mesh_linear_density)))+1;
	int mesh_position[D]={0};
	for(int d=0; d<D; d++) mesh_position[d]=(int)(_position[d]/(Map._size/(double)Map._mesh_linear_density));

	
	int mesh_search_volume=pow(2*sensing_radius_in_mesh_pixels+1,D);
	
	for(int i=0; i<mesh_search_volume; i++)
	{
		int nD_index[D]; custom_linear_index_to_nD_index(i, nD_index, 2*sensing_radius_in_mesh_pixels+1);
		for(int d=0; d<D; d++) nD_index[d] = mesh_position[d]+(nD_index[d]-sensing_radius_in_mesh_pixels);
		bool is_inside_map=1;
		for(int d=0; d<D; d++) is_inside_map = is_inside_map&&(nD_index[d]>-1)&&(nD_index[d]<Map._mesh_linear_density);
		if(is_inside_map)
		{
			int linear_index=Map.nD_index_to_linear_index(nD_index);
			for(int k=0; k<Map._mesh[linear_index].size(); k++)
			{
				if(distance(Map._position[Map._mesh[linear_index][k]],_position)<_sensing_radius)
				{
					if(Map._resource_quantity[Map._mesh[linear_index][k]]>0.L)
					{
						//std::cout << Map._resource_quantity[Map._mesh[linear_index][k]] << '\t';
						double resource_reduction = Map._resource_quantity[Map._mesh[linear_index][k]];
						_state += Map._resource_quantity[Map._mesh[linear_index][k]];
						Map._resource_quantity[Map._mesh[linear_index][k]]=0.L;
						_lifetime += _handling_time;
						_state -= _metabolic_rate*_handling_time;
						Map._empty_sites.push_back(Map._mesh[linear_index][k]);
						Map._resource_count -= resource_reduction;
						//std::cout << Map._resource_quantity[Map._mesh[linear_index][k]] << std::endl;
					}
				}
			}
		
		}
		
	}
	
}

void random_forager::custom_linear_index_to_nD_index(int linear_index, int nD_index[], int nD_index_max)
{
	for(int d=0; d<D; d++)
	{
		nD_index[d]=linear_index%nD_index_max;
		linear_index=linear_index/nD_index_max;
	}

}
