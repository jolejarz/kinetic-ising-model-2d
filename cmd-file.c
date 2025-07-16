/**************************************************************
***************************************************************
*** 
***   cmd-file.c
***
***   Jason Olejarz
***   2011-02-15
***
***   This file contains commands for file I/O.
***
***************************************************************
**************************************************************/

void cmd_load()
{
	short int data_index=arg_short_int[0];

	unsigned long int size;
	
	void *output;
	int *output_size;

	FILE *datafile=fopen(arg_string[0], "r");
	
	fseek (datafile, 0, SEEK_END);
	size = ftell (datafile);
	rewind (datafile);
  
	if (data_output(data, data_parameters, &output, &output_size, size, sizeof(int), data_index, data_num)==1) return;

	size=fread(output, 1, size, datafile);

	fclose(datafile);
	
	*output_size=size;
	
	printf("%d bytes were read from %s.\n", size, arg_string[0]);

	return;
}

void cmd_loadtext()
{
	short int data_index=arg_short_int[0];

	double *output;
	int *output_size;

	double value;
	int number;

	FILE *datafile=fopen(arg_string[0], "r");
	
	number=0;
	while (fscanf(datafile, "%lf", &value)!=EOF) number++;

	rewind (datafile);

	if (data_output(data, data_parameters, &output, &output_size, number*sizeof(double), sizeof(int), data_index, data_num)==1) return;

	number=0;
	while (fscanf(datafile, "%lf", &value)!=EOF) *(output+(number++))=value;

	fclose(datafile);
	
	*output_size=number*sizeof(double);
	
	printf("%d numbers were read from %s.\n", number, arg_string[0]);

	return;
}

void cmd_save()
{
	short int data_index=arg_short_int[0];

	void *input;
	int *input_size;
	
	if (data_input(data, data_parameters, &input, &input_size, data_index, data_num)==1) return;

	FILE *datafile=fopen(arg_string[0], "w");
		
	fwrite(input, 1, *input_size, datafile);
	
	fclose(datafile);
	
	printf("%d bytes were written to %s.\n", *input_size, arg_string[0]);
	
	return;
}

void cmd_savetext()
{
	// All data in the specified data block is written in text format to the output file.
	
	short int data_index=arg_short_int[0];

	double *input;
	int *input_size;
	
	if (data_input(data, data_parameters, &input, &input_size, data_index, data_num)==1) return;

	FILE *datafile=fopen(arg_string[0], "w");
	
	int i;
	
	for (i=0; i<(*input_size)/sizeof(double); i++)
	{
		fprintf(datafile, "%.10lf\n", *(input+i));
	}
	
	fclose(datafile);
	
	printf("%d numbers were written to %s.\n", (*input_size)/sizeof(double), arg_string[0]);
	
	return;
}

void cmd_xy()
{
	short int data_index_x=arg_short_int[0];
	short int data_index_y=arg_short_int[1];

	if (data[data_index_x]==0)
	{
		printf("The x array is empty.\n");
		return;
	}
	if (data[data_index_y]==0)
	{
		printf("The y array is empty.\n");
		return;
	}

	double *x_data=data[data_index_x], *y_data=data[data_index_y];
	
	int *x_size=data_parameters[data_index_x], *y_size=data_parameters[data_index_y];
	if (*x_size!=*y_size)
	{
		printf("The sizes of the x and y arrays must be the same.\n");
		return;
	}
	
	int i;
	
	FILE *data=fopen(arg_string[0], "w");

	for (i=0; i<(*x_size/sizeof(double)); i++)
	{
		fprintf (data, "%.10lf %.10lf\n", *(x_data+i), *(y_data+i));
	}
	
	fclose(data);
	
	return;
}

