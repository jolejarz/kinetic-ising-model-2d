/**************************************************************
***************************************************************
*** 
***   calc.c
***
***   Jason Olejarz
***   2011-01-25
***
***   This file contains the routine used to obtain data.
***
***************************************************************
**************************************************************/

#include "includes.h"

void calc(short int (*loop[])(), short int (*step[])(), short int (*save[])(), short int loop_size, short int step_size, short int save_size, short int data_size, void *data[], void *data_parameters[], short int (*run)(), void *parameters, short int set_arrays)
{
	// If 'next' is set to one, then there is another iteration of the loop.
	int result, next=1;
	
	short int loop_index=0, save_index=0;
	
	// Send 'begin' to all functions in 'loop' and 'save'.
	while (loop[loop_index]!=0 && loop_index<loop_size)
	{
		if (loop[loop_index++](command_begin, 0, data, data_parameters, data_size)==result_error) goto calc_end;
	}
	loop_index=0;
	while (save[save_index]!=0 && save_index<save_size)
	{
		if (save[save_index++](command_begin, 0, data, data_parameters, data_size)==result_error) goto calc_end;
	}
	save_index=0;

	do
	{
		// Each function in 'loop' is for preprocessing.
		// 'evolve' is for running the simulation.
		// Each function in 'save' is for postprocessing.

		// Do preprocessing.
		while (loop[loop_index]!=0 && loop_index<loop_size)
		{
			result=loop[loop_index++](command_run, 0, data, data_parameters, data_size);
			if (result==result_error) goto calc_end;
			else if (result==result_done) next=0;
		}
		loop_index=0;

		// Run the simulation.
		if (run(	step,
				step_size,
				data,
				data_parameters,
				data_size,
				parameters,
				set_arrays)
			==result_error) goto calc_end;

		// Do postprocessing.
		while (save[save_index]!=0 && save_index<save_size)
		{
			result=save[save_index++](command_run, 0, data, data_parameters, data_size);
			if (result==result_error) goto calc_end;
			else if (result==result_skip) break;
		}
		save_index=0;
	}
	while (next);

	// Send 'end' to all functions in 'loop' and 'save'.
	while (loop[loop_index]!=0 && loop_index<loop_size)
	{
		if (loop[loop_index++](command_end, 0, data, data_parameters, data_size)==result_error) goto calc_end;
	}
	loop_index=0;
	while (save[save_index]!=0 && save_index<save_size)
	{
		if (save[save_index++](command_end, 0, data, data_parameters, data_size)==result_error) goto calc_end;
	}
	save_index=0;

	calc_end:
	
	return;
}

