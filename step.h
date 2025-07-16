/**************************************************************
***************************************************************
*** 
***   step.h
***
***   Jason Olejarz
***   2011-01-27
***
***   This is the header file for step.c.
***
***************************************************************
**************************************************************/

struct step_average_deltae_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_average_deltae;
};
struct step_average_deltae_parameters
{
	int size;
	int index;
};

struct step_energy_drops_data
{
	short int data_index_parameters;
	short int data_index_time;
	short int data_index_energy_drop;
	int max;
	short int data_index_num_energy_drops;
	short int data_index_size_energy_drops;
	short int data_index_time_duration_energy_drops;
	double starting_time;
	double ending_time;
	double minimum_time_length;
	double minimum_energy_drop;
	int num;
};
struct step_energy_drops_parameters
{
	int size;
	int index;
};

struct step_energy_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_energy;
};
struct step_energy_parameters
{
	int size;
	int index;
};

struct step_equilibrium_stop_data
{
	short int data_index_parameters;
	short int data_index_temp;
	short int data_index_mag;
	short int data_index_energy;
	short int data_index_equilibrium;
	int temp_index;
	short int mag_energy;
	int size;
};
struct step_equilibrium_stop_parameters
{
	int size;
	int index;
};

struct step_equilibrium_table_data
{
	short int data_index_parameters;
	short int data_index_temp;
	short int data_index_mag;
	short int data_index_mag_width;
	short int data_index_energy;
	short int data_index_energy_width;
	short int slab_state;
	unsigned long int min_steps;
	double min_time;
	int runs;
};
struct step_equilibrium_table_parameters
{
	int size;
	int index;
};

struct step_flips_data
{
	short int data_index_parameters;
	short int data_index_flips;
	int steps;
};
struct step_flips_parameters
{
	int size;
	int index;
};

struct step_mag_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_mag;
};
struct step_mag_parameters
{
	int size;
	int index;
};

struct step_movie_data
{
	short int data_index_parameters;
	double starting_time;
	double ending_time;
	int last_frame;
	short int spin;
	int interval;
	short int end_frame;
};
struct step_movie_parameters
{
};

struct step_slab_stop_data
{
	short int data_index_parameters;
	short int data_index_temp;
	short int data_index_mag;
	short int data_index_energy;
	short int data_index_slab;
	int temp_index;
	double extra_time;
	short int terminate;
	int size;
};
struct step_slab_stop_parameters
{
	int size;
	int index;
};

struct step_slab_stop_noncubic_data
{
	short int data_index_parameters;
	short int data_index_temp;
	short int data_index_mag;
	short int data_index_energy;
	short int data_index_slab;
	int temp_index;
	double extra_time;
	short int terminate;
	short int percolation_in_x;
	short int percolation_in_y;
	short int percolation_in_z;
	int number_of_slabs;
	int size;
};
struct step_slab_stop_noncubic_parameters
{
	int size;
	int index;
};

struct step_stop_diagonal_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	double starting_time;
};
struct step_stop_diagonal_parameters
{
	int size;
	int index;
};

struct step_switch_stop_data
{
	short int data_index_parameters;
	short int data_index_switch;
	short int sign;
	double fraction;
	int size;
};
struct step_switch_stop_parameters
{
	int size;
	int index;
};

struct step_time_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	short int data_index_time;
};
struct step_time_parameters
{
	int size;
	int index;
};

struct step_trigger_steps_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	unsigned long int steps_interval;
	short int progress;
};
struct step_trigger_time_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	double time_interval;
	short int progress;
};
struct step_trigger_time_once_data
{
	short int data_index_parameters;
	short int data_index_trigger;
	double time;
	int runs;
};
// The parameters data index for a trigger is the same whether the trigger is after a number of steps
// have elapsed or after an amount of time has elapsed.
struct step_trigger_parameters
{
	int total;
	int current;
	unsigned long int steps;
	double time;
};

short int step_average_deltae(short int command, struct step_average_deltae_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_energy_drops(short int command, struct step_energy_drops_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_energy(short int command, struct step_energy_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_equilibrium_stop(short int command, struct step_equilibrium_stop_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_equilibrium_table(short int command, struct step_equilibrium_table_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_flips(short int command, struct step_flips_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_mag(short int command, struct step_mag_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_movie_1(short int command, struct step_movie_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_movie_2(short int command, struct step_movie_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_movie_3(short int command, struct step_movie_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_slab_stop(short int command, struct step_slab_stop_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_slab_stop_noncubic_0(short int command, struct step_slab_stop_noncubic_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_slab_stop_noncubic_1(short int command, struct step_slab_stop_noncubic_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_stop_diagonal(short int command, struct step_stop_diagonal_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_switch_stop(short int command, struct step_switch_stop_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_time(short int command, struct step_time_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_trigger_steps(short int command, struct step_trigger_steps_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_trigger_time(short int command, struct step_trigger_time_data *command_data, void *data[], void *data_parameters[], short int data_size);
short int step_trigger_time_once(short int command, struct step_trigger_time_once_data *command_data, void *data[], void *data_parameters[], short int data_size);

