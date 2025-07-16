/**************************************************************
***************************************************************
*** 
***   rng.h
***
***   Jason Olejarz
***   2010-07-21
***
***   This is the header file for rng.c.
***
***************************************************************
**************************************************************/

struct rng_struct
{
	void (*rng_alloc)(struct rng_struct *rng_struct);
	void (*rng_seed)(struct rng_struct *rng_struct, unsigned long int seed);
	void (*rng_next)(struct rng_struct *rng_struct);
	unsigned long int seed;
	unsigned long int current_long_int;
	double current_double;
};

void rng_alloc_lcg (struct rng_struct *rng_struct);
void rng_seed_lcg (struct rng_struct *rng_struct, unsigned long int seed);
void rng_next_lcg (struct rng_struct *rng_struct);
void rng_alloc_mt (struct rng_struct *rng_struct);
void rng_seed_mt (struct rng_struct *rng_struct, unsigned long int seed);
void rng_next_mt (struct rng_struct *rng_struct);

