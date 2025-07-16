/**************************************************************
***************************************************************
*** 
***   data.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains functions for manipulating data.
***
***************************************************************
**************************************************************/

#include "includes.h"

void data_delete(void (*data[])(), void (*data_parameters[])(), short int data_index, short int data_size)
{
	if (data[data_index]!=0 && data_index<data_size)
	{
		free(data[data_index]); data[data_index]=0;
		free(data_parameters[data_index]); data_parameters[data_index]=0;
	}

	return;
}

short int data_input(void (*data[])(), void (*data_parameters[])(), void **data_input, void **data_parameters_input, short int data_index, short int data_size)
{
	if (data_index>=data_size)
	{
		printf("Data index must be between 0 and %hd.\n", data_size-1);
		return 1;
	}
	if (data[data_index]==0)
	{
		printf("Data index %hd is empty.\n", data_index);
		return 1;
	}

	*data_input=data[data_index];
	*data_parameters_input=data_parameters[data_index];
	
	return 0;
}

short int data_output(void (*data[])(), void (*data_parameters[])(), void **data_output, void **data_parameters_output, int output_size, int output_parameters_size, short int data_index, short int data_size)
{
	if (data_index>=data_size)
	{
		printf("Data index must be between 0 and %hd.\n", data_size-1);
		return 1;
	}
	if (data[data_index]!=0)
	{
		printf("Data index %hd is already occupied.\n", data_index);
		return 1;
	}

	*data_output=malloc(output_size);
	if (*data_output==0)
	{
		printf("Insufficient memory.\n");
		return 1;
	}
	
	*data_parameters_output=malloc(output_parameters_size);
	if (*data_parameters_output==0)
	{
		printf("Insufficient memory.\n");
		return 1;
	}
	
	data[data_index]=*data_output;
	data_parameters[data_index]=*data_parameters_output;
	
	return 0;
}

struct parallel_index_data parallel_index(short int command, short int data_index, int data_size, short int data_num)
{
	static struct parallel_index_data a;
	
	static int i;

	switch (command)
	{
		case 0:			// '0' means to delete all entries from the tag list
		{
			for (i=0; i<data_num; i++)
			{
				a.data_index[i]=0;
				a.data_size[i]=0;
			}
			i=0;
			
			a.num=i;
			
			return a;
		}
		case 1:			// '1' means to add an entry to the tag list
		{
			if (i==data_num)
			{
				printf("The parallel index array is already full.\n");
				return a;
			}
			
			a.data_index[i]=data_index;
			a.data_size[i]=data_size;
			i++;

			a.num=i;
			
			return a;
		}
		default: return a;	// Any other value just returns the tag list
	}
}

void state(short int *spins, int *flips, int steps)
{
	int i;
	
	for (i=0; i<steps; i++)
	{
		*(spins+*(flips+2*i))=-*(spins+*(flips+2*i));
	}
	
	return;
}

struct tag_data tag(short int command, short int data_input, short int data_output, short int data_num)
{
	static struct tag_data a;
	
	static int i;
	
	switch (command)
	{
		case 0:			// '0' means to delete all entries from the tag list
		{
			for (i=0; i<data_num/2; i++)
			{
				a.data_input[i]=0;
				a.data_output[i]=0;
			}
			i=0;
			
			return a;
		}
		case 1:			// '1' means to add an entry to the tag list
		{
			if (i==data_num/2)
			{
				printf("The tag array is already full.\n");
				return a;
			}
			
			if (data_input==data_output)
			{
				printf("The input and output indices must be different.\n");
				return a;
			}
			
			a.data_input[i]=data_input;
			a.data_output[i]=data_output;
			i++;

			return a;
		}
		default: return a;	// Any other value just returns the tag list
	}
}

