/**
 * @brief This file executes the predator-prey model simulation
 *
 * In this file all the necessary objects are initialized and the model simulation is performed.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "puma_hare_funcs.h"

#include "io_funcs.h"

const int OUTPUT_HEIGHT = 350;
const int N_ITER = 500;


/**
 * @brief Main function that runs the simulation
 *
 * In this function the parameters are initialized, the input file is read, the necessary arrays are created and the simulation updates are performed. 
 * @return @c integer
 */
int main(int argc, char** argv){

	clock_t start, end;
    double cpu_time_used;
	char* filename;
	char* output_folder;
	char output_name[100];
	int i, j, k, input_height, input_width, input_size, print_step = 10, population_number, input_bigger_than_output;

	if(argc==0){
		printf("no parameters provided. Aborting. Look at README.md to see parameter list.");
	}else{

		// initializes parameters
		filename = argv[1];
		float hare_birth_rate = atof(argv[2]);  
		float prey_consumption_rate = atof(argv[3]);
		float hare_difussion_rate = atof(argv[4]);
		float puma_birth_rate = atof(argv[5]);
		float puma_mortality_rate = atof(argv[6]);
		float puma_difussion_rate = atof(argv[7]);
		float timestep = atof(argv[8]);
		int print_step = atoi(argv[9]);
		output_folder = argv[10];

    	// initializes array where data will be read
		get_input_size(filename, &input_height, &input_width);

		input_height += 2; //take halos into account
		input_width += 2; //take halos into account

		int** square = malloc((input_height)*sizeof(int*));

		for(i=0;i<input_height;i++){
	        square[i]= malloc((input_width)*sizeof(int));
	    }

	    fill_halo_entries(square,input_height, input_width);

		read_input(square, input_height, input_width, filename);


		// initializes grid of Square_Population structs
		Square_Population** grid = (Square_Population**) malloc(sizeof(Square_Population*)*input_height);

		for(i=0;i<input_height;i++){
	        grid[i] = (Square_Population*) malloc((input_width)*sizeof(Square_Population));
	    }

		initialize_population_grid(grid, square, input_height, input_width);

		// free unused memory
		for(i=0;i<input_height;i++){
	        free(square[i]);
	    }
	    free(square);

		// initializes variables to write output files
		int OUTPUT_WIDTH = (int) ( (double) OUTPUT_HEIGHT * ( (double) input_width/ (double) input_height));

		double** output_template = malloc(sizeof(double*)*OUTPUT_HEIGHT);

		for(i=0;i<OUTPUT_HEIGHT;i++){
			output_template[i] = malloc(sizeof(double)*OUTPUT_WIDTH);
		}

		// take initial snapshot and run central part of the program

		start = clock();
		for(i=1;i<=N_ITER;i++){

			update_population_grid(grid, \
								   input_height, \
								   input_width, \
								   hare_birth_rate, \
								   prey_consumption_rate, \
								   hare_difussion_rate, \
								   puma_birth_rate, \
								   puma_mortality_rate, \
								   puma_difussion_rate, \
								   timestep);

			if(i%print_step==0){

				generate_output(grid,\
								output_template, \
								input_height, \
								input_width,
								OUTPUT_HEIGHT, \
								OUTPUT_WIDTH, \
								output_name, \
								i,
								output_folder);

			}

		}

		end = clock();
     	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

     	printf("%f seconds ellapsed doing the simulation.\n",cpu_time_used);
		// free unused memory
		for(i=0;i<input_height;i++)
	    {
	        free(grid[i]);
	    }
	    free(grid);

	    for(i=0;i<OUTPUT_HEIGHT;i++)
	    {
	        free(output_template[i]);
	    }
	    free(output_template);

	}
	return 0;
}

