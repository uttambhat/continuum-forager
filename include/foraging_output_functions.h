#ifndef FORAGING_OUTPUT_FUNCTIONS_H
#define FORAGING_OUTPUT_FUNCTIONS_H

#include "forager_population.h"

void print(double time, resource_map_base Map, std::vector<forager_population> Forager_Population);
void print_to_file(double time, resource_map_base Map, std::vector<forager_population> Forager_Population, std::ofstream &output);
void print_forager_populations_to_file(std::vector<forager_population> &Forager_Population, char *output_filename);


#endif

