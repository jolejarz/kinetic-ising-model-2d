/**************************************************************
***************************************************************
*** 
***   cmd-run.c
***
***   Jason Olejarz
***   2011-01-25
***
***   This file contains the command for running the simulations.
***
***************************************************************
**************************************************************/

void cmd_rundiag()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int parameters_data_index=arg_short_int[0];
	short int set_arrays=arg_short_int[1];
	
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, rundiag, parameters, set_arrays);

	return;
}

void cmd_runfixper()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int parameters_data_index=arg_short_int[0];
	short int set_arrays=arg_short_int[1];
	
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, runfixper, parameters, set_arrays);

	return;
}

void cmd_runperper()
{
	// If this is the master process, do not do any calculation.
//	int rank;
//	int size;
//	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//	if (rank==0 && size>1) return;
	
	short int parameters_data_index=arg_short_int[0];
	short int set_arrays=arg_short_int[1];
	
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, parameters_data_index, data_num)==1) return result_error;

	calc(loop, step, save, loop_num, step_num, save_num, data_num, data, data_parameters, runperper, parameters, set_arrays);

	return;
}
