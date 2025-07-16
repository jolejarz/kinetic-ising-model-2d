/**************************************************************
***************************************************************
*** 
***   set.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains the routines used to set the parameters for 'evolve'.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int set_antiferro(struct run_parameters *parameters, short int first_spin)
{
	int i, xlen, ylen, zlen, xlentimesylen, xlentimesylentimeszlen;
	
	xlen=parameters->xlen; ylen=parameters->ylen;// zlen=parameters->zlen;
	xlentimesylen=xlen*ylen;// xlentimesylentimeszlen=xlentimesylen*zlen;

	if ((xlen*ylen*zlen)%2==1)
	{
		printf("For an antiferromagnetic initial state, xlen*ylen*zlen must be even.\n");
		return set_error;
	}
	
	*(parameters->spins_initial)=first_spin;

//	for (i=1; i<xlentimesylentimeszlen; i++)
	for (i=1; i<xlentimesylen; i++)
	{
		*(parameters->spins_initial+i)=-(*(parameters->spins_initial+i-1));

//		if (i%xlentimesylen==0)
//		{
//			if (xlen%2==1 && ylen%2==0 || xlen%2==0 && ylen%2==1) *(parameters->spins_initial+i)=-(*(parameters->spins_initial+i));
//		}
//		else if (i%xlen==0 && xlen%2==0) *(parameters->spins_initial+i)=-(*(parameters->spins_initial+i));
		if (i%xlen==0) *(parameters->spins_initial+i)=-(*(parameters->spins_initial+i));
	}

	return set_ok;
}

short int set_block(struct run_parameters *parameters, short int spin, int xi, int xf, int yi, int yf, int zi, int zf)
{
	if (xi<1 || xf>parameters->xlen || yi<1 || yf>parameters->ylen/* || zi<1 || zf>parameters->zlen*/)
	{
		printf("%d %d %d %d Invalid coordinates for the block.\n",xi,xf,yi,yf);
		return set_error;
	}
	
	int x, y, z, xlen, nxtimesny;
	
	xlen=parameters->xlen;
	nxtimesny=(parameters->xlen)*(parameters->ylen);
	
//	for (z=zi; z<=zf; z++)
//	{
		for (y=yi; y<=yf; y++)
		{
			for (x=xi; x<=xf; x++)
			{
				*(parameters->spins_initial+(x-1)+(y-1)*xlen)=spin;
			}
		}
//	}
	
	return set_ok;
}

short int set_blockrev(struct run_parameters *parameters, int xi, int xf, int yi, int yf, int zi, int zf)
{
	if (xi<1 || xf>parameters->xlen || yi<1 || yf>parameters->ylen/* || zi<1 || zf>parameters->zlen*/)
	{
		printf("Invalid coordinates for the block.\n");
		return set_error;
	}
	
	int x, y, z, xlen, nxtimesny;
	
	xlen=parameters->xlen;
	nxtimesny=(parameters->xlen)*(parameters->ylen);
	
//	for (z=zi; z<=zf; z++)
//	{
		for (y=yi; y<=yf; y++)
		{
			for (x=xi; x<=xf; x++)
			{
				*(parameters->spins_initial+(x-1)+(y-1)*xlen)=-*(parameters->spins_initial+(x-1)+(y-1));
			}
		}
//	}
	
	return set_ok;
}

short int set_diag_1(struct run_parameters *parameters)
{
	int i, xlen, ylen, zlen, xlentimesylen, xlentimesylentimeszlen;

	int x, y, xnext, start_down, end_down;
	
	xlen=parameters->xlen; ylen=parameters->ylen;// zlen=parameters->zlen;
	xlentimesylen=xlen*ylen;// xlentimesylentimeszlen=xlentimesylen*zlen;

	if (xlen%2==1 || ylen%2==1 || zlen%2==1)
	{
		printf("The lattice must be even.\n");
		return set_error;
	}
	
	for (y=0; y<ylen; y++)
	{
		start_down=y;
		end_down=(start_down+xlen/2)%xlen;
		x=start_down;

		do
		{
			xnext=(x+xlen/2)%xlen;

			*(parameters->spins_initial+x+y*xlen)=-1;
			*(parameters->spins_initial+xnext+y*xlen)=1;

			x++;
			if (x%xlen==0) x=0;
		}
		while (x!=end_down);
	}

	return set_ok;
}

short int set_diag_2(struct run_parameters *parameters)
{
	int i, xlen, ylen, zlen, xlentimesylen, xlentimesylentimeszlen;

	int x, y, xnext1, xnext2, xnext3, start_down, end_down;
	
	xlen=parameters->xlen; ylen=parameters->ylen;// zlen=parameters->zlen;
	xlentimesylen=xlen*ylen;// xlentimesylentimeszlen=xlentimesylen*zlen;

	if (xlen%2==1 || ylen%2==1 || zlen%2==1)
	{
		printf("The lattice must be even.\n");
		return set_error;
	}
	
	for (y=0; y<ylen; y++)
	{
		start_down=y/2;
		end_down=(start_down+xlen/4)%xlen;
		x=start_down;

		do
		{
			xnext1=(x+xlen/4)%xlen;
			xnext2=(xnext1+xlen/4)%xlen;
			xnext3=(xnext2+xlen/4)%xlen;

			*(parameters->spins_initial+x+y*xlen)=-1;
			*(parameters->spins_initial+xnext1+y*xlen)=1;
			*(parameters->spins_initial+xnext2+y*xlen)=-1;
			*(parameters->spins_initial+xnext3+y*xlen)=1;

			x++;
			if (x%xlen==0) x=0;
		}
		while (x!=end_down);
	}

	return set_ok;
}

short int set_randlat(struct run_parameters *parameters, double init_mag, int *index)
{
	long double largest_random_number_plus_1;
	long double random_number;
	
	// The largest random number that can be generated plus 1 is 2^64.
	largest_random_number_plus_1=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);

	int xlen, ylen, zlen;
	xlen=parameters->xlen; ylen=parameters->ylen;// zlen=parameters->zlen;

//	int area=(parameters->xlen)*(parameters->ylen);
//	int size=(parameters->xlen)*(parameters->ylen)*(parameters->zlen);
	int size=(parameters->xlen)*(parameters->ylen);
	
	int index_size;
	
	int i, j;
	short int initial_spin;
	int num_flips;

	int x, y, z;
	
	initial_spin=init_mag>=0 ? 1 : -1;
	
	for (i=0; i<size; i++)
	{
		*(parameters->spins_initial+i)=initial_spin;
	}
	
	for (i=0; i<size; i++)
	{
		*(index+i)=i;
	}

	index_size=size;

	num_flips=(int)(size/2*(1-sqrt(init_mag*init_mag)));

	for (i=0; i<num_flips; i++)
	{
		// Generate a random number.
		(parameters->rng_struct)->rng_next(parameters->rng_struct);
		random_number=((parameters->rng_struct)->current_double);

		j=(int)(index_size*random_number);
		
		*(parameters->spins_initial+(*(index+j)))=-*(parameters->spins_initial+(*(index+j)));
		
		*(index+j)=*(index+index_size-1);

		index_size--;
	}
	
	return set_ok;
}

short int set_randlat_lower_boundary(struct run_parameters *parameters, double init_mag, int *index)
{
	long double largest_random_number_plus_1;
	long double random_number;
	
	// The largest random number that can be generated plus 1 is 2^64.
	largest_random_number_plus_1=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);

	int xlen, ylen, zlen;
	xlen=parameters->xlen; ylen=parameters->ylen;// zlen=parameters->zlen;

//	int area=(parameters->xlen)*(parameters->ylen);
//	int size=(parameters->xlen)*(parameters->ylen)*(parameters->zlen);
	int size=(parameters->xlen);
	
	int index_size;
	
	int i, j;
	short int initial_spin;
	int num_flips;

	int x, y, z;
	
	initial_spin=init_mag>=0 ? 1 : -1;
	
	for (i=0; i<size; i++)
	{
		*(parameters->lower_boundary+i)=initial_spin;
	}
	
	for (i=0; i<size; i++)
	{
		*(index+i)=i;
	}

	index_size=size;

	num_flips=(int)(size/2*(1-sqrt(init_mag*init_mag)));

	for (i=0; i<num_flips; i++)
	{
		// Generate a random number.
		(parameters->rng_struct)->rng_next(parameters->rng_struct);
		random_number=((parameters->rng_struct)->current_double);

		j=(int)(index_size*random_number);
		
		*(parameters->lower_boundary+(*(index+j)))=-*(parameters->lower_boundary+(*(index+j)));
		
		*(index+j)=*(index+index_size-1);

		index_size--;
	}
	
	return set_ok;
}

short int set_randlat_upper_boundary(struct run_parameters *parameters, double init_mag, int *index)
{
	long double largest_random_number_plus_1;
	long double random_number;
	
	// The largest random number that can be generated plus 1 is 2^64.
	largest_random_number_plus_1=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);
	largest_random_number_plus_1*=(long double)pow(2,16);

	int xlen, ylen, zlen;
	xlen=parameters->xlen; ylen=parameters->ylen;// zlen=parameters->zlen;

//	int area=(parameters->xlen)*(parameters->ylen);
//	int size=(parameters->xlen)*(parameters->ylen)*(parameters->zlen);
	int size=(parameters->xlen);
	
	int index_size;
	
	int i, j;
	short int initial_spin;
	int num_flips;

	int x, y, z;
	
	initial_spin=init_mag>=0 ? 1 : -1;
	
	for (i=0; i<size; i++)
	{
		*(parameters->upper_boundary+i)=initial_spin;
	}
	
	for (i=0; i<size; i++)
	{
		*(index+i)=i;
	}

	index_size=size;

	num_flips=(int)(size/2*(1-sqrt(init_mag*init_mag)));

	for (i=0; i<num_flips; i++)
	{
		// Generate a random number.
		(parameters->rng_struct)->rng_next(parameters->rng_struct);
		random_number=((parameters->rng_struct)->current_double);

		j=(int)(index_size*random_number);
		
		*(parameters->upper_boundary+(*(index+j)))=-*(parameters->upper_boundary+(*(index+j)));
		
		*(index+j)=*(index+index_size-1);

		index_size--;
	}
	
	return set_ok;
}

short int set_seed(struct rng_struct *rng_struct, double seed_low, double seed_high)
{
	unsigned long int seed;
	
	seed=(unsigned long int)seed_high;
	seed*=(unsigned long int)pow(2,16);
	seed*=(unsigned long int)pow(2,16);
	seed+=(unsigned long int)seed_low;
	rng_struct->rng_seed(rng_struct, seed);

	return set_ok;
}

short int set_settsc(struct rng_struct *rng_struct)
{
	unsigned int low, high;
	unsigned long int seed;
	
	asm ("rdtsc" : "=a" (low), "=d" (high));
	seed=(unsigned long int)high;
	seed*=(unsigned long int)pow(2,16);
	seed*=(unsigned long int)pow(2,16);
	seed+=(unsigned long int)low;
	rng_struct->rng_seed(rng_struct, seed);
	
	return set_ok;
}

