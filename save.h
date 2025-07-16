/**************************************************************
***************************************************************
*** 
***   save.h
***
***   Jason Olejarz
***   2010-07-21
***
***   This is the header file for save.c.
***
***************************************************************
**************************************************************/

struct save_final_blinkers_data
{
	short int data_index_parameters;
	short int data_index_number_of_blinker_regions;
	short int data_index_volume_of_blinker_regions;
	int size;
};
struct save_final_blinkers_parameters
{
	int size;
	int index;
};

struct save_final_clusters_data
{
	short int data_index_parameters;
	short int data_index_number_of_clusters;
	int size;
};
struct save_final_clusters_parameters
{
	int size;
	int index;
};

struct save_final_clusters_percolation_data
{
	short int data_index_parameters;
	short int data_index_number_of_clusters;
	short int data_index_number_of_stripes;
	short int data_index_winding_a;
	short int data_index_winding_b;
	int size;
};
struct save_final_clusters_percolation_parameters
{
	int size;
	int index;
};

struct save_final_corr_data
{
	short int data_index_parameters;
	short int data_index_distance;
	short int data_index_temp;
	short int data_index_corr_func;
	short int data_index_corr_length;
	short int data_index_wavenumber;
	short int data_index_temp_struct_fact_real;
	short int data_index_temp_struct_fact_imaginary;
	short int data_index_temp_struct_fact_magnitude;
	short int data_index_struct_fact_real;
	short int data_index_struct_fact_imaginary;
	short int data_index_struct_fact_magnitude;
	int size;
};
struct save_final_corr_parameters
{
	int size;
	int index;
};

struct save_final_corr_3d_data
{
	short int data_index_parameters;
	short int data_index_corr_func;
	short int data_index_struct_fact_real;
	short int data_index_struct_fact_imaginary;
	short int data_index_struct_fact;
	short int data_index_corr_func_minus_m_avg_sq;
	short int data_index_struct_fact_real_minus_m_avg_sq;
	short int data_index_struct_fact_imaginary_minus_m_avg_sq;
	short int data_index_struct_fact_minus_m_avg_sq;
	int size;
};
struct save_final_corr_3d_parameters
{
	int size;
	int index;
};

struct save_final_energy_data
{
	short int data_index_parameters;
	short int data_index_final_energy;
	int size;
};
struct save_final_energy_parameters
{
	int size;
	int index;
};

struct save_final_state_data
{
	short int data_index_parameters;
	short int data_index_final_state;
	int size;
};
struct save_final_state_parameters
{
	int size;
	int index;
};

struct save_final_interfaces_data
{
	short int data_index_parameters;
	short int data_index_num_interfaces;
	short int data_index_y_coords;
	short int data_index_y_coords_index;
	short int sign_of_bottom_boundary;
	int max_interfaces;
	int size;
};
struct save_final_interfaces_parameters
{
	int size;
	int index;
};

struct save_final_mag_data
{
	short int data_index_parameters;
	short int data_index_final_mag;
	int size;
};
struct save_final_mag_parameters
{
	int size;
	int index;
};

struct save_final_mag_abs_data
{
	short int data_index_parameters;
	short int data_index_final_mag;
	int size;
};
struct save_final_mag_abs_parameters
{
	int size;
	int index;
};

struct save_final_steps_data
{
	short int data_index_parameters;
	short int data_index_final_steps;
	int size;
};
struct save_final_steps_parameters
{
	int size;
	int index;
};

struct save_final_time_data
{
	short int data_index_parameters;
	short int data_index_final_time;
	int size;
};
struct save_final_time_parameters
{
	int size;
	int index;
};

struct save_increment_T_data
{
	short int data_index_parameters;
	double increment;
};
struct save_increment_T_parameters
{
};

struct save_perc_data
{
	short int data_index_parameters;
	short int data_index_max_perc;
	short int data_index_perc_x;
	short int data_index_perc_y;
	short int data_index_perc_z;
	short int data_index_equilibrium;
	int size;
};
struct save_perc_parameters
{
	int size;
	int index;
};

struct save_seed_data
{
	short int data_index_rng;
	short int data_index_low;
	short int data_index_high;
	int size;
};
struct save_seed_parameters
{
	int size;
	int index;
};

struct save_value_data
{
	short int data_index;
	int size;
};
struct save_value_parameters
{
	int size;
	int index;
};

short int save_final_blinkers(short int command, struct save_final_blinkers_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_clusters(short int command, struct save_final_clusters_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_clusters_percolation(short int command, struct save_final_clusters_percolation_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_corr(short int command, struct save_final_corr_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_corr_3d(short int command, struct save_final_corr_3d_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_energy(short int command, struct save_final_energy_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_state_periodic(short int command, struct save_final_state_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_interfaces(short int command, struct save_final_interfaces_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_mag(short int command, struct save_final_mag_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_mag_abs(short int command, struct save_final_mag_abs_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_steps(short int command, struct save_final_steps_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_final_time(short int command, struct save_final_time_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_increment_T(short int command, struct save_increment_T_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_percolation(short int command, struct save_perc_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int save_seed(short int command, struct save_seed_data *command_data, void *data[], void *data_parameters[], short int data_size);
