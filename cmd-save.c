/**************************************************************
***************************************************************
*** 
***   cmd-save.c
***
***   Jason Olejarz
***   2011-03-09
***
***   This file contains commands for setting up postprocessing.
***
***************************************************************
**************************************************************/

void cmd_fbr()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_number_of_blinker_regions=arg_short_int[2];
	short int data_index_volume_of_blinker_regions=arg_short_int[3];
	int num=arg_int[0];
	
	struct save_final_blinkers_data finalblinkersdata;

	finalblinkersdata.data_index_parameters=data_index_parameters;
	finalblinkersdata.data_index_number_of_blinker_regions=data_index_number_of_blinker_regions;
	finalblinkersdata.data_index_volume_of_blinker_regions=data_index_volume_of_blinker_regions;
	finalblinkersdata.size=num;
	save_final_blinkers(command_insert, &finalblinkersdata, data, data_parameters, data_num);
	
	list_insert(save, save_index, save_final_blinkers, save_num);
	
	return;
}

void cmd_fcl()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_number_of_clusters=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_clusters_data finalclustersdata;

	finalclustersdata.data_index_parameters=data_index_parameters;
	finalclustersdata.data_index_number_of_clusters=data_index_number_of_clusters;
	finalclustersdata.size=num;
	save_final_clusters(command_insert, &finalclustersdata, data, data_parameters, data_num);
	
	list_insert(save, save_index, save_final_clusters, save_num);
	
	return;
}

void cmd_fe()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_energy=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_energy_data finalenergydata;

	finalenergydata.data_index_parameters=data_index_parameters;
	finalenergydata.data_index_final_energy=data_index_final_energy;
	finalenergydata.size=num;
	save_final_energy(command_insert, &finalenergydata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_energy, save_num);
	
	return;
}

void cmd_finalper()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_state=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_state_data finalstatedata;

	finalstatedata.data_index_parameters=data_index_parameters;
	finalstatedata.data_index_final_state=data_index_final_state;
	finalstatedata.size=num;
	save_final_state_periodic(command_insert, &finalstatedata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_state_periodic, save_num);
	
	return;
}

void cmd_fintf()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_num_interfaces=arg_short_int[2];
	short int data_index_y_coords=arg_short_int[3];
	short int data_index_y_coords_index=arg_short_int[4];
	short int sign_of_bottom_boundary=arg_short_int[5];
	int max_interfaces=arg_int[0];
	int num=arg_int[1];
	
	struct save_final_interfaces_data finalinterfacesdata;

	finalinterfacesdata.data_index_parameters=data_index_parameters;
	finalinterfacesdata.data_index_num_interfaces=data_index_num_interfaces;
	finalinterfacesdata.data_index_y_coords=data_index_y_coords;
	finalinterfacesdata.data_index_y_coords_index=data_index_y_coords_index;
	finalinterfacesdata.sign_of_bottom_boundary=sign_of_bottom_boundary;
	finalinterfacesdata.max_interfaces=max_interfaces;
	finalinterfacesdata.size=num;
	save_final_interfaces(command_insert, &finalinterfacesdata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_interfaces, save_num);
	
	return;
}

void cmd_fm()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_mag=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_mag_data finalmagdata;

	finalmagdata.data_index_parameters=data_index_parameters;
	finalmagdata.data_index_final_mag=data_index_final_mag;
	finalmagdata.size=num;
	save_final_mag(command_insert, &finalmagdata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_mag, save_num);
	
	return;
}

void cmd_fmabs()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_mag=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_mag_abs_data finalmagabsdata;

	finalmagabsdata.data_index_parameters=data_index_parameters;
	finalmagabsdata.data_index_final_mag=data_index_final_mag;
	finalmagabsdata.size=num;
	save_final_mag_abs(command_insert, &finalmagabsdata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_mag_abs, save_num);
	
	return;
}

void cmd_fperc()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_number_of_clusters=arg_short_int[2];
	short int data_index_number_of_stripes=arg_short_int[3];
	short int data_index_winding_a=arg_short_int[4];
	short int data_index_winding_b=arg_short_int[5];
	int num=arg_int[0];
	
	struct save_final_clusters_percolation_data finalclusterspercdata;

	finalclusterspercdata.data_index_parameters=data_index_parameters;
	finalclusterspercdata.data_index_number_of_clusters=data_index_number_of_clusters;
	finalclusterspercdata.data_index_number_of_stripes=data_index_number_of_stripes;
	finalclusterspercdata.data_index_winding_a=data_index_winding_a;
	finalclusterspercdata.data_index_winding_b=data_index_winding_b;
	finalclusterspercdata.size=num;
	save_final_clusters_percolation(command_insert, &finalclusterspercdata, data, data_parameters, data_num);
	
	list_insert(save, save_index, save_final_clusters_percolation, save_num);
	
	return;
}

void cmd_fs()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_steps=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_steps_data finalstepsdata;

	finalstepsdata.data_index_parameters=data_index_parameters;
	finalstepsdata.data_index_final_steps=data_index_final_steps;
	finalstepsdata.size=num;
	save_final_steps(command_insert, &finalstepsdata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_steps, save_num);
	
	return;
}

void cmd_ft()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	short int data_index_final_time=arg_short_int[2];
	int num=arg_int[0];
	
	struct save_final_time_data finaltimedata;

	finaltimedata.data_index_parameters=data_index_parameters;
	finaltimedata.data_index_final_time=data_index_final_time;
	finaltimedata.size=num;
	save_final_time(command_insert, &finaltimedata, data, data_parameters, data_num);

	list_insert(save, save_index, save_final_time, save_num);
	
	return;
}

void cmd_incT()
{
	short int save_index=arg_short_int[0];
	short int data_index_parameters=arg_short_int[1];
	double increment=arg_double[0];
	
	struct save_increment_T_data incrementTdata;

	incrementTdata.data_index_parameters=data_index_parameters;
	incrementTdata.increment=increment;
	save_increment_T(command_insert, &incrementTdata, data, data_parameters, data_num);

	list_insert(save, save_index, save_increment_T, save_num);
	
	return;
}

void cmd_svseed()
{
	short int save_index=arg_short_int[0];
	short int data_index_rng=arg_short_int[1];
	short int data_index_low=arg_short_int[2];
	short int data_index_high=arg_short_int[3];
	int num=arg_int[0];
	
	struct save_seed_data seeddata;

	seeddata.data_index_rng=data_index_rng;
	seeddata.data_index_low=data_index_low;
	seeddata.data_index_high=data_index_high;
	seeddata.size=num;
	save_seed(command_insert, &seeddata, data, data_parameters, data_num);

	list_insert(save, save_index, save_seed, save_num);
	
	return;
}

