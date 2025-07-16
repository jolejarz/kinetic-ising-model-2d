/**************************************************************
***************************************************************
*** 
***   cmd-data.c
***
***   Jason Olejarz
***   2011-01-26
***
***   This file contains commands for manipulating data.
***
***************************************************************
**************************************************************/

void cmd_bins()
{
	short int data_index=arg_short_int[0];
	int bin_size=arg_int[0];
	
	double *data_list_double;
	
	int *data_list_parameters;
	
	int data_number, i, j, num_bins;
	
	double *bin_averages;
	
	double average, stdev, sum_sq;
	
	if (data_input(data, data_parameters, &data_list_double, &data_list_parameters, data_index, data_num)==1) return;
	data_number=*data_list_parameters/sizeof(double);
	
	if (data_number%bin_size!=0) {printf ("The data set must be able to be broken into an integer number of bins.\n"); return;}
	num_bins=data_number/bin_size;
	bin_averages=malloc (sizeof(double)*data_number/bin_size);
	
	for (j=0; j<num_bins; j++)
	{
		*(bin_averages+j)=0;
		for (i=0; i<bin_size; i++)
		{
			*(bin_averages+j)+=*(data_list_double+j*bin_size+i);
		}
		*(bin_averages+j)/=(double)bin_size;
	}
	
	average=0; sum_sq=0;
	for (j=0; j<num_bins; j++)
	{
		average+=*(bin_averages+j);
		sum_sq+=pow(*(bin_averages+j),2);
	}
	average/=(double)num_bins;
	stdev=sqrt(sum_sq/(double)num_bins-pow(average,2));
	
	printf ("%lf %lf\n", average, stdev/sqrt(num_bins));
	
	free (bin_averages);
	
	return;
}

void cmd_cnt()
{
        short int data_index=arg_short_int[0];
        double min=arg_double[0];
        double max=arg_double[1];

        double *data_list_double;

        int *data_list_parameters;

        int data_number, i;

        int count=0;

        if (data_input(data, data_parameters, &data_list_double, &data_list_parameters, data_index, data_num)==1) return;
        data_number=*data_list_parameters/sizeof(double);

        for (i=0; i<data_number; i++)
        {
                if (*(data_list_double+i)>=min && *(data_list_double+i)<=max) count++;
        }

	printf("%d\n", count);

	return;
}

void cmd_dd()
{
	data_delete(data, data_parameters, arg_short_int[0], data_num);
	return;
}

void cmd_dindex()
{
	// Delete all entries from the parallel index list
	parallel_index(0, 0, 0, data_num);
	return;
}

void cmd_disp()
{
	short int data_index=arg_short_int[0];
	int min=arg_int[0];
	int max=arg_int[1];
	
	double *data_list_double;
	
	void *data_list_parameters;
	
	if (data_input(data, data_parameters, &data_list_double, &data_list_parameters, data_index, data_num)==1) return;
	
	int i;
	
	for (i=min; i<=max; i++)
	{
		printf("%.10lf\n", *(data_list_double+i));
	}
	
	return;
}

void cmd_dt()
{
	// Delete all entries from the tag list
	tag(0, 0, 0, data_num);
	return;
}

void cmd_flt()
{
        short int data_index=arg_short_int[0];
        double min=arg_double[0];
        double max=arg_double[1];

        double *input[data_num/2], *output[data_num/2];
        int *input_size[data_num/2], *output_size[data_num/2];

        struct tag_data a=tag(2, 0, 0, data_num);

        int i, j, k, output_index=0, count=0;

        for (i=0; i<data_num/2; i++)
        {
                if (a.data_input[i]!=a.data_output[i])
                {
                        if (data_input(data, data_parameters, &input[i], &input_size[i], a.data_input[i], data_num)==1) return;
                        if (data_output(data, data_parameters, &output[i], &output_size[i], *input_size[i], sizeof(int), a.data_output[i], data_num)==1) return;

                        *output_size[i]=*input_size[i];

                        if (a.data_input[i]==data_index) j=i;

                        count++;
                }
                else break;
        }

        for (i=1; i<count; i++)
        {
                if (*input_size[i]!=*input_size[i-1])
                {
                        printf("The sizes of all tagged input arrays must be the same.\n");
                        return;
                }
        }

        int data_number_input=*input_size[j]/sizeof(double);

        for (i=0; i<data_number_input; i++)
        {
                if (*(input[j]+i)>=min && *(input[j]+i)<=max)
                {
                        for (k=0; k<count; k++)
                        {
                                *(output[k]+output_index)=*(input[k]+i);
                        }
                        output_index++;
                }
        }

        int data_number_output=output_index*sizeof(double);

        for (i=0; i<count; i++)
        {
                *output_size[i]=data_number_output;
        }

        printf("%d numbers were written to each of the output data blocks.\n", output_index);

        return;
}

void cmd_fltintf()
{
        short int input_data_index=arg_short_int[0];
        short int offset_data_index=arg_short_int[1];
        short int ycoords_data_index=arg_short_int[2];
        short int output_data_index=arg_short_int[3];
        int num_interfaces=arg_int[0];

        double *input, *offset, *ycoords, *output;
        int *input_size, *offset_size, *ycoords_size, *output_size;

	if (data_input(data, data_parameters, &input, &input_size, input_data_index, data_num)==1) return;
	if (data_input(data, data_parameters, &offset, &offset_size, offset_data_index, data_num)==1) return;
	if (data_input(data, data_parameters, &ycoords, &ycoords_size, ycoords_data_index, data_num)==1) return;

	// As an estimate, allocate twice the size of the input array for the output (filtered y coordinates) array.
	if (data_output(data, data_parameters, &output, &output_size, 2*(*input_size), sizeof(int), output_data_index, data_num)==1) return;

        int i, j, output_index=0;

	for (i=0; i<(*(input_size))/sizeof(double); i++)
	{
		if (*(input+i)==num_interfaces)
		{
			for (j=0; j<num_interfaces; j++)
			{
				*(output+output_index)=*(ycoords+((int)((*(offset+i))))+j);
				output_index++;
			}
		}
	}

        *output_size=output_index*sizeof(double);

        printf("%d numbers were written to each of the output data blocks.\n", output_index);

        return;
}

void cmd_index()
{
	short int data_index=arg_short_int[0];
	int data_size=arg_int[0];

	// Add the data block to the index list for transfer in a parallel computation
	parallel_index(1, data_index, data_size, data_num);

	return;
}

void cmd_minmax()
{
        short int data_index=arg_short_int[0];

        double *input;
        int *input_size;
        int data_number;

        if (data_input(data, data_parameters, &input, &input_size, data_index, data_num)==1) return;
        data_number=*input_size/sizeof(double);

        double min;
        double max;

        int i;

        min=*input;
        max=*input;
        for (i=1; i<data_number; i++)
        {
                if (*(input+i)<min) min=*(input+i);
                if (*(input+i)>max) max=*(input+i);
        }

        printf("min=%lf, max=%lf\n", min, max);

        return;
}

void cmd_receive()
{
/*
	// If this process is the master (rank 0), then receive all data from the slaves.
	
	int size;
	int rank;
	
	int i, j;
	
	// This structure contains info on all the data blocks that are transferred from the slaves.
	struct parallel_index_data index_data=parallel_index(2, 0, 0, data_num);
	
	// This array contains pointers to the data blocks that the transferred data are put into.
	static double *all_data[data_num];
	static struct save_value_parameters *all_data_parameters[data_num];
	
	// Get the number of processes.
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// Get this process's rank.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// If not the master, or if there is only one process, then don't do any processing here.
	if (rank>0 || size==1) return;
	
	// Create the storage space for the transferred data.
	for (i=0; i<index_data.num; i++)
	{
		if (data_output(data, data_parameters, &all_data[i], &all_data_parameters[i], (size-1)*index_data.data_size[i], sizeof(struct save_value_parameters), index_data.data_index[i], data_num)==1) return;
	}
	
	for (j=0; j<index_data.num; j++)
	{
		all_data_parameters[j]->size=0;
	}

	// Get the data from each slave process.
	for (i=1; i<size; i++)
	{
		for (j=0; j<index_data.num; j++)
		{
			MPI_Recv(	data[index_data.data_index[j]]+(i-1)*index_data.data_size[j],
					index_data.data_size[j]/sizeof(double),
					MPI_DOUBLE,
					i,
					0,
					MPI_COMM_WORLD,
					MPI_STATUS_IGNORE
				);
			all_data_parameters[j]->size+=index_data.data_size[j];
		}
		printf("Received all data from process %d.\n", i);
	}
*/	
	return;
}

void cmd_sd()
{
	int i;
	
	for (i=0; i<data_num; i++)
	{
		printf("%d  ", i);
		if (data[i]!=0) printf("occupied\n"); else printf("free\n");
	}
	
	return;
}

void cmd_send()
{
/*
	// If this process is a slave (rank>0), then send its data to the master.
	
	int rank;
	
	int i;
	
	// This structure contains info on all the data blocks that are transferred from the slaves.
	struct parallel_index_data index_data=parallel_index(2, 0, 0, data_num);

	// Get this process's rank.
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// If not a slave, then don't do any processing here.
	if (rank==0) return;
	
	// Send all data to the master.
	for (i=0; i<index_data.num; i++)
	{
		MPI_Ssend(	data[index_data.data_index[i]],
				index_data.data_size[i]/sizeof(double),
				MPI_DOUBLE,
				0,
				0,
				MPI_COMM_WORLD
			);
	}
	
	// Exit this process.
	MPI_Finalize();
	exit(0);
*/	
	return;
}

void cmd_tag()
{
        short int data_index_input=arg_short_int[0];
        short int data_index_output=arg_short_int[1];
    
        // Add the data block to the tag list.
        tag(1, data_index_input, data_index_output, data_num);
    
        return;
}

