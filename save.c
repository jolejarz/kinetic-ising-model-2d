/**************************************************************
***************************************************************
*** 
***   save.c
***
***   Jason Olejarz
***   2010-07-21
***
***   This file contains the routines used by 'calc' to save data after each run of 'evolve'.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int save_final_blinkers(short int command, struct save_final_blinkers_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_blinkers_data cmd_data;

	static short int data_index_parameters;
	static short int data_index_number_of_blinker_regions;
	static short int data_index_volume_of_blinker_regions;
	
	static double *number_of_blinker_regions;
	static struct save_final_blinkers_parameters *number_of_blinker_regions_parameters;

	static double *volume_of_blinker_regions;
	static struct save_final_blinkers_parameters *volume_of_blinker_regions_parameters;

	static struct run_parameters *p;
	static int *p_size;
	
	static int i, count, nxtimesny;

	static int number_of_clusters;
	
	static int *lattice;
	static struct cluster_stats *cluster_list;
	static short int *cluster_spins;
	
	static short int *spins_backup;
	static short int *state_backup;
	static int *map_backup[7];
	static int *index_backup[7];
	static double prob_backup[7];
	static int coordinate_backup;
	static short int move_backup;
	static unsigned long int steps_backup;
	static double time_backup;
	static double energy_backup;
	static int mag_backup;

	static short int *spins_plus_field;
	static short int *spins_minus_field;

	static short int *blinker_regions;

	static short int bias;

	static short int run;

	static int blinker_regions_count;
	static int total_blinker_size;

	switch (command)
	{
		case command_info: {printf("save_final_blinkers\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_blinkers_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;

			nxtimesny=(p->xlen)*(p->ylen);

			lattice=(int*)malloc (sizeof(int)*nxtimesny);
			cluster_list=(struct cluster_stats*)malloc (sizeof(struct cluster_stats)*nxtimesny);
			cluster_spins=(short int*)malloc (sizeof(short int)*nxtimesny);
			
			// Allocate memory.
			spins_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			state_backup=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<7; i++)
			{
				map_backup[i]=(int*)malloc (sizeof(int)*(p->xlen)*(p->ylen));
				index_backup[i]=(int*)malloc (sizeof(int)*(1+(p->xlen)*(p->ylen)));
			}

			// These are used for storing the lattice after application of plus and minus bias fields.
			spins_plus_field=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));
			spins_minus_field=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			// This is used for saving the blinker regions.
			// A `0' value means that the lattice site is not part of a blinker region.
			// A `1' value means that the lattice site is part of a blinker region.
			blinker_regions=(short int*)malloc (sizeof(short int)*(p->xlen)*(p->ylen));

			return result_continue;
		}
		case command_begin:
		{
			if ((p->xlen)*(p->ylen)!=nxtimesny)
			{
				nxtimesny=(p->xlen)*(p->ylen);

				lattice=(int*)realloc (lattice, sizeof(int)*nxtimesny);
				cluster_list=(struct cluster_stats*)realloc (cluster_list, sizeof(struct cluster_stats)*nxtimesny);
				cluster_spins=(short int*)realloc (cluster_spins, sizeof(short int)*nxtimesny);
			}
			
			if (data_output(data, data_parameters, &number_of_blinker_regions, &number_of_blinker_regions_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_blinkers_parameters), cmd_data.data_index_number_of_blinker_regions, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &volume_of_blinker_regions, &volume_of_blinker_regions_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_blinkers_parameters), cmd_data.data_index_volume_of_blinker_regions, data_size)==1) return result_error;
			
			number_of_blinker_regions_parameters->index=0;
			number_of_blinker_regions_parameters->size=0;
			
			volume_of_blinker_regions_parameters->index=0;
			volume_of_blinker_regions_parameters->size=0;
			
			return result_continue;
		}
		case command_delete:
		{
			free (cluster_spins);
			free (cluster_list);
			free (lattice);
			return result_continue;
		}
		case command_run:
		{
			// Backup all simulation data.
			memcpy (spins_backup, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
			memcpy (state_backup, p->state, sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<7; i++)
			{
				memcpy (map_backup[i], p->map[i], sizeof(int)*(p->xlen)*(p->ylen));
				memcpy (index_backup[i], p->index[i], sizeof(int)*(1+(p->xlen)*(p->ylen)));
				prob_backup[i]=p->prob[i];
			}
			coordinate_backup=p->coordinate;
			move_backup=p->move;
			steps_backup=p->steps;
			time_backup=p->time;
			energy_backup=p->energy;
			mag_backup=p->mag;

			// Initialize this to `0'.
			memset (blinker_regions, 0, sizeof(short int)*(p->xlen)*(p->ylen));

			for (bias=0; bias<2; bias++)
			{
				run=0;

				switch (bias)
				{
					case 0:
					{
						if (*(p->index[5])>0)
						{
							p->prob[5]=1;
							p->prob[6]=0;
							
							run=1;
						}
						break;
					}
					case 1:
					{
						if (*(p->index[6])>0)
						{
							p->prob[5]=0;
							p->prob[6]=1;
							
							run=1;
						}
						break;
					}
				}

				// Check if there are eligible spins.
				if (run==1)
				{
					// Drive the system with a bias field.
					if (runfixper_bias(	0,
								0,
								0,
								0,
								0,
								p,
								0)
						==result_error) return result_error;
				}

				if (bias==0)
				{
					memcpy (spins_plus_field, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));
				}
				else
				{
					memcpy (spins_minus_field, p->spins, sizeof(short int)*(p->xlen)*(p->ylen));

					for (i=0; i<nxtimesny; i++)
					{
						if (*(spins_plus_field+i)!=*(spins_minus_field+i)) *(blinker_regions+i)=1;
					}
				}
			}
			
			// Restore the original lattice.
			memcpy (p->spins, spins_backup, sizeof(short int)*(p->xlen)*(p->ylen));
			memcpy (p->state, state_backup, sizeof(short int)*(p->xlen)*(p->ylen));
			for (i=0; i<7; i++)
			{
				memcpy (p->map[i], map_backup[i], sizeof(int)*(p->xlen)*(p->ylen));
				memcpy (p->index[i], index_backup[i], sizeof(int)*(1+(p->xlen)*(p->ylen)));
				p->prob[i]=prob_backup[i];
			}
			p->coordinate=coordinate_backup;
			p->move=move_backup;
			p->steps=steps_backup;
			p->time=time_backup;
			p->energy=energy_backup;
			p->mag=mag_backup;

			// Label the clusters.
			clusters (p, lattice, blinker_regions);

			// Get cluster statistics.
			cluster_stats (p, cluster_list, lattice, blinker_regions);

			// Initialize 'number_of_clusters'.
			number_of_clusters=0;
			
			// There must be at least one cluster.
			// 'number_of_clusters' gets incremented after each loop.
			blinker_regions_count=0;
			i=0;
			do
			{
				// Get the spins that are in the cluster.
				save_cluster_spins (p, cluster_spins, i, cluster_list, lattice);
				
				// Check if this is a blinker region.
				if ((cluster_list+number_of_clusters)->sign==1) blinker_regions_count++;

				// Increment 'number_of_clusters'.
				number_of_clusters++;
			}
			while (i<nxtimesny-1 && (cluster_list+(++i))->number_of_spins>0);

			// Save the number of blinker regions.
			*(number_of_blinker_regions+(number_of_blinker_regions_parameters->index)++)=(double)blinker_regions_count;
			number_of_blinker_regions_parameters->size+=sizeof(double);

			total_blinker_size=0;
			for (i=0; i<nxtimesny; i++)
			{
				if (*(blinker_regions+i)==1) total_blinker_size++;
			}

			// Save the volume of blinker regions.
			*(volume_of_blinker_regions+(volume_of_blinker_regions_parameters->index)++)=(double)total_blinker_size;
			volume_of_blinker_regions_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_clusters(short int command, struct save_final_clusters_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_clusters_data cmd_data;

	static short int data_index_parameters;
	static short int data_index_number_of_clusters;
	
	static double *cluster_count;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static int i, count, nxtimesny;

	static int number_of_clusters;
	
	static int *lattice;
	static struct cluster_stats *cluster_list;
	static short int *cluster_spins;
	
	static struct save_final_clusters_parameters *clusters_parameters;
	
	switch (command)
	{
		case command_info: {printf("save_final_clusters\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_clusters_data));

			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;

			nxtimesny=(parameters->xlen)*(parameters->ylen);

			lattice=(int*)malloc (sizeof(int)*nxtimesny);
			cluster_list=(struct cluster_stats*)malloc (sizeof(struct cluster_stats)*nxtimesny);
			cluster_spins=(short int*)malloc (sizeof(short int)*nxtimesny);
			
			return result_continue;
		}
		case command_begin:
		{
			if ((parameters->xlen)*(parameters->ylen)!=nxtimesny)
			{
				nxtimesny=(parameters->xlen)*(parameters->ylen);

				lattice=(int*)realloc (lattice, sizeof(int)*nxtimesny);
				cluster_list=(struct cluster_stats*)realloc (cluster_list, sizeof(struct cluster_stats)*nxtimesny);
				cluster_spins=(short int*)realloc (cluster_spins, sizeof(short int)*nxtimesny);
			}
			
			if (data_output(data, data_parameters, &cluster_count, &clusters_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_number_of_clusters, data_size)==1) return result_error;
			
			clusters_parameters->index=0;
			clusters_parameters->size=0;
			
			return result_continue;
		}
		case command_delete:
		{
			free (cluster_spins);
			free (cluster_list);
			free (lattice);
			return result_continue;
		}
		case command_run:
		{
			// Label the clusters.
			clusters (parameters, lattice, parameters->spins);

			// Get cluster statistics.
			cluster_stats (parameters, cluster_list, lattice, parameters->spins);

			// Initialize 'number_of_clusters'.
			number_of_clusters=0;
			
			// There must be at least one cluster.
			// 'number_of_clusters' gets incremented after each loop.
			i=0;
			do
			{
				// Get the spins that are in the cluster.
				save_cluster_spins (parameters, cluster_spins, i, cluster_list, lattice);
				
				// Increment 'number_of_clusters'.
				number_of_clusters++;
			}
			while (i<nxtimesny-1 && (cluster_list+(++i))->number_of_spins>0);

			// Save the number of clusters.
			*(cluster_count+(clusters_parameters->index)++)=(double)number_of_clusters;
			clusters_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_clusters_percolation(short int command, struct save_final_clusters_percolation_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_clusters_percolation_data cmd_data;

	static short int data_index_parameters;
	static short int data_index_number_of_clusters;
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static int i, count, nxtimesny;

	static int number_of_clusters;
	
	static int *lattice;
	static struct cluster_stats *cluster_list;
	static short int *cluster_spins;
	
	static int *displacement_x;
	static int *displacement_y;
	static int *winding_a_lattice;
	static int *winding_b_lattice;
	static int stripes_count;
	static int winding_a_max;
	static int winding_b_max;

	static double *cluster_count;
	static struct save_final_clusters_parameters *clusters_parameters;
	
	static double *number_of_stripes;
	static struct save_final_clusters_parameters *number_of_stripes_parameters;
	
	static double *winding_a;
	static struct save_final_clusters_parameters *winding_a_parameters;
	
	static double *winding_b;
	static struct save_final_clusters_parameters *winding_b_parameters;
	
	switch (command)
	{
		case command_info: {printf("save_final_clusters_percolation\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_clusters_percolation_data));

			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;

			nxtimesny=(parameters->xlen)*(parameters->ylen);

			lattice=(int*)malloc (sizeof(int)*nxtimesny);
			cluster_list=(struct cluster_stats*)malloc (sizeof(struct cluster_stats)*nxtimesny);
			cluster_spins=(short int*)malloc (sizeof(short int)*nxtimesny);
			
			displacement_x=(int*)malloc (sizeof(int)*nxtimesny);
			displacement_y=(int*)malloc (sizeof(int)*nxtimesny);
			winding_a_lattice=(int*)malloc (sizeof(int)*nxtimesny);
			winding_b_lattice=(int*)malloc (sizeof(int)*nxtimesny);

			return result_continue;
		}
		case command_begin:
		{
			if ((parameters->xlen)*(parameters->ylen)!=nxtimesny)
			{
				nxtimesny=(parameters->xlen)*(parameters->ylen);

				lattice=(int*)realloc (lattice, sizeof(int)*nxtimesny);
				cluster_list=(struct cluster_stats*)realloc (cluster_list, sizeof(struct cluster_stats)*nxtimesny);
				cluster_spins=(short int*)realloc (cluster_spins, sizeof(short int)*nxtimesny);
			}
			
			if (data_output(data, data_parameters, &cluster_count, &clusters_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_number_of_clusters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &number_of_stripes, &number_of_stripes_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_number_of_stripes, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &winding_a, &winding_a_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_winding_a, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &winding_b, &winding_b_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_clusters_parameters), cmd_data.data_index_winding_b, data_size)==1) return result_error;
			
			clusters_parameters->index=0;
			clusters_parameters->size=0;
			
			number_of_stripes_parameters->index=0;
			number_of_stripes_parameters->size=0;
			
			winding_a_parameters->index=0;
			winding_a_parameters->size=0;
			
			winding_b_parameters->index=0;
			winding_b_parameters->size=0;
			
			return result_continue;
		}
		case command_delete:
		{
			free (cluster_spins);
			free (cluster_list);
			free (lattice);
			return result_continue;
		}
		case command_run:
		{
			// Label the clusters.
			clusters_percolation (parameters, lattice, parameters->spins, displacement_x, displacement_y, winding_a_lattice, winding_b_lattice, &stripes_count, &winding_a_max, &winding_b_max);

			// Get cluster statistics.
			cluster_stats (parameters, cluster_list, lattice, parameters->spins);

			// Initialize 'number_of_clusters'.
			number_of_clusters=0;
			
			// There must be at least one cluster.
			// 'number_of_clusters' gets incremented after each loop.
			i=0;
			do
			{
				// Get the spins that are in the cluster.
				save_cluster_spins (parameters, cluster_spins, i, cluster_list, lattice);
				
				// Increment 'number_of_clusters'.
				number_of_clusters++;
			}
			while (i<nxtimesny-1 && (cluster_list+(++i))->number_of_spins>0);

			// Save the number of clusters.
			*(cluster_count+(clusters_parameters->index)++)=(double)number_of_clusters;
			clusters_parameters->size+=sizeof(double);

			// Save the number of stripes.
			*(number_of_stripes+(number_of_stripes_parameters->index)++)=(double)stripes_count;
			number_of_stripes_parameters->size+=sizeof(double);

			// Save the horizontal winding number.
			*(winding_a+(winding_a_parameters->index)++)=(double)winding_a_max;
			winding_a_parameters->size+=sizeof(double);

			// Save the vertical winding number.
			*(winding_b+(winding_b_parameters->index)++)=(double)winding_b_max;
			winding_b_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_energy(short int command, struct save_final_energy_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_energy_data cmd_data;
	
	static double *final_energy;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_energy_parameters *final_energy_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_energy\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_energy_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_energy, &final_energy_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_energy_parameters), cmd_data.data_index_final_energy, data_size)==1) return result_error;
			
			final_energy_parameters->index=0;
			final_energy_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_energy+(final_energy_parameters->index))=parameters->energy;
			
			(final_energy_parameters->index)++;
			final_energy_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_state_periodic(short int command, struct save_final_state_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_state_data cmd_data;
	
	static double *final_state;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_state_parameters *final_state_parameters;

	static int final_state_type;

	static int x, y;
	static int xlen, ylen;
	static int winding_horizontal, winding_vertical;

	static short int current_spin_value;

	switch (command)
	{
		case command_info: {printf("save_final_state_periodic\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_state_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_state, &final_state_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_state_parameters), cmd_data.data_index_final_state, data_size)==1) return result_error;
			
			final_state_parameters->index=0;
			final_state_parameters->size=0;
			
			xlen=parameters->xlen;
			ylen=parameters->ylen;

			return result_continue;
		}
		case command_run:
		{
			final_state_type=0;

			winding_horizontal=0;
			winding_vertical=0;

			if (abs(parameters->mag)<xlen*ylen)
			{
				current_spin_value=*(parameters->spins+(ylen-1)*xlen);

				for (y=0; y<ylen; y++)
				{
					if (*(parameters->spins+y*xlen)!=current_spin_value)
					{
						current_spin_value*=-1;
						winding_horizontal++;
					}
				}

				current_spin_value=*(parameters->spins+(xlen-1));

				for (x=0; x<xlen; x++)
				{
					if (*(parameters->spins+x)!=current_spin_value)
					{
						current_spin_value*=-1;
						winding_vertical++;
					}
				}

				winding_horizontal/=2;
				winding_vertical/=2;

				if (winding_horizontal>=1 && winding_vertical==0) final_state_type=1;
				else if (winding_horizontal==0 && winding_vertical>=1) final_state_type=2;
				else if (winding_horizontal>=1 && winding_vertical>=1 && winding_horizontal==winding_vertical) final_state_type=3;
				else if ((winding_horizontal>=1 && winding_vertical>=1) || winding_horizontal==winding_vertical+1 || winding_horizontal==winding_vertical-1 || winding_horizontal==-winding_vertical+1 || winding_horizontal==-winding_vertical-1) final_state_type=4;
				else final_state_type=5;
			}

			*(final_state+(final_state_parameters->index))=(double)final_state_type;
			
			(final_state_parameters->index)++;
			final_state_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_interfaces(short int command, struct save_final_interfaces_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_interfaces_data cmd_data;
	
	static double *num_interfaces;
	static double *y_coords;
	static double *y_coords_index;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_interfaces_parameters *num_interfaces_parameters;
	static struct save_final_interfaces_parameters *y_coords_parameters;
	static struct save_final_interfaces_parameters *y_coords_index_parameters;

	static short int sign_of_next_interface;

	static int counter;

	static int y;

	switch (command)
	{
		case command_info: {printf("save_final_interfaces\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_interfaces_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &num_interfaces, &num_interfaces_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_interfaces_parameters), cmd_data.data_index_num_interfaces, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &y_coords, &y_coords_parameters, cmd_data.max_interfaces*cmd_data.size*sizeof(double), sizeof(struct save_final_interfaces_parameters), cmd_data.data_index_y_coords, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &y_coords_index, &y_coords_index_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_interfaces_parameters), cmd_data.data_index_y_coords_index, data_size)==1) return result_error;
			
			num_interfaces_parameters->index=0;
			num_interfaces_parameters->size=0;
			
			y_coords_parameters->index=0;
			y_coords_parameters->size=0;
			
			y_coords_index_parameters->index=0;
			y_coords_index_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			counter=0;
			sign_of_next_interface=-(cmd_data.sign_of_bottom_boundary);

			// Set the index into the y coordinates array.
			*(y_coords_index+(y_coords_index_parameters->index))=y_coords_parameters->index;
			
			(y_coords_index_parameters->index)++;
			y_coords_index_parameters->size+=sizeof(double);

			// Check from y=0 to y=ylen-1 and count the number of interfaces.
			for (y=0; y<(parameters->ylen); y++)
			{
				if (*(parameters->spins+y*(parameters->xlen))==sign_of_next_interface)
				{
					counter++;

					*(y_coords+(y_coords_parameters->index))=y;
					
					(y_coords_parameters->index)++;
					y_coords_parameters->size+=sizeof(double);

					sign_of_next_interface*=-1;
				}
			}

			// There may be one additional interface at y=ylen.
			if (*(parameters->spins+((parameters->ylen)-1)*(parameters->xlen))==cmd_data.sign_of_bottom_boundary)
			{
				counter++;

				*(y_coords+(y_coords_parameters->index))=(parameters->ylen);
				
				(y_coords_parameters->index)++;
				y_coords_parameters->size+=sizeof(double);
			}

			*(num_interfaces+(num_interfaces_parameters->index))=counter;
			
			(num_interfaces_parameters->index)++;
			num_interfaces_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_mag(short int command, struct save_final_mag_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_mag_data cmd_data;
	
	static double *final_mag;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_mag_parameters *final_mag_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_mag\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_mag_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_mag, &final_mag_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_mag_parameters), cmd_data.data_index_final_mag, data_size)==1) return result_error;
			
			final_mag_parameters->index=0;
			final_mag_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_mag+(final_mag_parameters->index))=(double)parameters->mag;
			
			(final_mag_parameters->index)++;
			final_mag_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_mag_abs(short int command, struct save_final_mag_abs_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_mag_abs_data cmd_data;
	
	static double *final_mag_abs;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_mag_abs_parameters *final_mag_abs_parameters;

	switch (command)
	{
		case command_info: {printf("save_final_mag_abs\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_mag_abs_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_mag_abs, &final_mag_abs_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_mag_abs_parameters), cmd_data.data_index_final_mag, data_size)==1) return result_error;
			
			final_mag_abs_parameters->index=0;
			final_mag_abs_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_mag_abs+(final_mag_abs_parameters->index))=(double)abs(parameters->mag);
			
			(final_mag_abs_parameters->index)++;
			final_mag_abs_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_steps(short int command, struct save_final_steps_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_steps_data cmd_data;
	
	static double *final_steps;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_steps_parameters *final_steps_parameters;

	static int i, mag;
	
	switch (command)
	{
		case command_info: {printf("save_final_steps\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_steps_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_steps, &final_steps_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_steps_parameters), cmd_data.data_index_final_steps, data_size)==1) return result_error;
			
			final_steps_parameters->index=0;
			final_steps_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_steps+(final_steps_parameters->index))=(double)(parameters->steps);
			
			(final_steps_parameters->index)++;
			final_steps_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_final_time(short int command, struct save_final_time_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_final_time_data cmd_data;
	
	static double *final_time;

	static struct run_parameters *parameters;
	static int *parameters_size;
	
	static struct save_final_time_parameters *final_time_parameters;

	static int i, mag;
	
	switch (command)
	{
		case command_info: {printf("save_final_time\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_final_time_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &final_time, &final_time_parameters, cmd_data.size*sizeof(double), sizeof(struct save_final_time_parameters), cmd_data.data_index_final_time, data_size)==1) return result_error;
			
			final_time_parameters->index=0;
			final_time_parameters->size=0;
			
			return result_continue;
		}
		case command_run:
		{
			*(final_time+(final_time_parameters->index))=parameters->time;
			
			(final_time_parameters->index)++;
			final_time_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_increment_T(short int command, struct save_increment_T_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_increment_T_data cmd_data;
	
	static struct run_parameters *parameters;
	static int *parameters_size;
	
	switch (command)
	{
		case command_info: {printf("save_increment_T\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_increment_T_data));
			
			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &parameters, &parameters_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			return result_continue;
		}
		case command_run:
		{
			parameters->temp+=cmd_data.increment;

			return result_continue;
		}
		default: return result_continue;
	}
}

short int save_seed(short int command, struct save_seed_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct save_seed_data cmd_data;
	
	static double *seed_low;
	static double *seed_high;

	static unsigned long int num2pow32;

	static struct rng_struct *rng_struct;
	static int *rng_struct_size;
	
	static struct save_seed_parameters *seed_low_parameters;
	static struct save_seed_parameters *seed_high_parameters;

	switch (command)
	{
		case command_info: {printf("save_seed\n"); return result_continue;}
		case command_insert:
		{
			memcpy(&cmd_data, command_data, sizeof(struct save_seed_data));

			num2pow32=pow(2,16)*pow(2,16);

			return result_continue;
		}
		case command_begin:
		{
			if (data_input(data, data_parameters, &rng_struct, &rng_struct_size, cmd_data.data_index_rng, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &seed_low, &seed_low_parameters, cmd_data.size*sizeof(double), sizeof(struct save_seed_parameters), cmd_data.data_index_low, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &seed_high, &seed_high_parameters, cmd_data.size*sizeof(double), sizeof(struct save_seed_parameters), cmd_data.data_index_high, data_size)==1) return result_error;
			
			seed_low_parameters->index=0;
			seed_low_parameters->size=0;
			
			seed_high_parameters->index=0;
			seed_high_parameters->size=0;

			return result_continue;
		}
		case command_run:
		{
			*(seed_low+(seed_low_parameters->index))=(double)((rng_struct->seed)%num2pow32);
			*(seed_high+(seed_high_parameters->index))=(double)((rng_struct->seed)/num2pow32);
			
			(seed_low_parameters->index)++;
			(seed_high_parameters->index)++;
			
			seed_low_parameters->size+=sizeof(double);
			seed_high_parameters->size+=sizeof(double);

			return result_continue;
		}
		default: return result_continue;
	}
}

