#include "../include/foraging_output_functions.h"

void print(double time, resource_map_base Map, std::vector<forager_population> Forager_Population)
{
	std::cout << time << '\t';
	for(int n=0; n<Forager_Population.size(); n++)	std::cout << Forager_Population[n]._member.size() << '\t';
	std::cout << Map._resource_count << std::endl;

}

void print_to_file(double time, resource_map_base Map, std::vector<forager_population> Forager_Population, std::ofstream &output)
{
	output << time << '\t';
	for(int n=0; n<Forager_Population.size(); n++) output << Forager_Population[n]._member.size() << '\t';
	output << Map._resource_count << std::endl;

}


void print_forager_populations_to_file(std::vector<forager_population> &Forager_Population, char *output_filename)
{
	std::ofstream output;
	output.open(output_filename);

	int max_size=0;
	for(int n=0; n<Forager_Population.size(); n++) if(max_size<Forager_Population[n]._member.size()) max_size=Forager_Population[n]._member.size();

	for(int n=0; n<Forager_Population.size(); n++) for(int d=0; d<D; d++) output << 0.L << '\t'; output << std::endl; // Temporary fix to make sure all columns have correct ids. Not sure if it works though. To be tested.
	for(int i=0; i<max_size; i++)
	{
		for(int n=0; n<Forager_Population.size(); n++)
		{
			if(i<Forager_Population[n]._member.size())
			{
				for(int d=0; d<D; d++)
				{
					output << Forager_Population[n]._member[i]._position[d] << '\t';
				}
			}
			else
			{
				for(int d=0; d<D; d++)
				{
					output << '\t';
				}
			}
		}
		output << std::endl;
	}
	output.close();

}

