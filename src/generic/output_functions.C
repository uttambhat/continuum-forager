#include "../../include/generic/output_functions.h"

int update_filename_index(char *filename, int index, int max_digits)
{
	int end_of_char_array=0;
	while(filename[end_of_char_array]!='\0') end_of_char_array++;
	for(int i=0; i<max_digits; i++)
	{
		filename[end_of_char_array-i-1]=(char)((int)('0')+(index%10));
		index = index/10;
	}
	return 1;
}


