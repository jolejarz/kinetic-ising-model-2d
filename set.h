/**************************************************************
***************************************************************
*** 
***   set.h
***
***   Jason Olejarz
***   2011-01-26
***
***   This is the header file for set.c.
***
***************************************************************
**************************************************************/

enum {set_error, set_ok};

short int set_antiferro(struct run_parameters *parameters, short int first_spin);
short int set_block(struct run_parameters *parameters, short int spin, int xi, int xf, int yi, int yf, int zi, int zf);
short int set_blockrev(struct run_parameters *parameters, int xi, int xf, int yi, int yf, int zi, int zf);
short int set_diag_1(struct run_parameters *parameters);
short int set_diag_2(struct run_parameters *parameters);
short int set_randlat(struct run_parameters *parameters, double init_mag, int *index);
short int set_randlat_lower_boundary(struct run_parameters *parameters, double init_mag, int *index);
short int set_randlat_upper_boundary(struct run_parameters *parameters, double init_mag, int *index);
short int set_seed(struct rng_struct *rng_struct, double seed_low, double seed_high);
short int set_settsc(struct rng_struct *rng_struct);

