#include "../include/resource_map.h"

std::mt19937 gen_resource_map(1);

void resource_map_base::print_map()
{
	for(int i=0; i<_position.size(); i++)
	{
		for(int d=0; d<D; d++) std::cout << _position[i][d] << '\t';
		std::cout << _resource_quantity[i] << std::endl;
	}
}

void resource_map_base::print_map_to_file(char *outputfilename)
{
	std::ofstream output;
	output.open(outputfilename);
	for(int i=0; i<_position.size(); i++)
	{
		for(int d=0; d<D; d++) output << _position[i][d] << '\t';
		output << _resource_quantity[i] << std::endl;
	}
	output.close();
}

double resource_map_base::measure_zeta(int configurations)
{
	std::vector<std::vector<double> > data;
	for(double size=0.01; size<0.1; size+=0.01)
	{
		std::uniform_real_distribution<double> random_location(0.L,1.L-size);
		double sumx=0.L, sumx2=0.L;
		for(int config=0; config<configurations; config++)
		{
			double total=0.L;
			double box[D]={0.L};
			for(int d=0; d<D; d++) box[d]=random_location(gen_resource_map);
			for(int i=0; i<_position.size(); i++)
			{
				bool is_inside_box=1;
				for(int d=0; d<D; d++)
				{
					is_inside_box = (is_inside_box&&(_position[i][d]>box[d])&&(_position[i][d]<box[d]+size));
				}
				if(is_inside_box) total+=_resource_quantity[i];
			}
			sumx += total;
			sumx2 += pow(total,2);
		}
		std::vector<double> current_data;
		current_data.push_back(pow(size,D));
		current_data.push_back(sumx/(double)configurations);
		current_data.push_back(sumx2/(double)configurations-pow(sumx/(double)configurations,2));
		data.push_back(current_data);
	}
	return log_linear_fit_slope(data);
}

double resource_map_base::log_linear_fit_slope(std::vector<std::vector<double> > data)
{
	double sumx=0.L, sumxx=0.L, sumy=0.L, sumxy=0.L;
	for(int i=0; i<data.size(); i++)
	{
		sumx += log(data[i][0]);
		sumxx += pow(log(data[i][0]),2);
		sumy += log(data[i][2]);
		sumxy += (log(data[i][0])*log(data[i][2]));
	}
	return ((double)data.size()*sumxy - sumx*sumy)/((double)data.size()*sumxx - sumx*sumx);
}

int resource_map_base::nD_index_to_linear_index(int nD_index[])
{
	int linear_index=0;
	for(int d=0; d<D; d++) linear_index += nD_index[d]*pow(_mesh_linear_density,d);
	return linear_index;
}

void resource_map_base::linear_index_to_nD_index(int linear_index, int nD_index[])
{
	for(int d=0; d<D; d++)
	{
		nD_index[d]=linear_index%_mesh_linear_density;
		linear_index=linear_index/_mesh_linear_density;
	}
}

void resource_map_base::map_to_mesh(int mesh_linear_density)
{
	_mesh_linear_density = mesh_linear_density;
	_mesh.resize(pow(_mesh_linear_density,D));
	for(int i=0; i<_position.size(); i++)
	{
		int nD_index[D];
		for(int d=0; d<D; d++) nD_index[d] = (int)((double)_mesh_linear_density*_position[i][d]/_size);
		_mesh[nD_index_to_linear_index(nD_index)].push_back(i);
	}
}

void resource_map_base::print_mesh_to_file(char *outputfilename)
{
	std::ofstream output;
	output.open(outputfilename);
	for(int i=0; i<_mesh.size(); i++)
	{
		int nD_index[D]={0}; linear_index_to_nD_index(i, nD_index);
		for(int d=0; d<D; d++) output << ((double)nD_index[d]+0.5L)/(double)_mesh_linear_density << '\t';
		double total_resource_in_cell=0.L;
		for(int j=0; j<_mesh[i].size(); j++) total_resource_in_cell += _resource_quantity[_mesh[i][j]];
		output << total_resource_in_cell << std::endl;
	}
	output.close();
}

double resource_map_base::total_resources()
{
	double result=0.L;
	for(int i=0; i<_resource_quantity.size(); i++) result += _resource_quantity[i];
	_resource_count=result;
	return result;
}

void resource_map_base::fill_resources(double quantity)
{
	for(int i=0; i<_position.size(); i++) _resource_quantity.push_back(quantity);
}

void resource_map_base::move_n_random_points(int n)
{
	std::uniform_int_distribution<int> pick_random_point(0,_position.size()-1);
	std::uniform_real_distribution<double> uniform(0.L,_size);
	for(int i=0; i<n; i++)
	{
		int candidate=pick_random_point(gen_resource_map);
		for(int d=0; d<D; d++) _position[candidate][d]=uniform(gen_resource_map);
	}
}

double resource_map_base::set_zeta(double zeta_target, double zeta_tolerance, int output)
{
	int measurement_sample_size=10, measurement_configuration_size=8*(1+(D-2)*8);
	double sumzeta=0.L, sumzeta2=0.L, average_zeta=0.L, average_zeta2=0.L, stdev_zeta=0.L;
	do
	{
		sumzeta=0.L; sumzeta2=0.L; average_zeta=0.L; average_zeta2=0.L;
		measurement_configuration_size *= 2;	
		for(int sample=0; sample<measurement_sample_size; sample++)
		{
			double measured_zeta=measure_zeta(measurement_configuration_size);
			sumzeta += measured_zeta;
			sumzeta2 += pow(measured_zeta,2);
		}	
		average_zeta=sumzeta/(double)measurement_sample_size;
		average_zeta2=sumzeta2/(double)measurement_sample_size;
		stdev_zeta=sqrt(average_zeta2-pow(average_zeta,2));
		if(output==1) std::cout << average_zeta << '\t' << stdev_zeta << '\t' << measurement_configuration_size << std::endl;
	}
	while(stdev_zeta>zeta_tolerance);
	if(average_zeta+stdev_zeta<zeta_target)
	{ 
		if(output==1) std::cout << "Input zeta is too high. Try a lower value" << std::endl; 
		return average_zeta; 
	}
	
	else
	{
		int num_random_points_to_move=8;
		double previous_zeta=average_zeta, current_zeta=0.L;
		do
		{
			move_n_random_points(num_random_points_to_move);
			current_zeta=measure_zeta(measurement_configuration_size*4);
			if(std::abs(current_zeta-previous_zeta)<stdev_zeta/2.L) num_random_points_to_move *=2;
			if(output==1) std::cout << current_zeta << '\t' << num_random_points_to_move << std::endl;
			previous_zeta=current_zeta;
			
		}
		while(current_zeta-stdev_zeta>zeta_target);
		return current_zeta;
	}
}

int resource_map_base::regrow(int number_of_sites_to_regrow)
{
	int number_of_valid_regrowths=0;
	while(_empty_sites.size()>0)
	{
		std::uniform_int_distribution<int> random_empty_site(0,_empty_sites.size()-1);
		int site_to_regrow = random_empty_site(gen_resource_map);
		_resource_quantity[_empty_sites[site_to_regrow]] = 1.0L;
		_resource_count += _resource_quantity[_empty_sites[site_to_regrow]];
		_empty_sites.erase(_empty_sites.begin()+site_to_regrow);		
		number_of_valid_regrowths++;
		if(number_of_valid_regrowths==number_of_sites_to_regrow) break;
	}
	return number_of_valid_regrowths;
}

resource_map::resource_map()
{
	_size=1.L;
	_distribution = 'u';

	std::uniform_real_distribution<double> uniform(0.L,_size);

	_volume=pow(_size,D);
	_num_patches=100;
	_position.resize(_num_patches);
	_resource_quantity.resize(_num_patches);
	for(int i=0; i<_num_patches; i++)
	{
		_position[i].resize(D);
		for(int d=0; d<D; d++) _position[i][d]=uniform(gen_resource_map);
		_resource_quantity[i] = uniform(gen_resource_map);
	}

}

resource_map::resource_map(int num_patches, double size, char distribution)
{
	_size = size;
	_distribution = distribution;

	std::uniform_real_distribution<double> uniform(0.L,_size);
	std::uniform_real_distribution<double> dist(0.L,_size);

	_num_patches = num_patches;
	_volume = pow(_size,D);
	_position.resize(_num_patches);
	_resource_quantity.resize(_num_patches);
	for(int i=0; i<_num_patches; i++) _position[i].resize(D);
	
	if(_distribution='u')
	{
		for(int i=0; i<_num_patches; i++)
		{
			for(int d=0; d<D; d++) _position[i][d]=uniform(gen_resource_map);
			_resource_quantity[i] = dist(gen_resource_map);
		}
	}
	else if(_distribution='f')
	{

	}
}

resource_map::resource_map(int num_patches, double size, std::vector<std::vector <double> > &position, std::vector<double> &resource_quantity)
{
	_size = size;

	_num_patches = num_patches;
	_volume = pow(_size,D);
	_position.resize(_num_patches);
	_resource_quantity.resize(_num_patches);
	for(int i=0; i<_num_patches; i++)
	{
		_position[i].resize(D);
		for(int d=0; d<D; d++) _position[i][d]=position[i][d];
		_resource_quantity[i] = resource_quantity[i];
	}


}

resource_map::~resource_map()
{
}

void resource_map_fractal::fill_fractal(int levels, int fill, int subdivide, std::vector <std::vector <double> > &position, double x_start[], double x_end[], int seed)
{
	std::vector<int> boxes; 
	for(int i=0; i<pow(subdivide,D); i++) boxes.push_back(i);
	std::shuffle(boxes.begin(), boxes.end(), gen_resource_map);
	if(levels>1)
	{
		//------------------------------
		// DOES NOT WORK FOR D!=2 !!!!!!
		//-------------------------------
	
		for(int i=0; i<fill; i++)
		{
			double new_x_start[D] = {x_start[0] + (x_end[0]-x_start[0])*((double)(boxes[i]%subdivide))/(double)subdivide, x_start[1] + (x_end[1]-x_start[1])*((double)(boxes[i]/subdivide))/(double)subdivide};
			double new_x_end[D] = {x_start[0] + (x_end[0]-x_start[0])*((double)(boxes[i]%subdivide+1))/(double)subdivide, x_start[1] + (x_end[1]-x_start[1])*((double)(boxes[i]/subdivide+1))/(double)subdivide};
			fill_fractal(levels-1, fill, subdivide, position, new_x_start, new_x_end, gen_resource_map());
		}			
	}
	
	else
	{
		std::vector <double> new_position;
		new_position.resize(D);
		std::uniform_real_distribution<double> uniform(0.L,1.L);

		for(int i=0; i<fill; i++)
		{
			new_position[0] = x_start[0] + (x_end[0]-x_start[0])*((double)(boxes[i]%subdivide)+uniform(gen_resource_map))/(double)subdivide;
			new_position[1] = x_start[1] + (x_end[1]-x_start[1])*((double)(boxes[i]/subdivide)+uniform(gen_resource_map))/(double)subdivide;
			position.push_back(new_position);
		}
	}

}
	


resource_map_fractal::resource_map_fractal(int levels, int fill, int subdivide, double size)
{
	_levels = levels;
	_fill = fill;
	_subdivide = subdivide;
	_size = size;
	//------------------------------
	// DOES NOT WORK FOR D!=2 !!!!!!
	//-------------------------------
	
	double x_start[]={0.L,0.L}, x_end[]={size,size};
	fill_fractal(_levels, _fill, _subdivide, _position, x_start, x_end, 1);
	fill_resources(1.L);	
}

resource_map_fractal::~resource_map_fractal()
{

}




resource_map_Brownian::resource_map_Brownian(int num_patches, double step_size_resource)
{
	_num_patches=num_patches;
	_step_size_resource = step_size_resource;
	std::vector<double> position;
	for(int d=0; d<D; d++) position.push_back(0.L);
	double step_size=0.01L;
	std::normal_distribution<double> single_step(0.L,step_size_resource);
	for(int i=0; i<_num_patches; i++)
	{

		for(int d=0; d<D; d++)
		{
			position[d] += single_step(gen_resource_map);
			if(position[d]<0.L) position[d]=0.L;
			if(position[d]>=1.L) position[d]=1.L-0.00000000001L;
		}
		_position.push_back(position);
	}

	fill_resources(1.L);	
}

resource_map_Brownian::~resource_map_Brownian()
{
}


