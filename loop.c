/**************************************************************
***************************************************************
*** 
***   loop.c
***
***   Jason Olejarz
***   2011-01-25
***
***   This file contains the routines used by 'calc' to set up the lattice before each run of 'evolve'.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int loop_iterate(short int command, struct loop_iterate_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static int i, count, progress;
	static int last_progress, new_progress;
	
	switch (command)
	{
		case command_begin:
		{
			if (progress==1)
			{
				printf("Processing...      ");
				fflush(stdout);

				last_progress=-1;
			}

			i=0;

			return result_continue;
		}
		case command_end:
		{
			if (progress==1)
			{
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bDone                    \n");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_insert: {count=command_data->count; progress=command_data->progress; return result_continue;}
		case command_run:
		{
			if (progress==1)
			{
				new_progress=(int)(1000*i/(double)count);

				if (new_progress>last_progress)
				{
					printf("\b\b\b\b\b");
					printf("%4.1lf%%", 100*i/(double)count);
					last_progress=new_progress;
					fflush(stdout);
				}
			}
			if (++i==count) return result_done;
			return result_continue;
		}
		default: return result_continue;
	}
}

short int loop_randinit(short int command, struct loop_randinit_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct loop_randinit_data cmd_data;

	static struct run_parameters *parameters;
	static int *parameters_size;

	static int size;
	static int *index;
	
	switch (command)
	{
		case command_info: {printf("loop_randinit\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct loop_randinit_data));

			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;

			size=(parameters->xlen)*(parameters->ylen);
			index=(int*)malloc(sizeof(int)*size);

			return result_continue;
		}
		case command_run:
		{
			set_randlat(parameters, cmd_data.init_mag, index);
			return result_continue;
		}
		default: return result_continue;
	}
}

short int loop_randinit_lower_boundary(short int command, struct loop_randinit_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct loop_randinit_data cmd_data;

	static struct run_parameters *parameters;
	static int *parameters_size;

	static int size;
	static int *index_lower;
	
	switch (command)
	{
		case command_info: {printf("loop_randinit_lower_boundary\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct loop_randinit_data));

			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;

			size=(parameters->xlen);
			index_lower=(int*)malloc(sizeof(int)*size);

			return result_continue;
		}
		case command_run:
		{
			set_randlat_lower_boundary(parameters, cmd_data.init_mag, index_lower);
			return result_continue;
		}
		default: return result_continue;
	}
}

short int loop_randinit_upper_boundary(short int command, struct loop_randinit_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct loop_randinit_data cmd_data;

	static struct run_parameters *parameters;
	static int *parameters_size;

	static int size;
	static int *index_upper;
	
	switch (command)
	{
		case command_info: {printf("loop_randinit_upper_boundary\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct loop_randinit_data));

			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;

			size=(parameters->xlen);
			index_upper=(int*)malloc(sizeof(int)*size);

			return result_continue;
		}
		case command_run:
		{
			set_randlat_upper_boundary(parameters, cmd_data.init_mag, index_upper);
			return result_continue;
		}
		default: return result_continue;
	}
}

short int loop_seed(short int command, struct loop_seed_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct loop_seed_data cmd_data;
	
	static double *seed_low;
	static double *seed_high;
	static int *seed_size;

	static int i;

//	static int rank, size;
	
	static unsigned long int seed;
	
	switch (command)
	{
		case command_info: {printf("loop_seed\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct loop_seed_data));
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &seed_low, &seed_size, cmd_data.data_index_low, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &seed_high, &seed_size, cmd_data.data_index_high, data_size)==1) return result_error;

			// Get this process's rank.
//			MPI_Comm_rank(MPI_COMM_WORLD, &rank);

			// Get the total number of processes.
//			MPI_Comm_size(MPI_COMM_WORLD, &size);
			
			// Set the index into the list of seeds.
			// This ensures that processes of the same rank always get the same seeds.
			//
			// If this is the only process, then the list of seeds must be read
			// starting at the beginning.
			//
//			if (size==1) i=0;
//			else i=(rank-1)*(cmd_data.num);
			i=0;

			return result_continue;
		}
		case command_run:
		{
			seed=(unsigned long int)(*(seed_high+i));
			seed*=(unsigned long int)pow(2,16);
			seed*=(unsigned long int)pow(2,16);
			seed+=(unsigned long int)(*(seed_low+i));
			cmd_data.rng_struct->rng_seed(cmd_data.rng_struct, seed);
			i++;
			
			return result_continue;
		}
		default: return result_continue;
	}
}

short int loop_seed_tsc(short int command, struct loop_seed_tsc_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct loop_seed_tsc_data cmd_data;

	static unsigned int low, high;
	
	static unsigned long int seed;
	
	switch (command)
	{
		case command_info: {printf("loop_seed_tsc\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct loop_seed_tsc_data));
			return result_continue;
		}
		case command_run:
		{
			asm ("rdtsc" : "=a" (low), "=d" (high));
			seed=(unsigned long int)high;
			seed*=(unsigned long int)pow(2,16);
			seed*=(unsigned long int)pow(2,16);
			seed+=(unsigned long int)low;
			cmd_data.rng_struct->rng_seed(cmd_data.rng_struct, seed);

			return result_continue;
		}
		default: return result_continue;
	}
}

