/**************************************************************
***************************************************************
*** 
***   step.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains the routines used by 'evolve' for intermediate processing.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int step_energy(short int command, struct step_energy_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_energy_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *energy;
	static struct step_energy_parameters *energy_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_energy\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_energy_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &energy, &energy_parameters, sizeof(double)*((trigger_parameters->total)+1), sizeof(struct step_energy_parameters), cmd_data.data_index_energy, data_size)==1) return result_error;
			
			energy_parameters->size=0;
			energy_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			*(energy+(energy_parameters->index))=p->energy;
			
			(energy_parameters->size)+=sizeof(double);
			(energy_parameters->index)++;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(energy+(energy_parameters->index))=p->energy;
				
				(energy_parameters->size)+=sizeof(double);
				(energy_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_equilibrium_stop(short int command, struct step_equilibrium_stop_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_equilibrium_stop_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static double *temp;
	static struct step_equilibrium_stop_parameters *temp_parameters;
	static double *mag;
	static struct step_equilibrium_stop_parameters *mag_parameters;
	static double *energy;
	static struct step_equilibrium_stop_parameters *energy_parameters;
	static double *equilibrium;
	static struct step_equilibrium_stop_parameters *equilibrium_parameters;
	
	static int i, equilibrated;
	
	switch (command)
	{
		case command_info: {printf("step_equilibrium_stop\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_equilibrium_stop_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &temp, &temp_parameters, cmd_data.data_index_temp, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &mag, &mag_parameters, cmd_data.data_index_mag, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &energy, &energy_parameters, cmd_data.data_index_energy, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &equilibrium, &equilibrium_parameters, cmd_data.size*sizeof(double), sizeof(struct step_equilibrium_stop_parameters), cmd_data.data_index_equilibrium, data_size)==1) return result_error;
			
			equilibrium_parameters->index=0;
			equilibrium_parameters->size=0;

			// Set the index into the equilibrium magnetization and energy list.
			i=cmd_data.temp_index;

			return result_continue;
		}
		case command_begin:
		{
			equilibrated=0;
			return result_continue;
		}
		case command_run:
		{
			if (cmd_data.mag_energy==0 && (double)abs(p->mag)>=*(mag+i))
			{
				equilibrated=1;
				return result_done;
			}
			if (cmd_data.mag_energy==1 && p->energy<=*(energy+i))
			{
				equilibrated=1;
				return result_done;
			}

			return result_continue;
		}
		case command_end:
		{
			*(equilibrium+(equilibrium_parameters->index))=(double)equilibrated;
			
			(equilibrium_parameters->index)++;
			equilibrium_parameters->size+=sizeof(double);
			
			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_equilibrium_table(short int command, struct step_equilibrium_table_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_equilibrium_table_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static double *temp;
	static struct step_equilibrium_table_parameters *temp_parameters;
	static double *mag;
	static struct step_equilibrium_table_parameters *mag_parameters;
	static double *mag_width;
	static struct step_equilibrium_table_parameters *mag_width_parameters;
	static double *energy;
	static struct step_equilibrium_table_parameters *energy_parameters;
	static double *energy_width;
	static struct step_equilibrium_table_parameters *energy_width_parameters;
	
	static int i;
	
	static int old_mag;
	static double old_time, old_energy;
	
	static double avg_mag, avg_mag_sq, avg_energy, avg_energy_sq, delta_t;
	
	static short int skip_divide;
	
	static double start_time;

	switch (command)
	{
		case command_info: {printf("step_equilibrium_table\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_equilibrium_table_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &temp, &temp_parameters, sizeof(double)*(cmd_data.runs), sizeof(struct step_equilibrium_table_parameters), cmd_data.data_index_temp, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &mag, &mag_parameters, sizeof(double)*(cmd_data.runs), sizeof(struct step_equilibrium_table_parameters), cmd_data.data_index_mag, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &mag_width, &mag_width_parameters, sizeof(double)*(cmd_data.runs), sizeof(struct step_equilibrium_table_parameters), cmd_data.data_index_mag_width, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &energy, &energy_parameters, sizeof(double)*(cmd_data.runs), sizeof(struct step_equilibrium_table_parameters), cmd_data.data_index_energy, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &energy_width, &energy_width_parameters, sizeof(double)*(cmd_data.runs), sizeof(struct step_equilibrium_table_parameters), cmd_data.data_index_energy_width, data_size)==1) return result_error;
			
			temp_parameters->size=0;
			temp_parameters->index=0;

			mag_parameters->size=0;
			mag_parameters->index=0;

			mag_width_parameters->size=0;
			mag_width_parameters->index=0;

			energy_parameters->size=0;
			energy_parameters->index=0;

			energy_width_parameters->size=0;
			energy_width_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			old_time=p->time;
			old_mag=p->mag;
			old_energy=p->energy;
			
			avg_mag=0;
			avg_mag_sq=0;

			avg_energy=0;
			avg_energy_sq=0;
			
			skip_divide=0;
			
			return result_continue;
		}
		case command_run:
		{
			if ((p->steps>cmd_data.min_steps && p->time>cmd_data.min_time) || p->temp==0)
			{
				delta_t=p->time-old_time;
				
				avg_mag+=old_mag*delta_t;
				avg_mag_sq+=old_mag*old_mag*delta_t;

				avg_energy+=old_energy*delta_t;
				avg_energy_sq+=old_energy*old_energy*delta_t;

				// If the temperature is too low, then the equilibrium state can be
				// assumed to be the ground state (to avoid having infinities in
				// the floating-point numbers).
				//
				// If either `avg_mag' or `avg_energy' are `inf' or `NaN', then just
				// assume that the equilibrium state is the ground state.
				//
				if (avg_mag!=avg_mag || avg_energy!=avg_energy)
				{
					if (cmd_data.slab_state==0)
					{
						avg_mag=-(p->xlen)*(p->ylen);
						avg_mag_sq=0;

						avg_energy=0;
						avg_energy_sq=0;
					}
					else
					{
						avg_mag=0;				// A slab state should be prepared so that the two slabs of opposite sign have the same width.
						avg_mag_sq=0;

						avg_energy=4*(p->xlen);	// A slab state should be prepared so that the two slabs of opposite sign have the same width.
						avg_energy_sq=0;
					}
					
					skip_divide=1;
					
					return result_done;
				}
			}
			else start_time=p->time;

			old_time=p->time;
			old_mag=p->mag;
			old_energy=p->energy;
			
			return result_continue;
		}
		case command_end:
		{
			// Determine the equilibrium magnetization and energy, and save these numbers.
			if (skip_divide==0)
			{
				avg_mag/=p->time-start_time;
				avg_mag_sq/=p->time-start_time;

				avg_energy/=p->time-start_time;
				avg_energy_sq/=p->time-start_time;
			}

			*(temp+(temp_parameters->index))=p->temp;
			*(mag+(mag_parameters->index))=fabs(avg_mag);
			*(mag_width+(mag_width_parameters->index))=sqrt(avg_mag_sq-avg_mag*avg_mag);
			*(energy+(energy_parameters->index))=avg_energy;
			*(energy_width+(energy_width_parameters->index))=sqrt(avg_energy_sq-avg_energy*avg_energy);
			
			(temp_parameters->size)+=sizeof(double);
			(temp_parameters->index)++;

			(mag_parameters->size)+=sizeof(double);
			(mag_parameters->index)++;

			(mag_width_parameters->size)+=sizeof(double);
			(mag_width_parameters->index)++;

			(energy_parameters->size)+=sizeof(double);
			(energy_parameters->index)++;

			(energy_width_parameters->size)+=sizeof(double);
			(energy_width_parameters->index)++;

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_flips(short int command, struct step_flips_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_flips_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static int *flips;
	static struct step_flips_parameters *flips_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_flips\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_flips_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &flips, &flips_parameters, (int)(sizeof(int)*2*(p->maxsteps)), sizeof(struct step_flips_parameters), cmd_data.data_index_flips, data_size)==1) return result_error;

			flips_parameters->size=0;
			flips_parameters->index=0;

			return result_continue;
		}
		case command_run:
		{
			*(flips+(flips_parameters->index)++)=p->coordinate;
			*(flips+(flips_parameters->index)++)=p->move;

			flips_parameters->size+=2*sizeof(int);

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_mag(short int command, struct step_mag_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_mag_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *mag;
	static struct step_mag_parameters *mag_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_mag\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_mag_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &mag, &mag_parameters, sizeof(double)*((trigger_parameters->total)+1), sizeof(struct step_mag_parameters), cmd_data.data_index_mag, data_size)==1) return result_error;
			
			mag_parameters->size=0;
			mag_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			*(mag+(mag_parameters->index))=(double)(p->mag);
			
			(mag_parameters->size)+=sizeof(double);
			(mag_parameters->index)++;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(mag+(mag_parameters->index))=(double)(p->mag);
				
				(mag_parameters->size)+=sizeof(double);
				(mag_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_movie_1(short int command, struct step_movie_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_movie_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;

	static double last_time;
	static int i;

	static char data_filename[6];
	static char plot_filename[8];

	static FILE *data_file, *plot_file;
	
	switch (command)
	{
		case command_info: {printf("step_movie_1\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_movie_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			return result_continue;
		}
		case command_begin:
		{
			last_time=0;
			i=0;

			printf("frame = %d,  t = %lf\n",i,p->time);

			sprintf(data_filename,"d%04d",i);
			sprintf(plot_filename,"p%04d.m",i);

			data_file=fopen(data_filename, "w");
			plot_file=fopen(plot_filename, "w");

			plot_frame_movie (p->spins, p->xlen, p->ylen, p->time, i, cmd_data.spin, data_filename, data_file, plot_file);
			
			fclose(plot_file);
			fclose(data_file);

			return result_continue;
		}
		case command_run:
		{
			if (pow(10,4)/log(1+pow(10,4))*log(1+p->time)-last_time>cmd_data.interval || p->steps==p->maxsteps)
			{
				last_time=pow(10,4)/log(1+pow(10,4))*log(1+p->time);
				i++;

				printf("frame = %d,  t = %lf\n",i,p->time);

				sprintf(data_filename,"d%04d",i);
				sprintf(plot_filename,"p%04d.m",i);

				data_file=fopen(data_filename, "w");
				plot_file=fopen(plot_filename, "w");

				plot_frame_movie (p->spins, p->xlen, p->ylen, p->time, i, cmd_data.spin, data_filename, data_file, plot_file);
				
				fclose(plot_file);
				fclose(data_file);
			}

			return result_continue;
		}
		case command_end:
		{
			if (cmd_data.end_frame==1)
			{
				last_time=pow(10,4)/log(1+pow(10,4))*log(1+p->time);
				i++;

				printf("frame = %d,  t = %lf\n",i,p->time);

				sprintf(data_filename,"d%04d",i);
				sprintf(plot_filename,"p%04d.m",i);

				data_file=fopen(data_filename, "w");
				plot_file=fopen(plot_filename, "w");

				plot_frame_movie (p->spins, p->xlen, p->ylen, p->time, i, cmd_data.spin, data_filename, data_file, plot_file);
				
				fclose(plot_file);
				fclose(data_file);
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_movie_2(short int command, struct step_movie_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_movie_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;

	static double last_time;
	static int i;

	static char data_filename[6];
	static char plot_filename[8];

	static FILE *data_file, *plot_file;
	
	static double starting_time;
	static double ending_time;

	static double starting_slope;
	static double ending_slope;

	static double B;
	static double A;

	switch (command)
	{
		case command_info: {printf("step_movie_2\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_movie_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			return result_continue;
		}
		case command_begin:
		{
			starting_time=cmd_data.starting_time;
			ending_time=cmd_data.ending_time;

			starting_slope=pow(10,4)/log(1+pow(10,4))/(1+starting_time);
			ending_slope=1;

			B=log(starting_slope/ending_slope)/(starting_time-ending_time);
			A=ending_slope/B/exp(B*ending_time);

			last_time=A*exp(B*starting_time);
			i=cmd_data.last_frame;

//			printf("frame = %d,  t = %lf\n",i,p->time);

//			sprintf(data_filename,"d%03d",i);
//			sprintf(plot_filename,"p%03d.m",i);

//			data_file=fopen(data_filename, "w");
//			plot_file=fopen(plot_filename, "w");

//			plot_frame_movie (p->spins, p->xlen, p->ylen, p->time, i, data_filename, data_file, plot_file);
			
//			fclose(plot_file);
//			fclose(data_file);

			return result_continue;
		}
		case command_run:
		{
			if (p->time>=ending_time) return result_done;

			if (p->time>starting_time+1 && (A*exp(B*(p->time))-last_time>cmd_data.interval || p->steps==p->maxsteps))
			{
				last_time=A*exp(B*(p->time));
				i++;

				printf("frame = %d,  t = %lf\n",i,p->time);

				sprintf(data_filename,"d%04d",i);
				sprintf(plot_filename,"p%04d.m",i);

				data_file=fopen(data_filename, "w");
				plot_file=fopen(plot_filename, "w");

				plot_frame_movie (p->spins, p->xlen, p->ylen, p->time, i, cmd_data.spin, data_filename, data_file, plot_file);
				
				fclose(plot_file);
				fclose(data_file);
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_movie_3(short int command, struct step_movie_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_movie_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;

	static double last_time;
	static int i;

	static char data_filename[6];
	static char plot_filename[8];

	static FILE *data_file, *plot_file;
	
	static double starting_time;
	static double ending_time;

	static double starting_slope;
	static double ending_slope;

	static double B;
	static double A;

	switch (command)
	{
		case command_info: {printf("step_movie_3\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_movie_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			
			return result_continue;
		}
		case command_begin:
		{
			starting_time=cmd_data.starting_time;
			ending_time=cmd_data.ending_time;

			last_time=starting_time;
			i=cmd_data.last_frame;

//			printf("frame = %d,  t = %lf\n",i,p->time);

//			sprintf(data_filename,"d%03d",i);
//			sprintf(plot_filename,"p%03d.m",i);

//			data_file=fopen(data_filename, "w");
//			plot_file=fopen(plot_filename, "w");

//			plot_frame_movie (p->spins, p->xlen, p->ylen, p->time, i, data_filename, data_file, plot_file);
			
//			fclose(plot_file);
//			fclose(data_file);

			return result_continue;
		}
		case command_run:
		{
			if (p->time>=ending_time) return result_done;

			if (p->time>starting_time+1 && (p->time-last_time>cmd_data.interval || p->steps==p->maxsteps))
			{
				last_time=p->time;
				i++;

				printf("frame = %d,  t = %lf\n",i,p->time);

				sprintf(data_filename,"d%04d",i);
				sprintf(plot_filename,"p%04d.m",i);

				data_file=fopen(data_filename, "w");
				plot_file=fopen(plot_filename, "w");

				plot_frame_movie (p->spins, p->xlen, p->ylen, p->time, i, cmd_data.spin, data_filename, data_file, plot_file);
				
				fclose(plot_file);
				fclose(data_file);
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_slab_stop(short int command, struct step_slab_stop_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_slab_stop_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static double *temp;
	static struct step_slab_stop_parameters *temp_parameters;
	static double *mag;
	static struct step_slab_stop_parameters *mag_parameters;
	static double *energy;
	static struct step_slab_stop_parameters *energy_parameters;
	static double *slab;
	static struct step_slab_stop_parameters *slab_parameters;
	
	static int nxtimesny;

	static int i, j, k, x, y, slab_state;
	
	static int *lattice;
	static struct cluster_stats *cluster_list;
	static short int *cluster_spins;

	static short int *x_occupied, *y_occupied;

	static int no_percolation_in_x, no_percolation_in_y;

	static short int already_checked, slab_state_energy_reached;

	static double ref_time;

	switch (command)
	{
		case command_info: {printf("step_slab_stop\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_slab_stop_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &temp, &temp_parameters, cmd_data.data_index_temp, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &mag, &mag_parameters, cmd_data.data_index_mag, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &energy, &energy_parameters, cmd_data.data_index_energy, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &slab, &slab_parameters, cmd_data.size*sizeof(double), sizeof(struct step_slab_stop_parameters), cmd_data.data_index_slab, data_size)==1) return result_error;
			
			slab_parameters->index=0;
			slab_parameters->size=0;

			// Set the index into the slab magnetization and energy list.
			i=cmd_data.temp_index;

			nxtimesny=(p->xlen)*(p->ylen);

			lattice=(int*)malloc (sizeof(int)*nxtimesny);
			cluster_list=(struct cluster_stats*)malloc (sizeof(struct cluster_stats)*nxtimesny);
			cluster_spins=(short int*)malloc (sizeof(short int)*nxtimesny);

			x_occupied=(short int*)malloc (sizeof(short int)*(p->xlen));
			y_occupied=(short int*)malloc (sizeof(short int)*(p->ylen));
			
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
			
			slab_state=0;
			
			already_checked=0;
			slab_state_energy_reached=0;

			ref_time=0;

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
			if (already_checked==0 || cmd_data.terminate==2)
			{
				if (slab_state_energy_reached==1 && p->time-ref_time>cmd_data.extra_time)
				{
					no_percolation_in_x=0;
					no_percolation_in_y=0;

					// Label the clusters.
					clusters (p, lattice, p->spins);

					// Get cluster statistics.
					cluster_stats (p, cluster_list, lattice, p->spins);

					// If there is a single direction in space (x, y, or z) in which
					// no cluster in the system percolates, then the system is in a
					// slab state.

					// Loop through the cluster list.
					k=0;
					do
					{
						// Get the spins that are in the cluster.
						save_cluster_spins (p, cluster_spins, k, cluster_list, lattice);
						
						// Initialize these arrays to zero.
						for (j=0; j<(p->xlen); j++)
						{
							*(x_occupied+j)=0;
						}
						for (j=0; j<(p->ylen); j++)
						{
							*(y_occupied+j)=0;
						}
						
						// Determine which locations in the x, y, and z directions
						// are occupied by a spin of this cluster.
						for (j=0; j<nxtimesny; j++)
						{
							y=(int)(j/(p->xlen));
							x=(int)(j-y*(p->xlen));
							
							if (*(cluster_spins+j)==1)
							{
								if (*(x_occupied+x)==0) *(x_occupied+x)=1;
								if (*(y_occupied+y)==0) *(y_occupied+y)=1;
							}
						}
						
						// Check if this cluster does not percolate in any of
						// the (x, y, z) directions.
						for (j=0; j<(p->xlen); j++)
						{
							if (*(x_occupied+j)==0) {no_percolation_in_x++; break;}
						}
						for (j=0; j<(p->ylen); j++)
						{
							if (*(y_occupied+j)==0) {no_percolation_in_y++; break;}
						}
					}
					while (k<nxtimesny-1 && (cluster_list+(++k))->number_of_spins>0);

					if (no_percolation_in_x==k || no_percolation_in_y==k)
					{
						slab_state=1;
						return result_done;
					}

					if (cmd_data.terminate==1) return result_done;

					ref_time=p->time;

					already_checked=1;
				}
				else if (slab_state_energy_reached==0 && p->energy<=*(energy+i))
				{
					slab_state_energy_reached=1;
					ref_time=p->time;
				}
			}

			return result_continue;
		}
		case command_end:
		{
			*(slab+(slab_parameters->index))=(double)slab_state;
			
			(slab_parameters->index)++;
			slab_parameters->size+=sizeof(double);
			
			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_stop_diagonal(short int command, struct step_stop_diagonal_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_stop_diagonal_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;

	static int x, y;
	static int xlen, ylen;
	static int *winding_horizontal, *winding_vertical;

	static short int current_spin_value;

	static short int all_winding_horizontal_equal, all_winding_vertical_equal;
	
	switch (command)
	{
		case command_info: {printf("step_stop_diagonal\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_stop_diagonal_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			
			return result_continue;
		}
		case command_begin:
		{
			winding_horizontal=(int*)malloc(sizeof(int)*(p->xlen));
			winding_vertical=(int*)malloc(sizeof(int)*(p->ylen));

			return result_continue;
		}
		case command_run:
		{
			if ((p->time)>=cmd_data.starting_time && *trigger==1)
			{
				for (x=0; x<xlen; x++)
				{
					current_spin_value=*(p->spins+x+(ylen-1)*xlen);

					*(winding_horizontal+x)=0;
					
					for (y=0; y<ylen; y++)
					{
						if (*(p->spins+x+y*xlen)!=current_spin_value)
						{
							current_spin_value*=-1;
							(*(winding_horizontal+x))++;
						}
					}
				}

				for (y=0; y<ylen; y++)
				{
					current_spin_value=*(p->spins+(xlen-1)+y*xlen);

					*(winding_vertical+y)=0;
					
					for (x=0; x<xlen; x++)
					{
						if (*(p->spins+x+y*xlen)!=current_spin_value)
						{
							current_spin_value*=-1;
							(*(winding_vertical+y))++;
						}
					}
				}

				all_winding_horizontal_equal=1;
				all_winding_vertical_equal=1;

				for (x=1; x<xlen; x++)
				{
					if (*(winding_horizontal+x)!=*winding_horizontal) all_winding_horizontal_equal=0;
				}

				for (y=1; y<ylen; y++)
				{
					if (*(winding_vertical+y)!=*winding_vertical) all_winding_vertical_equal=0;
				}

				if (all_winding_horizontal_equal==1 && all_winding_vertical_equal==1) return result_done;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_switch_stop(short int command, struct step_switch_stop_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_switch_stop_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static double *reverse;
	static struct step_switch_stop_parameters *reverse_parameters;
	
	static int nxtimesny;

	static short int switched;
	
	switch (command)
	{
		case command_info: {printf("step_switch_stop\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_switch_stop_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &reverse, &reverse_parameters, cmd_data.size*sizeof(double), sizeof(struct step_switch_stop_parameters), cmd_data.data_index_switch, data_size)==1) return result_error;
			
			reverse_parameters->index=0;
			reverse_parameters->size=0;

			nxtimesny=(p->xlen)*(p->ylen);

			return result_continue;
		}
		case command_begin:
		{
			switched=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (cmd_data.sign==1 && (p->mag)/(double)nxtimesny>=cmd_data.fraction)
			{
				switched=1;
				return result_done;
			}
			else if (cmd_data.sign==-1 && (p->mag)/(double)nxtimesny<=cmd_data.fraction)
			{
				switched=1;
				return result_done;
			}
			
			return result_continue;
		}
		case command_end:
		{
			*(reverse+(reverse_parameters->index))=(double)switched;
			
			(reverse_parameters->index)++;
			reverse_parameters->size+=sizeof(double);
			
			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_time(short int command, struct step_time_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_time_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	static double *time;
	static struct step_time_parameters *time_parameters;
	
	switch (command)
	{
		case command_info: {printf("step_time\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_time_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_input(data, data_parameters, &trigger, &trigger_parameters, cmd_data.data_index_trigger, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &time, &time_parameters, sizeof(double)*((trigger_parameters->total)+1), sizeof(struct step_time_parameters), cmd_data.data_index_time, data_size)==1) return result_error;
			
			time_parameters->size=0;
			time_parameters->index=0;

			return result_continue;
		}
		case command_begin:
		{
			*(time+(time_parameters->index))=p->time;
			
			(time_parameters->size)+=sizeof(double);
			(time_parameters->index)++;

			return result_continue;
		}
		case command_run:
		{
			if (*trigger==1)
			{
				*(time+(time_parameters->index))=p->time;
				
				(time_parameters->size)+=sizeof(double);
				(time_parameters->index)++;
			}

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_trigger_steps(short int command, struct step_trigger_steps_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_trigger_steps_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	
	switch (command)
	{
		case command_begin:
		{
			if (cmd_data.progress==1)
			{
				printf("Processing...      ");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_end:
		{
			if (cmd_data.progress==1)
			{
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bDone                    \n");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_info: {printf("step_trigger_steps\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_trigger_steps_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &trigger, &trigger_parameters, sizeof(short int), sizeof(struct step_trigger_parameters), cmd_data.data_index_trigger, data_size)==1) return result_error;
			
			trigger_parameters->total=(p->maxsteps)/cmd_data.steps_interval;
			trigger_parameters->current=0;
			trigger_parameters->steps=0;
			
			return result_continue;
		}
		case command_run:
		{
			if ((p->steps)%cmd_data.steps_interval==0)
			{
				*trigger=1;
				(trigger_parameters->current)++;
				trigger_parameters->steps=p->steps;

				if (cmd_data.progress==1)
				{
					printf("\b\b\b\b\b");
					printf("%4.1lf%%", 100*(trigger_parameters->current)/(double)(trigger_parameters->total));
					fflush(stdout);
				}
			}
			else *trigger=0;

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_trigger_time(short int command, struct step_trigger_time_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_trigger_time_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	
	static double time_elapsed;
	
	switch (command)
	{
		case command_begin:
		{
			if (cmd_data.progress==1)
			{
				printf("Processing...      ");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_end:
		{
			if (cmd_data.progress==1)
			{
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bDone                    \n");
				fflush(stdout);
			}
			return result_continue;
		}
		case command_info: {printf("step_trigger_time\n"); return result_continue;}

		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_trigger_time_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &trigger, &trigger_parameters, sizeof(short int), sizeof(struct step_trigger_parameters), cmd_data.data_index_trigger, data_size)==1) return result_error;
			
			trigger_parameters->total=(int)((p->maxtime)/cmd_data.time_interval);
			trigger_parameters->current=0;
			trigger_parameters->time=0;
			
			time_elapsed=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (p->time-time_elapsed>cmd_data.time_interval)
			{
				*trigger=1;
				(trigger_parameters->current)++;
				time_elapsed+=cmd_data.time_interval;
				trigger_parameters->time=time_elapsed;

				if (cmd_data.progress==1)
				{
					printf("\b\b\b\b\b");
					printf("%4.1lf%%", 100*(trigger_parameters->current)/(double)(trigger_parameters->total));
					fflush(stdout);
				}
			}
			else *trigger=0;

			return result_continue;
		}
		default: return result_continue;
	}
}

short int step_trigger_time_once(short int command, struct step_trigger_time_once_data *command_data, void *data[], void *data_parameters[], short int data_size)
{
	static struct step_trigger_time_once_data cmd_data;
	
	static struct run_parameters *p;
	static int *p_size;
	static short int *trigger;
	static struct step_trigger_parameters *trigger_parameters;
	
	static short int already_triggered;

	switch (command)
	{
		case command_info: {printf("step_trigger_time_once\n"); return result_continue;}
		case command_insert:
		{
			memcpy (&cmd_data, command_data, sizeof(struct step_trigger_time_once_data));

			if (data_input(data, data_parameters, &p, &p_size, cmd_data.data_index_parameters, data_size)==1) return result_error;
			if (data_output(data, data_parameters, &trigger, &trigger_parameters, sizeof(short int), sizeof(struct step_trigger_parameters), cmd_data.data_index_trigger, data_size)==1) return result_error;
			
			trigger_parameters->total=cmd_data.runs;
			trigger_parameters->current=0;
			
			return result_continue;
		}
		case command_begin:
		{
			already_triggered=0;
			
			return result_continue;
		}
		case command_run:
		{
			if (p->time>=cmd_data.time && already_triggered==0)
			{
				*trigger=1;
				(trigger_parameters->current)++;

				already_triggered=1;
			}
			else *trigger=0;

			return result_continue;
		}
		default: return result_continue;
	}
}

