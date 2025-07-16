/**************************************************************
***************************************************************
*** 
***   cmd-set.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains commands for setting parameters.
***
***************************************************************
**************************************************************/

void cmd_af()
{
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, arg_short_int[0], data_num)==1) return result_error;

	set_antiferro(parameters, arg_short_int[1]);

	return;
}

void cmd_blkinit()
{
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, arg_short_int[0], data_num)==1) return result_error;

        // Creates an initial lattice of blocks with alternating spins.
        // The parameter passed is the linear size of each block.
        // The first block (starting at (x,y,z)=(1,1,1)) is filled with up spins.

        if (parameters->xlen!=parameters->ylen/* || parameters->ylen!=parameters->zlen || parameters->zlen!=parameters->xlen*/)
        {
                printf("The lattice must be square to use 'blkinit'.\n");
                return;
        }   
            
        int size=parameters->xlen;
        int block_size=arg_int[0];

        if (size%(2*block_size)!=0)
        {   
                printf("An integer number of up/down block combinations must fit within the linear size of the lattice.\n");
                return;
        }   
                    
        int x,y,z;
        int spin=1;
            
        for (z=1; z<=size; z+=block_size)
        {   
                for (y=1; y<=size; y+=block_size)
                {   
                        for (x=1; x<=size; x+=block_size)
                        {   
                                set_block(parameters, spin, x, x+block_size-1, y, y+block_size-1, z, z+block_size-1);
                                spin*=-1;
                        }   
                        spin*=-1;
                }
                spin*=-1;
        }

	return;
}

void cmd_block()
{
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, arg_short_int[0], data_num)==1) return result_error;

	set_block(parameters, arg_short_int[1], arg_int[0], arg_int[1], arg_int[2], arg_int[3], arg_int[4], arg_int[5]);

	return;
}

void cmd_blockrev()
{
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, arg_short_int[0], data_num)==1) return result_error;

	set_blockrev(parameters, arg_int[0], arg_int[1], arg_int[2], arg_int[3], arg_int[4], arg_int[5]);

	return;
}

void cmd_cpcur()
{
	short int data_index=arg_short_int[0];
	
	struct run_parameters *p;
	int *p_size;
	
	if (data_input(data, data_parameters, &p, &p_size, data_index, data_num)==1) return result_error;
	
	memcpy (p->spins_initial, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));

	return;
}

void cmd_cpinit()
{
	short int data_index=arg_short_int[0];
	
	struct run_parameters *p;
	int *p_size;
	
	if (data_input(data, data_parameters, &p, &p_size, data_index, data_num)==1) return result_error;
	
	memcpy (p->spins, p->spins_initial, sizeof(short int)*(p->xlen)*(p->ylen));
	
	return;
}

void cmd_diag1()
{
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, arg_short_int[0], data_num)==1) return result_error;

	set_diag_1(parameters);

	return;
}

void cmd_diag2()
{
	struct run_parameters *parameters;
	int *parameters_size;

	if (data_input(data, data_parameters, &parameters, &parameters_size, arg_short_int[0], data_num)==1) return result_error;

	set_diag_2(parameters);

	return;
}

void cmd_reset()
{
	short int data_index=arg_short_int[0];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	double temp=arg_double[1];
	double bias=arg_double[2];
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	int i, x, y;

	// Load the parameters structure.
	if (data_input(data, data_parameters, &parameters, &parameters_size, data_index, data_num)==1) return result_error;

	// Reset the parameters.
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->temp=temp;
	parameters->bias=bias;

	return;
}

void cmd_rnglcg()
{
	short int data_index=arg_short_int[0];
	
	struct rng_struct *rng_struct;
	int *rng_struct_size;
	
	// Allocate a block of memory to store info for the random number generator.
	if (data_output(data, data_parameters, &rng_struct, &rng_struct_size, sizeof(struct rng_struct), sizeof(int), data_index, data_num)==1) return result_error;
	
	rng_struct->rng_alloc=rng_alloc_lcg;
	rng_struct->rng_seed=rng_seed_lcg;
	rng_struct->rng_next=rng_next_lcg;

	rng_struct->rng_alloc(rng_struct);
	
	return;
}

void cmd_rngmt()
{
	short int data_index=arg_short_int[0];
	
	struct rng_struct *rng_struct;
	int *rng_struct_size;
	
	// Allocate a block of memory to store info for the random number generator.
	if (data_output(data, data_parameters, &rng_struct, &rng_struct_size, sizeof(struct rng_struct), sizeof(int), data_index, data_num)==1) return result_error;
	
	rng_struct->rng_alloc=rng_alloc_mt;
	rng_struct->rng_seed=rng_seed_mt;
	rng_struct->rng_next=rng_next_mt;

	rng_struct->rng_alloc(rng_struct);
	
	return;
}

void cmd_seed()
{
	set_seed(data[arg_short_int[0]], arg_double[0], arg_double[1]);

	return;
}

void cmd_setdiag()
{
	short int data_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	int xlen=arg_int[0];
	int ylen=arg_int[1];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	double temp=arg_double[1];
	double bias=arg_double[2];
	double J2=arg_double[3];
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	int i, x, y;

	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &parameters, &parameters_size, sizeof(struct run_parameters), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	parameters->rng_struct=data[data_index_rng];
	parameters->xlen=xlen;
	parameters->ylen=ylen;
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->temp=temp;
	parameters->bias=bias;
	parameters->J2=J2;
	parameters->periodic_bc_x=1;
	parameters->periodic_bc_y=1;

	// Allocate memory.
	parameters->spins_initial=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->spins=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->state_initial=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->state=(short int*)malloc (sizeof(short int)*xlen*ylen);
	for (i=0; i<25; i++)
	{
		parameters->map[i]=(int*)malloc (sizeof(int)*xlen*ylen);
		parameters->index[i]=(int*)malloc (sizeof(int)*(1+xlen*ylen));
	}

	return;
}

void cmd_setfixper()
{
	short int data_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	int xlen=arg_int[0];
	int ylen=arg_int[1];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	double temp=arg_double[1];
	double bias=arg_double[2];
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	int i, x, y;

	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &parameters, &parameters_size, sizeof(struct run_parameters), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	parameters->rng_struct=data[data_index_rng];
	parameters->xlen=xlen;
	parameters->ylen=ylen;
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->temp=temp;
	parameters->bias=bias;
	parameters->periodic_bc_x=1;
	parameters->periodic_bc_y=0;

	// Allocate memory.
	parameters->spins_initial=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->spins=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->state_initial=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->state=(short int*)malloc (sizeof(short int)*xlen*ylen);
	for (i=0; i<5; i++)
	{
		parameters->map[i]=(int*)malloc (sizeof(int)*xlen*ylen);
		parameters->index[i]=(int*)malloc (sizeof(int)*(1+xlen*ylen));
	}
	parameters->lower_boundary=(short int*)malloc (sizeof(short int)*xlen);
	parameters->upper_boundary=(short int*)malloc (sizeof(short int)*xlen);

	return;
}

void cmd_setperper()
{
	short int data_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	int xlen=arg_int[0];
	int ylen=arg_int[1];
	unsigned long int maxsteps=arg_unsigned_long_int[0];
	double maxtime=arg_double[0];
	double temp=arg_double[1];
	double bias=arg_double[2];
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	int i, x, y;

	// Allocate a block of memory to store parameter values.
	if (data_output(data, data_parameters, &parameters, &parameters_size, sizeof(struct run_parameters), sizeof(int), data_index, data_num)==1) return result_error;

	// Fill in the parameters.
	parameters->rng_struct=data[data_index_rng];
	parameters->xlen=xlen;
	parameters->ylen=ylen;
	parameters->maxsteps=maxsteps;
	parameters->maxtime=maxtime;
	parameters->temp=temp;
	parameters->bias=bias;
	parameters->periodic_bc_x=1;
	parameters->periodic_bc_y=1;

	// Allocate memory.
	parameters->spins_initial=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->spins=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->state_initial=(short int*)malloc (sizeof(short int)*xlen*ylen);
	parameters->state=(short int*)malloc (sizeof(short int)*xlen*ylen);
	for (i=0; i<5; i++)
	{
		parameters->map[i]=(int*)malloc (sizeof(int)*xlen*ylen);
		parameters->index[i]=(int*)malloc (sizeof(int)*(1+xlen*ylen));
	}

	return;
}

void cmd_settsc()
{
	set_settsc(data[arg_short_int[0]]);
	return;
}

