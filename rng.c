/**************************************************************
***************************************************************
*** 
***   rng.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains the random number generator functions for ising3d.
***
***************************************************************
**************************************************************/

#include "includes.h"

// **************************************************
//   64-bit Linear Congruential Generator
// **************************************************

const unsigned long int a=2862933555777941757, b=1013904243;

// Contains the largest number that can be produced by the RNG plus 1.
double largest_random_number_plus_1;

//	algorithm:	x_(i+1) = x(i)*a + b

void rng_alloc_lcg (struct rng_struct *rng_struct)
{
	// The largest random number that can be generated plus 1 is 2^64.
	largest_random_number_plus_1=(double)pow(2,16);
	largest_random_number_plus_1*=(double)pow(2,16);
	largest_random_number_plus_1*=(double)pow(2,16);
	largest_random_number_plus_1*=(double)pow(2,16);

	return;
}

void rng_seed_lcg (struct rng_struct *rng_struct, unsigned long int seed)
{
	rng_struct->seed=seed;
	rng_struct->current_long_int=seed;
	rng_struct->current_double=(rng_struct->current_long_int)/largest_random_number_plus_1;
	
	return;
}

void rng_next_lcg (struct rng_struct *rng_struct)
{
	rng_struct->current_long_int=(rng_struct->current_long_int)*a+b;
	rng_struct->current_double=(rng_struct->current_long_int)/largest_random_number_plus_1;

	return;
}

// **************************************************
//   Mersenne Twister
// **************************************************

gsl_rng *rng_mt;

void rng_alloc_mt (struct rng_struct *rng_struct)
{
	rng_mt=gsl_rng_alloc (gsl_rng_mt19937);
	
	largest_random_number_plus_1=(double)(gsl_rng_max(rng_mt))+1;
	
	return;
}

void rng_seed_mt (struct rng_struct *rng_struct, unsigned long int seed)
{
	rng_struct->seed=seed;
	rng_struct->current_long_int=seed;
	rng_struct->current_double=(rng_struct->current_long_int)/largest_random_number_plus_1;
	
	gsl_rng_set (rng_mt, seed);
	
	return;
}

void rng_next_mt (struct rng_struct *rng_struct)
{
	rng_struct->current_long_int=gsl_rng_get(rng_mt);
	rng_struct->current_double=(rng_struct->current_long_int)/largest_random_number_plus_1;

	return;
}

