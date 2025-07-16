/**************************************************************
***************************************************************
*** 
***   cmd-step.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains commands for intermediate processing.
***
***************************************************************
**************************************************************/

void cmd_energy()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_energy=arg_short_int[3];
	
	struct step_energy_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_energy=data_index_energy;
	
	step_energy(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_energy, step_num);

	return;
}

void cmd_equstop()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_temp=arg_short_int[2];
	short int data_index_mag=arg_short_int[3];
	short int data_index_energy=arg_short_int[4];
	short int data_index_equilibrium=arg_short_int[5];
	int temp_index=arg_int[0];
	short int mag_energy=arg_short_int[6];
	int num=arg_int[1];
	
	struct step_equilibrium_stop_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_temp=data_index_temp;
	command_data.data_index_mag=data_index_mag;
	command_data.data_index_energy=data_index_energy;
	command_data.data_index_equilibrium=data_index_equilibrium;
	command_data.temp_index=temp_index;
	command_data.mag_energy=mag_energy;
	command_data.size=num;
	
	step_equilibrium_stop(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_equilibrium_stop, step_num);

	return;
}

void cmd_equtable()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_temp=arg_short_int[2];
	short int data_index_mag=arg_short_int[3];
	short int data_index_mag_width=arg_short_int[4];
	short int data_index_energy=arg_short_int[5];
	short int data_index_energy_width=arg_short_int[6];
	short int slab_state=arg_short_int[7];
	unsigned long int min_steps=arg_unsigned_long_int[0];
	double min_time=arg_double[0];
	int runs=arg_int[0];
	
	struct step_equilibrium_table_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_temp=data_index_temp;
	command_data.data_index_mag=data_index_mag;
	command_data.data_index_mag_width=data_index_mag_width;
	command_data.data_index_energy=data_index_energy;
	command_data.data_index_energy_width=data_index_energy_width;
	command_data.slab_state=slab_state;
	command_data.min_steps=min_steps;
	command_data.min_time=min_time;
	command_data.runs=runs;
	
	step_equilibrium_table(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_equilibrium_table, step_num);

	return;
}

void cmd_flips()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_flips=arg_short_int[2];
	unsigned long int steps=arg_unsigned_long_int[0];
	
	struct step_flips_data flipsdata;
	
	flipsdata.data_index_parameters=data_index_parameters;
	flipsdata.data_index_flips=data_index_flips;
	flipsdata.steps=steps;
	step_flips(command_insert, &flipsdata, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_flips, step_num);
	
	return;
}

void cmd_mag()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_mag=arg_short_int[3];
	
	struct step_mag_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_mag=data_index_mag;
	
	step_mag(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_mag, step_num);

	return;
}

void cmd_movie1()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int spin=arg_short_int[2];
	int interval=arg_int[0];
	short int end_frame=arg_short_int[3];
	
	struct step_movie_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.spin=spin;
	command_data.interval=interval;
	command_data.end_frame=end_frame;
	
	step_movie_1(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_movie_1, step_num);

	return;
}

void cmd_movie2()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	double starting_time=arg_double[0];
	double ending_time=arg_double[1];
	int last_frame=arg_int[0];
	short int spin=arg_short_int[2];
	int interval=arg_int[1];
	
	struct step_movie_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.starting_time=starting_time;
	command_data.ending_time=ending_time;
	command_data.last_frame=last_frame;
	command_data.spin=spin;
	command_data.interval=interval;
	
	step_movie_2(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_movie_2, step_num);

	return;
}

void cmd_movie3()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	double starting_time=arg_double[0];
	double ending_time=arg_double[1];
	int last_frame=arg_int[0];
	short int spin=arg_short_int[2];
	int interval=arg_int[1];
	
	struct step_movie_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.starting_time=starting_time;
	command_data.ending_time=ending_time;
	command_data.last_frame=last_frame;
	command_data.spin=spin;
	command_data.interval=interval;
	
	step_movie_3(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_movie_3, step_num);

	return;
}

void cmd_slabstop()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_temp=arg_short_int[2];
	short int data_index_mag=arg_short_int[3];
	short int data_index_energy=arg_short_int[4];
	short int data_index_slab=arg_short_int[5];
	int temp_index=arg_int[0];
	double extra_time=arg_double[0];
	short int terminate=arg_short_int[6];
	int num=arg_int[1];
	
	struct step_slab_stop_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_temp=data_index_temp;
	command_data.data_index_mag=data_index_mag;
	command_data.data_index_energy=data_index_energy;
	command_data.data_index_slab=data_index_slab;
	command_data.temp_index=temp_index;
	command_data.extra_time=extra_time;
	command_data.terminate=terminate;
	command_data.size=num;
	
	step_slab_stop(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_slab_stop, step_num);

	return;
}

void cmd_stopdiag()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	double starting_time=arg_double[0];
	
	struct step_stop_diagonal_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.starting_time=starting_time;
	
	step_stop_diagonal(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_stop_diagonal, step_num);

	return;
}

void cmd_swstop()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_switch=arg_short_int[2];
	short int sign=arg_short_int[3];
	double fraction=arg_double[0];
	int num=arg_int[0];
	
	struct step_switch_stop_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_switch=data_index_switch;
	command_data.sign=sign;
	command_data.fraction=fraction;
	command_data.size=num;
	
	step_switch_stop(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_switch_stop, step_num);

	return;
}

void cmd_time()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	short int data_index_time=arg_short_int[3];
	
	struct step_time_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.data_index_time=data_index_time;
	
	step_time(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_time, step_num);

	return;

}

void cmd_trigs()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	unsigned long int steps_interval=arg_unsigned_long_int[0];
	short int progress=arg_short_int[3];
	
	struct step_trigger_steps_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.steps_interval=steps_interval;
	command_data.progress=progress;

	step_trigger_steps(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_trigger_steps, step_num);
	
	return;
}

void cmd_trigt()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	double time_interval=arg_double[0];
	short int progress=arg_short_int[3];
	
	struct step_trigger_time_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.time_interval=time_interval;
	command_data.progress=progress;

	step_trigger_time(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_trigger_time, step_num);

	return;
}

void cmd_trigtonce()
{
	short int step_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_trigger=arg_short_int[2];
	double time=arg_double[0];
	int runs=arg_int[0];
	
	struct step_trigger_time_once_data command_data;
	
	command_data.data_index_parameters=data_index_parameters;
	command_data.data_index_trigger=data_index_trigger;
	command_data.time=time;
	command_data.runs=runs;

	step_trigger_time_once(command_insert, &command_data, data, data_parameters, data_num);
	
	list_insert(step, step_index, step_trigger_time_once, step_num);
	
	return;
}

