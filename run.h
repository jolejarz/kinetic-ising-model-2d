/**************************************************************
***************************************************************
*** 
***   run.h
***
***   Jason Olejarz
***   2011-01-26
***
***   This is the header file for run.c.
***
***************************************************************
**************************************************************/

struct run_parameters
{
	struct rng_struct *rng_struct;
	int xlen;
	int ylen;
	unsigned long int maxsteps;
	double maxtime;
	double temp;
	double bias;
	short int *spins_initial;
	short int *spins;
	short int *state_initial;
	short int *state;
	int *map[25];
	int *index[25];
	double prob[25];
	int coordinate;
	short int move;
	unsigned long int steps;
	double time;
	double energy;		// This is the total energy of the system.
	int mag;		// This is the total magnetization of the system.
	double J2;
	short int periodic_bc_x;
	short int periodic_bc_y;
	short int *lower_boundary;
	short int *upper_boundary;
};

short int rundiag (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters, short int set_arrays);
short int runfixper (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters, short int set_arrays);
short int runfixper_bias (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters, short int set_arrays);
short int runperper (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters, short int set_arrays);
