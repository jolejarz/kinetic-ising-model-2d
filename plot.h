/**************************************************************
***************************************************************
*** 
***   plot.h
***
***   Jason Olejarz
***   2011-01-27
***
***   This is the header file for plot.c.
***
***************************************************************
**************************************************************/

void animate_real_time(short int *spins, int *flips, double *energy, double *time, int steps_begin, int steps, double time_between_frames, int nx, int ny, short int spin, double delay, double time_offset, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void animate_real_time_step(short int *spins, int *flips, double *energy, double *time, int steps_begin, int steps, int nx, int ny, int nz, short int spin, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void cluster_spins_plot(short int *cluster_spins, int nx, int ny, int nz, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void plot_frame_movie(short int *spins, int nx, int ny, double time, int image_number, short int spin, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void state_plot_matlab(short int *spins, int nx, int ny, int steps, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
void state_plot_matlab_fixed_boundaries(short int *spins, short int *lower_boundary, short int *upper_boundary, int nx, int ny, int steps, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot);
