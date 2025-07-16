/**************************************************************
***************************************************************
*** 
***   run.c
***
***   Jason Olejarz
***   2011-01-25
***
***   This file contains the core calculations for evolving the interfaces.
***
***************************************************************
**************************************************************/

#include "includes.h"

short int rundiag (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters, short int set_arrays)
{
	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	short int *spins_initial=parameters->spins_initial;
	short int *spins=parameters->spins;
	short int *state_initial=parameters->state_initial;
	short int *state=parameters->state;
	int *map[25];
	int *index[25];
	double temp=parameters->temp;
	double bias=parameters->bias;
	double J2=parameters->J2;

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	int a, b, i, j;

	int x, y, z;

	int xminus;
	int xplus;
	int yminus;
	int yplus;

	int num_diff;

	int area=xlen*ylen;

	// If the values of `temp' and/or `bias' are changed by an external function, then the
	// probabilities of selecting spins of different types must be recomputed.
	//
	// These are set to `1' if the probabilities of selecting spins of different
	// energies need updating.
	//
	// If only `update_prob_num' is set to `1', then the probabilities are the same,
	// but the sum of probabilities array must be updated.
	//
	short int update_prob=1;
	short int update_prob_num=1;
	
	for (i=0; i<25; i++)
	{
		map[i]=parameters->map[i];
		index[i]=parameters->index[i];
	}

	// The probabilities of flipping spins with different energies are stored here.
	double prob[25];
	double prob_num[25];
	int prob_num_type[25];
	
	// The number of spins of different types is stored here.
	int number_of_spin_types;

	// The positions of the current spin and its eight neighboring spins are stored here.
	int position[9];

	// The energies of the selected spins before and after the flip are stored here.
	int type_start[9];
	int type_end[9];

	// The numbers of spins with different energies before the previous step are saved here.
	// These numbers are used to determine if the number of spin types increases or decreases.
	int active[25];
	
	int holes[25][9]={};
	int holes_count[25]={};

	if (set_arrays==1)
	{
		memcpy (spins, spins_initial, sizeof(short int)*xlen*ylen);
		memcpy (state, state_initial, sizeof(short int)*xlen*ylen);

		for (i=0; i<25; i++)
		{
			*(index[i])=0;
		}

		for (j=0; j<xlen*ylen; j++)
		{
			for (i=0; i<25; i++)
			{
				*(index[i]+1+j)=0;
				*(map[i]+j)=0;
			}
		}

		// Determine the type of each spin.
		for (y=0; y<ylen; y++)
		{
			for (x=0; x<xlen; x++)
			{
				// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
				if (x==0) xminus=xlen-1;
				else xminus=x-1;
				if (x==xlen-1) xplus=0;
				else xplus=x+1;
				if (y==0) yminus=ylen-1;
				else yminus=y-1;
				if (y==ylen-1) yplus=0;
				else yplus=y+1;

				// Save the type of this spin (between 0 and 24).
				*(state+x+y*xlen)=
					(5*(4-(*(spins+x+y*xlen)*
					(*(spins+xminus+y*xlen)+
					 *(spins+xplus+y*xlen)+
					 *(spins+x+yminus*xlen)+
					 *(spins+x+yplus*xlen)))))/2;
				
				num_diff=0;
				if (*(spins+xminus+yminus*xlen)!=*(spins+x+y*xlen)) num_diff++;
				if (*(spins+xplus+yminus*xlen)!=*(spins+x+y*xlen)) num_diff++;
				if (*(spins+xminus+yplus*xlen)!=*(spins+x+y*xlen)) num_diff++;
				if (*(spins+xplus+yplus*xlen)!=*(spins+x+y*xlen)) num_diff++;

				*(state+x+y*xlen)+=num_diff;

				// Add this spin to the correct list.
				(*index[*(state+x+y*xlen)])++;
				*(index[*(state+x+y*xlen)]+*index[*(state+x+y*xlen)])=x+y*xlen;
				*(map[*(state+x+y*xlen)]+x+y*xlen)=*index[*(state+x+y*xlen)];
			}
		}

		// Determine the energy and magnetization of the system.
		parameters->mag=0;
		parameters->energy=0;
		for (i=0; i<area; i++)
		{
			y=(int)(i/xlen);
			x=(int)(i-y*xlen);

			if (x==xlen-1) position[0]=y*xlen;
			else position[0]=x+1+y*xlen;
			if (y==ylen-1) position[1]=x;
			else position[1]=x+(y+1)*xlen;
			
			// Compute the lower-left and lower-right diagonal neighbors.
			// When the loop is finished, each diagonal bond will have been counted
			// exactly once.
			if (y==0)
			{
				if (x==0)
				{
					position[2]=(xlen-1)+(ylen-1)*xlen;
					position[3]=1+(ylen-1)*xlen;
				}
				else if (x==xlen-1)
				{
					position[2]=(xlen-2)+(ylen-1)*xlen;
					position[3]=(ylen-1)*xlen;
				}
				else
				{
					position[2]=(x-1)+(ylen-1)*xlen;
					position[3]=(x+1)+(ylen-1)*xlen;
				}
			}
			else
			{
				if (x==0)
				{
					position[2]=(xlen-1)+(y-1)*xlen;
					position[3]=1+(y-1)*xlen;
				}
				else if (x==xlen-1)
				{
					position[2]=(xlen-2)+(y-1)*xlen;
					position[3]=(y-1)*xlen;
				}
				else
				{
					position[2]=(x-1)+(y-1)*xlen;
					position[3]=(x+1)+(y-1)*xlen;
				}
			}

			parameters->energy+=bias*(*(spins+i));
			for (j=0; j<2; j++)
			{
				parameters->energy-=*(spins+i)*(*(spins+position[j]));
			}
			parameters->energy+=J2*(*(spins+i)*(*(spins+position[2])));
			parameters->energy+=J2*(*(spins+i)*(*(spins+position[3])));
			
			parameters->mag+=*(spins+i);
		}
		// This ensures that, with no bias field, the ground state energy equals zero.
		parameters->energy+=2*area-2*J2*area;
	}

	parameters->steps=0;
	parameters->time=0;
	
	parameters->coordinate=0;
	parameters->move=0;

	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	number_of_spin_types=0;
	for (i=0; i<25; i++)
	{
		active[i]=*index[i];
		if (*index[i]!=0)
		{
			number_of_spin_types++;
		}
	}

	// Set the probability arrays.
	// This is used for correctly computing the time increment on the first step.
	if (temp>0)
	{
		for (i=0; i<5; i++)
		{
			for (j=0; j<5; j++)
			{
				prob[5*i+j]=1/(double)(1+exp((4*(2-i)-4*J2*(2-j))/temp));
			}
		}
	}
	else
	{
		if (J2<=0)
		{
			for (i=0; i<5; i++)
			{
				for (j=0; j<5; j++)
				{
					if (i+j>4) prob[5*i+j]=1;
					else if (i+j<4) prob[5*i+j]=0;
					else prob[5*i+j]=0.5;
				}
			}
		}
		else
		{
			for (i=0; i<5; i++)
			{
				for (j=0; j<5; j++)
				{
					if (i-j>0) prob[5*i+j]=1;
					else if (i-j<0) prob[5*i+j]=0;
					else prob[5*i+j]=0.5;
				}
			}
		}
	}

	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/(active[0]*prob[0]+active[1]*prob[1]+active[2]*prob[2]+active[3]*prob[3]+active[4]*prob[4]+active[5]*prob[5]+active[6]*prob[6]+active[7]*prob[7]+active[8]*prob[8]+active[9]*prob[9]+active[10]*prob[10]+active[11]*prob[11]+active[12]*prob[12]+active[13]*prob[13]+active[14]*prob[14]+active[15]*prob[15]+active[16]*prob[16]+active[17]*prob[17]+active[18]*prob[18]+active[19]*prob[19]+active[20]*prob[20]+active[21]*prob[21]+active[22]*prob[22]+active[23]*prob[23]+active[24]*prob[24]));

		// Check if the probabilities of flipping spins of different types must be recomputed.
		if (update_prob==1)
		{
			if (temp>0)
			{
				for (i=0; i<5; i++)
				{
					for (j=0; j<5; j++)
					{
						prob[5*i+j]=1/(double)(1+exp((4*(2-i)-4*J2*(2-j))/temp));
					}
				}
			}
			else
			{
				if (J2<=0)
				{
					for (i=0; i<5; i++)
					{
						for (j=0; j<5; j++)
						{
							if (i+j>4) prob[5*i+j]=1;
							else if (i+j<4) prob[5*i+j]=0;
							else prob[5*i+j]=0.5;
						}
					}
				}
				else
				{
					for (i=0; i<5; i++)
					{
						for (j=0; j<5; j++)
						{
							if (i-j>0) prob[5*i+j]=1;
							else if (i-j<0) prob[5*i+j]=0;
							else prob[5*i+j]=0.5;
						}
					}
				}
			}
			update_prob=0;
		}

		// Check if the sum of probabilities array must be updated.
		if (update_prob_num==1)
		{
			j=0;
			for (i=0; i<25; i++)
			{
				if (*index[i]!=0)
				{
					if (j==0) prob_num[j]=(*index[i])*prob[i];
					else prob_num[j]=prob_num[j-1]+(*index[i])*prob[i];
					
					prob_num_type[j]=i;
					j++;
				}
								
				// Check if all spin types in the system are included in the `prob_num' array.
				if (j==number_of_spin_types) break;
			}
			
			// Rescale the `prob_num' array so that any random number between 0 and 1 points
			// to a spin type.
			for (i=0; i<j-1; i++)
			{
				prob_num[i]=prob_num[i]/prob_num[j-1];
			}
			prob_num[j-1]=1;
			
			update_prob_num=0;
		}

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Determine the energy of the spin that is selected to flip.
		// Also determine the energy of the spin after it is flipped.
		for (j=0; j<number_of_spin_types; j++)
		{
			if (prob_num[j]>random_number) break;
		}

		type_start[0]=prob_num_type[j];

		type_end[0]=5*(4-type_start[0]/5);
		if (type_start[0]%5==0) type_end[0]+=4;
		else if (type_start[0]%5==1) type_end[0]+=3;
		else if (type_start[0]%5==2) type_end[0]+=2;
		else if (type_start[0]%5==3) type_end[0]+=1;

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Choose a spin to flip.	
		j=1+(int)((*index[type_start[0]])*random_number);
		position[0]=*(index[type_start[0]]+j);

		// Get the x and y coordinates of the selected spin.
		y=(int)(position[0]/xlen);
		x=(int)(position[0]-y*xlen);

		// Flip the spin.
		*(spins+position[0])=-*(spins+position[0]);
		
		// Record the flip.
		parameters->coordinate=position[0];
		parameters->move=*(spins+position[0]);

		// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
		if (x==0) xminus=xlen-1; else xminus=x-1;
		if (x==xlen-1) xplus=0; else xplus=x+1;
		if (y==0) yminus=ylen-1; else yminus=y-1;
		if (y==ylen-1) yplus=0; else yplus=y+1;
		position[1]=xminus+y*xlen;
		position[2]=xplus+y*xlen;
		position[3]=x+yminus*xlen;
		position[4]=x+yplus*xlen;
		position[5]=xminus+yminus*xlen;
		position[6]=xplus+yminus*xlen;
		position[7]=xminus+yplus*xlen;
		position[8]=xplus+yplus*xlen;

		// Save the starting and ending energies of the four neighboring spins.
		for (i=1; i<5; i++)
		{
			type_start[i]=*(state+position[i]);
			type_end[i]=type_start[i]-5*(*(spins+position[i])*(*(spins+position[0])));
		}
		for (i=5; i<9; i++)
		{
			type_start[i]=*(state+position[i]);
			type_end[i]=type_start[i]-*(spins+position[i])*(*(spins+position[0]));
		}

		// Update the map and index arrays for the selected spin and its eight neighboring spins.
		for (i=0; i<9; i++)
		{
			*(index[type_start[i]]+*(map[type_start[i]]+position[i]))=0;
			holes[type_start[i]][holes_count[type_start[i]]++]=*(map[type_start[i]]+position[i]);
			*(map[type_start[i]]+position[i])=0;

			(*index[type_end[i]])++;
			*(index[type_end[i]]+*index[type_end[i]])=position[i];
			*(map[type_end[i]]+position[i])=*index[type_end[i]];

			*(state+position[i])=type_end[i];
		}

		// Fill in holes in the active spins arrays.		
		for (i=0; i<25; i++)
		{
			holes_count[i]--;
			while (holes_count[i]>=0)
			{
				if (*(index[i]+*index[i])!=0 || (*(index[i]+*index[i])==0 && *map[i]==*index[i]))
				{
					*(index[i]+holes[i][holes_count[i]])=*(index[i]+*index[i]);
					*(map[i]+*(index[i]+*index[i]))=holes[i][holes_count[i]];
					*(index[i]+*index[i])=0;
					holes[i][holes_count[i]]=0;
				}
				else
				{
					for (a=0; a<8; a++)
					{
						if (holes[i][a]==*index[i])
						{
							for (b=a; b<8; b++)
							{
								holes[i][b]=holes[i][b+1];
							}
							break;
						}
					}
				}
				
				(*index[i])--;
				holes_count[i]--;

				if (*index[i]==0) break;
			}
			holes_count[i]=0;
		}

		// Check if the sum of probabilities array needs updating.
		for (i=0; i<25; i++)
		{
			if (active[i]!=*index[i])
			{
				update_prob_num=1;
				
				if (active[i]!=0 && *index[i]==0) number_of_spin_types--;
				if (active[i]==0 && *index[i]!=0) number_of_spin_types++;
			}
		}
		
		for (i=0; i<25; i++)
		{
			active[i]=*index[i];
		}

		// Update the energy.
		parameters->energy+=4*(2-type_start[0]/5)-4*J2*(2-type_start[0]%5);
		
		// Update the magnetization.
		parameters->mag+=2*(*(spins+position[0]));
		
		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}
		
		// If either `temp' or `bias' was changed by an external function,
		// then the probabilities need updating.
		if (parameters->temp!=temp || parameters->bias!=bias)
		{
			update_prob=1;
			update_prob_num=1;
		}
	}
	while (evolve_continue==1 &&
		(temp>0 || ((prob[0]>0 && *index[0]>0) || (prob[1]>0 && *index[1]>0) || (prob[2]>0 && *index[2]>0) || (prob[3]>0 && *index[3]>0) || (prob[4]>0 && *index[4]>0) || (prob[5]>0 && *index[5]>0) || (prob[6]>0 && *index[6]>0) || (prob[7]>0 && *index[7]>0) || (prob[8]>0 && *index[8]>0) || (prob[9]>0 && *index[9]>0) || (prob[10]>0 && *index[10]>0) || (prob[11]>0 && *index[11]>0) || (prob[12]>0 && *index[12]>0) || (prob[13]>0 && *index[13]>0) || (prob[14]>0 && *index[14]>0) || (prob[15]>0 && *index[15]>0) || (prob[16]>0 && *index[16]>0) || (prob[17]>0 && *index[17]>0) || (prob[18]>0 && *index[18]>0) || (prob[19]>0 && *index[19]>0) || (prob[20]>0 && *index[20]>0) || (prob[21]>0 && *index[21]>0) || (prob[22]>0 && *index[22]>0) || (prob[23]>0 && *index[23]>0) || (prob[24]>0 && *index[24]>0))) &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime)));

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}

short int runfixper (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters, short int set_arrays)
{
	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	short int *spins_initial=parameters->spins_initial;
	short int *spins=parameters->spins;
	short int *state_initial=parameters->state_initial;
	short int *state=parameters->state;
	int *map[5];
	int *index[5];
	double temp=parameters->temp;
	double bias=parameters->bias;
	short int *lower_boundary=parameters->lower_boundary;
	short int *upper_boundary=parameters->upper_boundary;

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	int a, b, i, j;

	int x, y, z;

	int xminus;
	int xplus;
	int yminus;
	int yplus;

	int area=xlen*ylen;

	// If the values of `temp' and/or `bias' are changed by an external function, then the
	// probabilities of selecting spins of different types must be recomputed.
	//
	// These are set to `1' if the probabilities of selecting spins of different
	// energies need updating.
	//
	// If only `update_prob_num' is set to `1', then the probabilities are the same,
	// but the sum of probabilities array must be updated.
	//
	short int update_prob=1;
	short int update_prob_num=1;
	
	for (i=0; i<5; i++)
	{
		map[i]=parameters->map[i];
		index[i]=parameters->index[i];
	}

	// The probabilities of flipping spins with different energies are stored here.
	double prob[5];
	double prob_num[5];
	int prob_num_type[5];
	
	// The number of spins of different types is stored here.
	int number_of_spin_types;

	// The positions of the current spin and its four neighboring spins are stored here.
	int position[5];

	// The energies of the selected spins before and after the flip are stored here.
	int type_start[5];
	int type_end[5];

	// The numbers of spins with different energies before the previous step are saved here.
	// These numbers are used to determine if the number of spin types increases or decreases.
	int active[5];
	
	int holes[5][5]={};
	int holes_count[5]={};

	if (set_arrays==1)
	{
		memcpy (spins, spins_initial, sizeof(short int)*xlen*ylen);
		memcpy (state, state_initial, sizeof(short int)*xlen*ylen);

		for (i=0; i<5; i++)
		{
			*(index[i])=0;
		}

		for (j=0; j<xlen*ylen; j++)
		{
			for (i=0; i<5; i++)
			{
				*(index[i]+1+j)=0;
				*(map[i]+j)=0;
			}
		}

		// Determine the type of each spin.
		for (y=0; y<ylen; y++)
		{
			for (x=0; x<xlen; x++)
			{
				// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
				if (x==0) xminus=xlen-1;
				else xminus=x-1;
				if (x==xlen-1) xplus=0;
				else xplus=x+1;
				if (y==0) yminus=ylen-1;
				else yminus=y-1;
				if (y==ylen-1) yplus=0;
				else yplus=y+1;

				// Save the type of this spin (between 0 and 4).
				*(state+x+y*xlen)=
					(4-(*(spins+x+y*xlen)*
					(*(spins+xminus+y*xlen)+
					 *(spins+xplus+y*xlen)+
					 *(spins+x+yminus*xlen)+
					 *(spins+x+yplus*xlen))))/2;

				// Check if this spin is on the lower or upper boundaries.
				// If so, adjust the energy accordingly.
				if (y==0)
				{
					if (*(spins+x+yminus*xlen)!=*(lower_boundary+x))
					{
						if (*(lower_boundary+x)==*(spins+x+y*xlen)) (*(state+x+y*xlen))--;
						else (*(state+x+y*xlen))++;
					}
				}
				else if (y==ylen-1)
				{
					if (*(spins+x+yplus*xlen)!=*(upper_boundary+x))
					{
						if (*(upper_boundary+x)==*(spins+x+y*xlen)) (*(state+x+y*xlen))--;
						else (*(state+x+y*xlen))++;
					}
				}

				// Add this spin to the correct list.
				(*index[*(state+x+y*xlen)])++;
				*(index[*(state+x+y*xlen)]+*index[*(state+x+y*xlen)])=x+y*xlen;
				*(map[*(state+x+y*xlen)]+x+y*xlen)=*index[*(state+x+y*xlen)];
			}
		}

		// Determine the energy and magnetization of the system.
		parameters->mag=0;
		parameters->energy=0;
		for (i=0; i<area; i++)
		{
			y=(int)(i/xlen);
			x=(int)(i-y*xlen);

			if (x==xlen-1) position[0]=y*xlen;
			else position[0]=x+1+y*xlen;
			if (y==ylen-1) position[1]=x;
			else position[1]=x+(y+1)*xlen;

			parameters->energy+=bias*(*(spins+i));
			for (j=0; j<2; j++)
			{
				parameters->energy-=*(spins+i)*(*(spins+position[j]));
			}
			
			// Check if this spin is on the lower or upper boundaries.
			// If so, adjust the energy accordingly.
			if (y==0)
			{
				if (*(lower_boundary+x)==*(spins+x+y*xlen)) parameters->energy-=1;
				else parameters->energy+=1;
			}
			else if (y==ylen-1)
			{
				if (*(spins+x)!=*(upper_boundary+x))
				{
					if (*(upper_boundary+x)==*(spins+x+y*xlen)) parameters->energy-=2;
					else parameters->energy+=2;
				}
			}

			parameters->mag+=*(spins+i);
		}
		// This ensures that, with no bias field, the ground state energy equals zero.
		parameters->energy+=2*area+xlen;
	}

	parameters->steps=0;
	parameters->time=0;
	
	parameters->coordinate=0;
	parameters->move=0;

	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	number_of_spin_types=0;
	for (i=0; i<5; i++)
	{
		active[i]=*index[i];
		if (*index[i]!=0)
		{
			number_of_spin_types++;
		}
	}

	// Set the probability arrays.
	// This is used for correctly computing the time increment on the first step.
	if (temp>0)
	{
		for (i=0; i<5; i++)
		{
			prob[i]=1/(double)(1+exp((4*(2-i))/temp));
		}
	}
	else
	{
		for (i=0; i<2; i++)
		{
			prob[i]=0;
			prob[i+3]=1;
		}
		prob[2]=0.5;
	}

	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/(active[0]*prob[0]+active[1]*prob[1]+active[2]*prob[2]+active[3]*prob[3]+active[4]*prob[4]));

		// Check if the probabilities of flipping spins of different types must be recomputed.
		if (update_prob==1)
		{
			if (temp>0)
			{
				for (i=0; i<5; i++)
				{
					prob[i]=1/(double)(1+exp((4*(2-i))/temp));
				}
			}
			else
			{
				for (i=0; i<2; i++)
				{
					prob[i]=0;
					prob[i+3]=1;
				}
				prob[2]=0.5;
			}
			update_prob=0;
		}

		// Check if the sum of probabilities array must be updated.
		if (update_prob_num==1)
		{
			j=0;
			for (i=0; i<5; i++)
			{
				if (*index[i]!=0)
				{
					if (j==0) prob_num[j]=(*index[i])*prob[i];
					else prob_num[j]=prob_num[j-1]+(*index[i])*prob[i];
					
					prob_num_type[j]=i;
					j++;
				}
								
				// Check if all spin types in the system are included in the `prob_num' array.
				if (j==number_of_spin_types) break;
			}
			
			// Rescale the `prob_num' array so that any random number between 0 and 1 points
			// to a spin type.
			for (i=0; i<j-1; i++)
			{
				prob_num[i]=prob_num[i]/prob_num[j-1];
			}
			prob_num[j-1]=1;
			
			update_prob_num=0;
		}

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Determine the energy of the spin that is selected to flip.
		// Also determine the energy of the spin after it is flipped.
		for (j=0; j<number_of_spin_types; j++)
		{
			if (prob_num[j]>random_number) break;
		}

		type_start[0]=prob_num_type[j];
		type_end[0]=4-type_start[0];

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Choose a spin to flip.	
		j=1+(int)((*index[type_start[0]])*random_number);
		position[0]=*(index[type_start[0]]+j);

		// Get the x and y coordinates of the selected spin.
		y=(int)(position[0]/xlen);
		x=(int)(position[0]-y*xlen);

		// Flip the spin.
		*(spins+position[0])=-*(spins+position[0]);
		
		// Record the flip.
		parameters->coordinate=position[0];
		parameters->move=*(spins+position[0]);

		// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
		if (x==0) position[1]=xlen-1+y*xlen;
		else position[1]=x-1+y*xlen;
		if (x==xlen-1) position[2]=y*xlen;
		else position[2]=x+1+y*xlen;
		if (y==0) position[3]=x+(ylen-1)*xlen;
		else position[3]=x+(y-1)*xlen;
		if (y==ylen-1) position[4]=x;
		else position[4]=x+(y+1)*xlen;

		// Save the starting and ending energies of the four neighboring spins.
		for (i=1; i<5; i++)
		{
			type_start[i]=*(state+position[i]);
			type_end[i]=type_start[i]-*(spins+position[i])*(*(spins+position[0]));
		}

		// Update the map and index arrays for the selected spin and its four neighboring spins.
		for (i=0; i<5; i++)
		{
			// If the current spin is on the lower or upper boundaries, then there is nothing to do here.
			y=(int)(position[i]/xlen);
			x=(int)(position[i]-y*xlen);
			if ((i==3 && y==ylen-1) || (i==4 && y==0)) continue;

			*(index[type_start[i]]+*(map[type_start[i]]+position[i]))=0;
			holes[type_start[i]][holes_count[type_start[i]]++]=*(map[type_start[i]]+position[i]);
			*(map[type_start[i]]+position[i])=0;

			(*index[type_end[i]])++;
			*(index[type_end[i]]+*index[type_end[i]])=position[i];
			*(map[type_end[i]]+position[i])=*index[type_end[i]];

			*(state+position[i])=type_end[i];
		}

		// Fill in holes in the active spins arrays.		
		for (i=0; i<5; i++)
		{
			holes_count[i]--;
			while (holes_count[i]>=0)
			{
				if (*(index[i]+*index[i])!=0 || (*(index[i]+*index[i])==0 && *map[i]==*index[i]))
				{
					*(index[i]+holes[i][holes_count[i]])=*(index[i]+*index[i]);
					*(map[i]+*(index[i]+*index[i]))=holes[i][holes_count[i]];
					*(index[i]+*index[i])=0;
					holes[i][holes_count[i]]=0;
				}
				else
				{
					for (a=0; a<4; a++)
					{
						if (holes[i][a]==*index[i])
						{
							for (b=a; b<4; b++)
							{
								holes[i][b]=holes[i][b+1];
							}
							break;
						}
					}
				}
				
				(*index[i])--;
				holes_count[i]--;

				if (*index[i]==0) break;
			}
			holes_count[i]=0;
		}

		// Check if the sum of probabilities array needs updating.
		for (i=0; i<5; i++)
		{
			if (active[i]!=*index[i])
			{
				update_prob_num=1;
				
				if (active[i]!=0 && *index[i]==0) number_of_spin_types--;
				if (active[i]==0 && *index[i]!=0) number_of_spin_types++;
			}
		}
		
		for (i=0; i<5; i++)
		{
			active[i]=*index[i];
		}

		// Update the energy and magnetization.
		parameters->energy+=4*(2-type_start[0]);
		parameters->mag+=2*(*(spins+position[0]));
		
		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}
		
		// If either `temp' or `bias' was changed by an external function,
		// then the probabilities need updating.
		if (parameters->temp!=temp || parameters->bias!=bias)
		{
			update_prob=1;
			update_prob_num=1;
		}
	}
	while (evolve_continue==1 &&
		(*index[2]>0 || *index[3]>0 || *index[4]>0 || temp>0) &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime)));

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}

short int runfixper_bias (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters, short int set_arrays)
{
	// The difference between `runfixper_bias' and `runfixper' is that `runfixper_bias' keeps track
	// of the blinker spins (spin type 2) as two separate types of spin (spin types 5 and 6).
	//
	// In `runfixper_bias', spin type 2 is not used.
	//

	// spin type 5 = blinker spin with sign -1
	// spin type 6 = blinker spin with sign +1

	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	short int *spins_initial=parameters->spins_initial;
	short int *spins=parameters->spins;
	short int *state_initial=parameters->state_initial;
	short int *state=parameters->state;
	int *map[7];
	int *index[7];
	double temp=parameters->temp;
	double bias=parameters->bias;
	short int *lower_boundary=parameters->lower_boundary;
	short int *upper_boundary=parameters->upper_boundary;

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	int a, b, i, j;

	int x, y, z;

	int xminus;
	int xplus;
	int yminus;
	int yplus;

	int area=xlen*ylen;

	// If the values of `temp' and/or `bias' are changed by an external function, then the
	// probabilities of selecting spins of different types must be recomputed.
	//
	// These are set to `1' if the probabilities of selecting spins of different
	// energies need updating.
	//
	// If only `update_prob_num' is set to `1', then the probabilities are the same,
	// but the sum of probabilities array must be updated.
	//
	short int update_prob=1;
	short int update_prob_num=1;
	
	for (i=0; i<7; i++)
	{
		map[i]=parameters->map[i];
		index[i]=parameters->index[i];
	}

	// The probabilities of flipping spins with different energies are stored here.
	double prob[7];
	double prob_num[7];
	int prob_num_type[7];
	
	// The number of spins of different types is stored here.
	int number_of_spin_types;

	// The positions of the current spin and its four neighboring spins are stored here.
	int position[5];

	// The energies of the selected spins before and after the flip are stored here.
	int type_start[5];
	int type_end[5];

	// The numbers of spins with different energies before the previous step are saved here.
	// These numbers are used to determine if the number of spin types increases or decreases.
	int active[7];
	
	int holes[7][7]={};
	int holes_count[7]={};

	if (set_arrays==1)
	{
		memcpy (spins, spins_initial, sizeof(short int)*xlen*ylen);
		memcpy (state, state_initial, sizeof(short int)*xlen*ylen);

		for (i=0; i<7; i++)
		{
			*(index[i])=0;
		}

		for (j=0; j<xlen*ylen; j++)
		{
			for (i=0; i<7; i++)
			{
				*(index[i]+1+j)=0;
				*(map[i]+j)=0;
			}
		}

		// Determine the type of each spin.
		for (y=0; y<ylen; y++)
		{
			for (x=0; x<xlen; x++)
			{
				// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
				if (x==0) xminus=xlen-1;
				else xminus=x-1;
				if (x==xlen-1) xplus=0;
				else xplus=x+1;
				if (y==0) yminus=ylen-1;
				else yminus=y-1;
				if (y==ylen-1) yplus=0;
				else yplus=y+1;

				// Save the type of this spin (between 0 and 6).
				*(state+x+y*xlen)=
					(4-(*(spins+x+y*xlen)*
					(*(spins+xminus+y*xlen)+
					 *(spins+xplus+y*xlen)+
					 *(spins+x+yminus*xlen)+
					 *(spins+x+yplus*xlen))))/2;

				// Check if this spin is on the lower or upper boundaries.
				// If so, adjust the energy accordingly.
				if (y==0)
				{
					if (*(spins+x+yminus*xlen)!=*(lower_boundary+x))
					{
						if (*(lower_boundary+x)==*(spins+x+y*xlen)) (*(state+x+y*xlen))--;
						else (*(state+x+y*xlen))++;
					}
				}
				else if (y==ylen-1)
				{
					if (*(spins+x+yplus*xlen)!=*(upper_boundary+x))
					{
						if (*(upper_boundary+x)==*(spins+x+y*xlen)) (*(state+x+y*xlen))--;
						else (*(state+x+y*xlen))++;
					}
				}

				// If the type of this spin was set to `2', then change it
				// to `5' or `6'.
				if (*(state+x+y*xlen)==2)
				{
					if (*(spins+x+y*xlen)==-1) *(state+x+y*xlen)=5;
					else *(state+x+y*xlen)=6;
				}

				// Add this spin to the correct list.
				(*index[*(state+x+y*xlen)])++;
				*(index[*(state+x+y*xlen)]+*index[*(state+x+y*xlen)])=x+y*xlen;
				*(map[*(state+x+y*xlen)]+x+y*xlen)=*index[*(state+x+y*xlen)];
			}
		}

		// Determine the energy and magnetization of the system.
		parameters->mag=0;
		parameters->energy=0;
		for (i=0; i<area; i++)
		{
			y=(int)(i/xlen);
			x=(int)(i-y*xlen);

			if (x==xlen-1) position[0]=y*xlen;
			else position[0]=x+1+y*xlen;
			if (y==ylen-1) position[1]=x;
			else position[1]=x+(y+1)*xlen;

			parameters->energy+=bias*(*(spins+i));
			for (j=0; j<2; j++)
			{
				parameters->energy-=*(spins+i)*(*(spins+position[j]));
			}
			
			// Check if this spin is on the lower or upper boundaries.
			// If so, adjust the energy accordingly.
			if (y==0)
			{
				if (*(lower_boundary+x)==*(spins+x+y*xlen)) parameters->energy-=1;
				else parameters->energy+=1;
			}
			else if (y==ylen-1)
			{
				if (*(spins+x)!=*(upper_boundary+x))
				{
					if (*(upper_boundary+x)==*(spins+x+y*xlen)) parameters->energy-=2;
					else parameters->energy+=2;
				}
			}

			parameters->mag+=*(spins+i);
		}
		// This ensures that, with no bias field, the ground state energy equals zero.
		parameters->energy+=2*area+xlen;
	}

	parameters->steps=0;
	parameters->time=0;
	
	parameters->coordinate=0;
	parameters->move=0;

	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	number_of_spin_types=0;
	for (i=0; i<7; i++)
	{
		active[i]=*index[i];
		if (*index[i]!=0)
		{
			number_of_spin_types++;
		}
	}

	// Set the probability arrays.
	// This is used for correctly computing the time increment on the first step.
	prob[0]=parameters->prob[0];
	prob[1]=parameters->prob[1];
	prob[2]=parameters->prob[2];
	prob[3]=parameters->prob[3];
	prob[4]=parameters->prob[4];
	prob[5]=parameters->prob[5];
	prob[6]=parameters->prob[6];

	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/(active[0]*prob[0]+active[1]*prob[1]+active[2]*prob[2]+active[3]*prob[3]+active[4]*prob[4]+active[5]*prob[5]+active[6]*prob[6]));

		// Check if the sum of probabilities array must be updated.
		if (update_prob_num==1)
		{
			j=0;
			for (i=0; i<7; i++)
			{
				if (*index[i]!=0)
				{
					if (j==0) prob_num[j]=(*index[i])*prob[i];
					else prob_num[j]=prob_num[j-1]+(*index[i])*prob[i];
					
					prob_num_type[j]=i;
					j++;
				}
								
				// Check if all spin types in the system are included in the `prob_num' array.
				if (j==number_of_spin_types) break;
			}
			
			// Rescale the `prob_num' array so that any random number between 0 and 1 points
			// to a spin type.
			for (i=0; i<j-1; i++)
			{
				prob_num[i]=prob_num[i]/prob_num[j-1];
			}
			prob_num[j-1]=1;
			
			update_prob_num=0;
		}

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Determine the energy of the spin that is selected to flip.
		// Also determine the energy of the spin after it is flipped.
		for (j=0; j<number_of_spin_types; j++)
		{
			if (prob_num[j]>random_number) break;
		}

		type_start[0]=prob_num_type[j];
		if (type_start[0]<5) type_end[0]=4-type_start[0];
		else if (type_start[0]==5) type_end[0]=6;
		else type_end[0]=5;

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Choose a spin to flip.	
		j=1+(int)((*index[type_start[0]])*random_number);
		position[0]=*(index[type_start[0]]+j);

		// Get the x and y coordinates of the selected spin.
		y=(int)(position[0]/xlen);
		x=(int)(position[0]-y*xlen);

		// Flip the spin.
		*(spins+position[0])=-*(spins+position[0]);
		
		// Record the flip.
		parameters->coordinate=position[0];
		parameters->move=*(spins+position[0]);

		// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
		if (x==0) position[1]=xlen-1+y*xlen;
		else position[1]=x-1+y*xlen;
		if (x==xlen-1) position[2]=y*xlen;
		else position[2]=x+1+y*xlen;
		if (y==0) position[3]=x+(ylen-1)*xlen;
		else position[3]=x+(y-1)*xlen;
		if (y==ylen-1) position[4]=x;
		else position[4]=x+(y+1)*xlen;

		// Save the starting and ending energies of the four neighboring spins.
		for (i=1; i<5; i++)
		{
			type_start[i]=*(state+position[i]);
			if (type_start[i]<5)
			{
				type_end[i]=type_start[i]-*(spins+position[i])*(*(spins+position[0]));

				if (type_end[i]==2)
				{
					if (*(spins+position[i])==-1) type_end[i]=5;
					else type_end[i]=6;
				}
			}
			else
			{
				if (*(spins+position[i])==*(spins+position[0])) type_end[i]=1;
				else type_end[i]=3;
			}
		}

		// Update the map and index arrays for the selected spin and its four neighboring spins.
		for (i=0; i<5; i++)
		{
			// If the current spin is on the lower or upper boundaries, then there is nothing to do here.
			y=(int)(position[i]/xlen);
			x=(int)(position[i]-y*xlen);
			if ((i==3 && y==ylen-1) || (i==4 && y==0)) continue;

			*(index[type_start[i]]+*(map[type_start[i]]+position[i]))=0;
			holes[type_start[i]][holes_count[type_start[i]]++]=*(map[type_start[i]]+position[i]);
			*(map[type_start[i]]+position[i])=0;

			(*index[type_end[i]])++;
			*(index[type_end[i]]+*index[type_end[i]])=position[i];
			*(map[type_end[i]]+position[i])=*index[type_end[i]];

			*(state+position[i])=type_end[i];
		}

		// Fill in holes in the active spins arrays.		
		for (i=0; i<7; i++)
		{
			holes_count[i]--;
			while (holes_count[i]>=0)
			{
				if (*(index[i]+*index[i])!=0 || (*(index[i]+*index[i])==0 && *map[i]==*index[i]))
				{
					*(index[i]+holes[i][holes_count[i]])=*(index[i]+*index[i]);
					*(map[i]+*(index[i]+*index[i]))=holes[i][holes_count[i]];
					*(index[i]+*index[i])=0;
					holes[i][holes_count[i]]=0;
				}
				else
				{
					for (a=0; a<6; a++)
					{
						if (holes[i][a]==*index[i])
						{
							for (b=a; b<6; b++)
							{
								holes[i][b]=holes[i][b+1];
							}
							break;
						}
					}
				}
				
				(*index[i])--;
				holes_count[i]--;

				if (*index[i]==0) break;
			}
			holes_count[i]=0;
		}

		// Check if the sum of probabilities array needs updating.
		for (i=0; i<7; i++)
		{
			if (active[i]!=*index[i])
			{
				update_prob_num=1;
				
				if (active[i]!=0 && *index[i]==0) number_of_spin_types--;
				if (active[i]==0 && *index[i]!=0) number_of_spin_types++;
			}
		}
		
		for (i=0; i<7; i++)
		{
			active[i]=*index[i];
		}

		// Update the energy.
		if (type_start[0]==4) parameters->energy-=8;
		else if (type_start[0]==3) parameters->energy-=4;
		else if (type_start[0]==1) parameters->energy+=4;
		else if (type_start[0]==0) parameters->energy+=8;

		// Update the magnetization.
		parameters->mag+=2*(*(spins+position[0]));
		
		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}
		
		// If either `temp' or `bias' was changed by an external function,
		// then the probabilities need updating.
		if (parameters->temp!=temp || parameters->bias!=bias)
		{
			update_prob=1;
			update_prob_num=1;
		}
	}
	while (evolve_continue==1 && (step!=0 && 
		(/**index[2]>0 || */*index[3]>0 || *index[4]>0 || *index[5]>0 || *index[6]>0 || temp>0) &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime))
		|| step==0));

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}

short int runperper (short int (*step[])(), short int step_size, void *data[], void *data_parameters[], short int data_size, struct run_parameters *parameters, short int set_arrays)
{
	// Load all parameter values.
	struct rng_struct *rng_struct=parameters->rng_struct;
	int xlen=parameters->xlen;
	int ylen=parameters->ylen;
	short int *spins_initial=parameters->spins_initial;
	short int *spins=parameters->spins;
	short int *state_initial=parameters->state_initial;
	short int *state=parameters->state;
	int *map[5];
	int *index[5];
	double temp=parameters->temp;
	double bias=parameters->bias;

	short int result, step_index=0, evolve_continue=1;

	// A random number between 0 inclusive and 1 exclusive is stored here.
	double random_number;
	
	int a, b, i, j;

	int x, y, z;

	int xminus;
	int xplus;
	int yminus;
	int yplus;

	int area=xlen*ylen;

	// If the values of `temp' and/or `bias' are changed by an external function, then the
	// probabilities of selecting spins of different types must be recomputed.
	//
	// These are set to `1' if the probabilities of selecting spins of different
	// energies need updating.
	//
	// If only `update_prob_num' is set to `1', then the probabilities are the same,
	// but the sum of probabilities array must be updated.
	//
	short int update_prob=1;
	short int update_prob_num=1;
	
	for (i=0; i<5; i++)
	{
		map[i]=parameters->map[i];
		index[i]=parameters->index[i];
	}

	// The probabilities of flipping spins with different energies are stored here.
	double prob[5];
	double prob_num[5];
	int prob_num_type[5];
	
	// The number of spins of different types is stored here.
	int number_of_spin_types;

	// The positions of the current spin and its four neighboring spins are stored here.
	int position[5];

	// The energies of the selected spins before and after the flip are stored here.
	int type_start[5];
	int type_end[5];

	// The numbers of spins with different energies before the previous step are saved here.
	// These numbers are used to determine if the number of spin types increases or decreases.
	int active[5];
	
	int holes[5][5]={};
	int holes_count[5]={};

	if (set_arrays==1)
	{
		memcpy (spins, spins_initial, sizeof(short int)*xlen*ylen);
		memcpy (state, state_initial, sizeof(short int)*xlen*ylen);

		for (i=0; i<5; i++)
		{
			*(index[i])=0;
		}

		for (j=0; j<xlen*ylen; j++)
		{
			for (i=0; i<5; i++)
			{
				*(index[i]+1+j)=0;
				*(map[i]+j)=0;
			}
		}

		// Determine the type of each spin.
		for (y=0; y<ylen; y++)
		{
			for (x=0; x<xlen; x++)
			{
				// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
				if (x==0) xminus=xlen-1;
				else xminus=x-1;
				if (x==xlen-1) xplus=0;
				else xplus=x+1;
				if (y==0) yminus=ylen-1;
				else yminus=y-1;
				if (y==ylen-1) yplus=0;
				else yplus=y+1;

				// Save the type of this spin (between 0 and 6).
				*(state+x+y*xlen)=
					(4-(*(spins+x+y*xlen)*
					(*(spins+xminus+y*xlen)+
					 *(spins+xplus+y*xlen)+
					 *(spins+x+yminus*xlen)+
					 *(spins+x+yplus*xlen))))/2;

				// Add this spin to the correct list.
				(*index[*(state+x+y*xlen)])++;
				*(index[*(state+x+y*xlen)]+*index[*(state+x+y*xlen)])=x+y*xlen;
				*(map[*(state+x+y*xlen)]+x+y*xlen)=*index[*(state+x+y*xlen)];
			}
		}

		// Determine the energy and magnetization of the system.
		parameters->mag=0;
		parameters->energy=0;
		for (i=0; i<area; i++)
		{
			y=(int)(i/xlen);
			x=(int)(i-y*xlen);

			if (x==xlen-1) position[0]=y*xlen;
			else position[0]=x+1+y*xlen;
			if (y==ylen-1) position[1]=x;
			else position[1]=x+(y+1)*xlen;

			parameters->energy+=bias*(*(spins+i));
			for (j=0; j<2; j++)
			{
				parameters->energy-=*(spins+i)*(*(spins+position[j]));
			}
			
			parameters->mag+=*(spins+i);
		}
		// This ensures that, with no bias field, the energy is between 0 (ground state) and
		// 4*(number of spins) (antiferromagnetic state).
		parameters->energy+=2*area;
	}

	parameters->steps=0;
	parameters->time=0;
	
	parameters->coordinate=0;
	parameters->move=0;

	// Send 'begin' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_begin, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	// If both 'maxsteps' and 'maxtime' are zero, then there is nothing to do.
	if (parameters->maxsteps==0 && parameters->maxtime==0) return 0;
	
	number_of_spin_types=0;
	for (i=0; i<5; i++)
	{
		active[i]=*index[i];
		if (*index[i]!=0)
		{
			number_of_spin_types++;
		}
	}

	// Set the probability arrays.
	// This is used for correctly computing the time increment on the first step.
	if (temp>0)
	{
		for (i=0; i<5; i++)
		{
			prob[i]=1/(double)(1+exp((4*(2-i))/temp));
		}
	}
	else
	{
		for (i=0; i<2; i++)
		{
			prob[i]=0;
			prob[i+3]=1;
		}
		prob[2]=0.5;
	}

	do
	{
		// Increment the number of steps.
		(parameters->steps)++;

		// Update the time elapsed.  The time increment is drawn from an
		// exponential distribution.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);
		(parameters->time)+=(double)(-log(1-random_number)/(active[0]*prob[0]+active[1]*prob[1]+active[2]*prob[2]+active[3]*prob[3]+active[4]*prob[4]));

		// Check if the probabilities of flipping spins of different types must be recomputed.
		if (update_prob==1)
		{
			if (temp>0)
			{
				for (i=0; i<5; i++)
				{
					prob[i]=1/(double)(1+exp((4*(2-i))/temp));
				}
			}
			else
			{
				for (i=0; i<2; i++)
				{
					prob[i]=0;
					prob[i+3]=1;
				}
				prob[2]=0.5;
			}
			update_prob=0;
		}

		// Check if the sum of probabilities array must be updated.
		if (update_prob_num==1)
		{
			j=0;
			for (i=0; i<5; i++)
			{
				if (*index[i]!=0)
				{
					if (j==0) prob_num[j]=(*index[i])*prob[i];
					else prob_num[j]=prob_num[j-1]+(*index[i])*prob[i];
					
					prob_num_type[j]=i;
					j++;
				}
								
				// Check if all spin types in the system are included in the `prob_num' array.
				if (j==number_of_spin_types) break;
			}
			
			// Rescale the `prob_num' array so that any random number between 0 and 1 points
			// to a spin type.
			for (i=0; i<j-1; i++)
			{
				prob_num[i]=prob_num[i]/prob_num[j-1];
			}
			prob_num[j-1]=1;
			
			update_prob_num=0;
		}

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Determine the energy of the spin that is selected to flip.
		// Also determine the energy of the spin after it is flipped.
		for (j=0; j<number_of_spin_types; j++)
		{
			if (prob_num[j]>random_number) break;
		}

		type_start[0]=prob_num_type[j];
		type_end[0]=4-type_start[0];

		// Generate a random number.
		rng_struct->rng_next(rng_struct);
		random_number=(rng_struct->current_double);

		// Choose a spin to flip.	
		j=1+(int)((*index[type_start[0]])*random_number);
		position[0]=*(index[type_start[0]]+j);

		// Get the x and y coordinates of the selected spin.
		y=(int)(position[0]/xlen);
		x=(int)(position[0]-y*xlen);

		// Flip the spin.
		*(spins+position[0])=-*(spins+position[0]);
		
		// Record the flip.
		parameters->coordinate=position[0];
		parameters->move=*(spins+position[0]);

		// Determine the coordinates of the neighboring spins, accounting for the boundary conditions.
		if (x==0) position[1]=xlen-1+y*xlen;
		else position[1]=x-1+y*xlen;
		if (x==xlen-1) position[2]=y*xlen;
		else position[2]=x+1+y*xlen;
		if (y==0) position[3]=x+(ylen-1)*xlen;
		else position[3]=x+(y-1)*xlen;
		if (y==ylen-1) position[4]=x;
		else position[4]=x+(y+1)*xlen;

		// Save the starting and ending energies of the four neighboring spins.
		for (i=1; i<5; i++)
		{
			type_start[i]=*(state+position[i]);
			type_end[i]=type_start[i]-*(spins+position[i])*(*(spins+position[0]));
		}

		// Update the map and index arrays for the selected spin and its four neighboring spins.
		for (i=0; i<5; i++)
		{
			*(index[type_start[i]]+*(map[type_start[i]]+position[i]))=0;
			holes[type_start[i]][holes_count[type_start[i]]++]=*(map[type_start[i]]+position[i]);
			*(map[type_start[i]]+position[i])=0;

			(*index[type_end[i]])++;
			*(index[type_end[i]]+*index[type_end[i]])=position[i];
			*(map[type_end[i]]+position[i])=*index[type_end[i]];

			*(state+position[i])=type_end[i];
		}

		// Fill in holes in the active spins arrays.		
		for (i=0; i<5; i++)
		{
			holes_count[i]--;
			while (holes_count[i]>=0)
			{
				if (*(index[i]+*index[i])!=0 || (*(index[i]+*index[i])==0 && *map[i]==*index[i]))
				{
					*(index[i]+holes[i][holes_count[i]])=*(index[i]+*index[i]);
					*(map[i]+*(index[i]+*index[i]))=holes[i][holes_count[i]];
					*(index[i]+*index[i])=0;
					holes[i][holes_count[i]]=0;
				}
				else
				{
					for (a=0; a<4; a++)
					{
						if (holes[i][a]==*index[i])
						{
							for (b=a; b<4; b++)
							{
								holes[i][b]=holes[i][b+1];
							}
							break;
						}
					}
				}
				
				(*index[i])--;
				holes_count[i]--;

				if (*index[i]==0) break;
			}
			holes_count[i]=0;
		}

		// Check if the sum of probabilities array needs updating.
		for (i=0; i<5; i++)
		{
			if (active[i]!=*index[i])
			{
				update_prob_num=1;
				
				if (active[i]!=0 && *index[i]==0) number_of_spin_types--;
				if (active[i]==0 && *index[i]!=0) number_of_spin_types++;
			}
		}
		
		for (i=0; i<5; i++)
		{
			active[i]=*index[i];
		}

		// Update the energy and magnetization.
		parameters->energy+=4*(2-type_start[0]);
		parameters->mag+=2*(*(spins+position[0]));
		
		// Do intermediate processing.
		if (step!=0)
		{
			while (step[step_index]!=0 && step_index<step_size)
			{
				result=step[step_index++](command_run, 0, data, data_parameters, data_size, parameters);
				if (result==result_error) return result_error;
				else if (result==result_done) {evolve_continue=0; break;}
			}
			step_index=0;
		}
		
		// If either `temp' or `bias' was changed by an external function,
		// then the probabilities need updating.
		if (parameters->temp!=temp || parameters->bias!=bias)
		{
			update_prob=1;
			update_prob_num=1;
		}
	}
	while (evolve_continue==1 &&
		(*index[2]>0 || *index[3]>0 || *index[4]>0 || temp>0) &&
		((parameters->steps>0 && parameters->steps<parameters->maxsteps) ||
		(parameters->time>0 && parameters->time<parameters->maxtime)));

	// Send 'end' to all functions in 'step'.
	if (step!=0)
	{
		while (step[step_index]!=0 && step_index<step_size)
		{
			if (step[step_index++](command_end, 0, data, data_parameters, data_size, parameters)==result_error) return result_error;
		}
		step_index=0;
	}

	return 0;
}
