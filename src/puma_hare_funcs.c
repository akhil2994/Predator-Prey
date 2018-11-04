/**
 *
 * File containing the actual functions that perform the simulations and grid updates at every step.
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "puma_hare_funcs.h"

double update_hare_population(double H_i_j, double adjacent_hares, double P_ij_old, float hare_birth_rate, float prey_consumption_rate, float hare_difussion_rate, int adjacent_land_squares, float delta_t){


	double H_new;
	double adjacent_tiles_info, tempt_dif;

	adjacent_tiles_info = adjacent_hares - (H_i_j * adjacent_land_squares);

	tempt_dif = hare_difussion_rate*(adjacent_tiles_info);

	H_new = H_i_j + delta_t *(hare_birth_rate * H_i_j - prey_consumption_rate * H_i_j * P_ij_old + tempt_dif);

	H_new = (H_new<0) ? 0 : H_new; // ensure that population remains non-negative

	return H_new;
}


double update_puma_population(double P_i_j, double adjacent_pumas, double H_i_j, float puma_birth_rate, float puma_mortality_rate, float puma_difussion_rate, int adjacent_land_squares, float delta_t){

	double P_new;
	double  adjacent_tiles_info, tempt_dif;

	adjacent_tiles_info = adjacent_pumas - adjacent_land_squares * P_i_j;

	tempt_dif = puma_difussion_rate * (adjacent_tiles_info);

	P_new = P_i_j + delta_t *(puma_birth_rate *H_i_j*P_i_j - puma_mortality_rate * P_i_j + tempt_dif);

	P_new = (P_new<0) ? 0 : P_new; // ensure that population remains non-negative

	return P_new;

}


void initialize_population_grid(Square_Population** grid, int** land, int height, int width){

	int puma_deployed = 0;
	int i,j;
	long aa,cc,mm, rand;
	double u;
	double pumas=0.0, hares=0.0;

	aa=1103515245; // this is just to generate pseudo-random numbers
	cc=12345;  // this is just to generate pseudo-random numbers
	mm=4294967296;  // this is just to generate pseudo-random numbers

	rand = 0;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{

			grid[i][j].is_land = land[i][j];

			if(land[i][j]==1){
				if(puma_deployed==0){
					grid[i][j].puma_old = 1.0;
					puma_deployed += 1;
					pumas += grid[i][j].puma_old;
				}
				rand = (aa * rand + cc) % mm;
				u = (double) rand / mm;

				grid[i][j].hare_old = 5*u; //there are hares in every square
				hares += 5*u;

				grid[i][j].puma_new = grid[i][j].puma_old;
				grid[i][j].hare_new = grid[i][j].hare_old;

			}else{

				grid[i][j].puma_old = 0;
				grid[i][j].hare_old = 0;
				grid[i][j].puma_new = 0;
				grid[i][j].hare_new = 0;
			}


		}
	}
	printf("total population: pumas: %f, hares: %f\n",pumas,hares);
}

void update_population_grid(Square_Population** grid, int input_height, int input_width, float hare_birth_rate, float prey_consumption_rate, float hare_difussion_rate, float puma_birth_rate, float puma_mortality_rate, float puma_difussion_rate, float t){

	int i,j, adjacent_land_squares;
	double adjacent_hares, adjacent_pumas;
	double placeholder;

	for (i = 1; i < input_height-1; i++)
	{
		for (j = 1; j < input_width-1; j++)
		{
			if(grid[i][j].is_land!=0){

                adjacent_land_squares = grid[i-1][j].is_land + grid[i+1][j].is_land + grid[i][j-1].is_land + grid[i][j+1].is_land;

                adjacent_hares = grid[i-1][j].hare_old + grid[i+1][j].hare_old + grid[i][j-1].hare_old + grid[i][j+1].hare_old;

                adjacent_pumas = grid[i-1][j].puma_old + grid[i+1][j].puma_old + grid[i][j-1].puma_old + grid[i][j+1].puma_old;


                grid[i][j].hare_new = update_hare_population(grid[i][j].hare_old, \
                											 adjacent_hares, \
                											 grid[i][j].puma_old, \
                											 hare_birth_rate, \
                											 prey_consumption_rate, \
                											 hare_difussion_rate, \
                											 adjacent_land_squares, \
                											 t);



                grid[i][j].puma_new = update_puma_population(grid[i][j].puma_old, \
                											 adjacent_pumas, \
                											 grid[i][j].hare_old, \
                											 puma_birth_rate, \
                											 puma_mortality_rate, \
                											 puma_difussion_rate, \
                											 adjacent_land_squares, \
                											 t);



        	}
		}
	}

	for(i=1;i < input_height-1;i++){
		for(j=1;j<input_width-1;j++){
			grid[i][j].puma_old = grid[i][j].puma_new;
			grid[i][j].hare_old = grid[i][j].hare_new;

		}
	}
}


void print_grid(Square_Population **grid, int height, int width, int idx){

	int i,j;

	if(idx == 1){ // idx = 1 ==> print terrain

		for (i = 0; i < height; i++){

			for (j = 0; j < width; j++){

				printf("%d ", grid[i][j].is_land);
			}

			printf("\n");

		}

	}else{

		if(idx == 2){ // idx = 2 ==> print puma population

			for (i = 0; i < height; i++){

				for (j = 0; j < width; j++){

					printf("%f ", grid[i][j].puma_old);

				}

				printf("\n");

			}

		}else{ //idx >3 ==> print hare population

			for (i = 0; i < height; i++){

				for (j = 0; j < width; j++){

						printf("%f ", grid[i][j].hare_old);

					}

					printf("\n");
			}
		}
	}
}

void average_grid(Square_Population** grid, double** output_template, int input_height, int input_width, int output_height, int output_width, int population){

	double prop,x,tile_population,sum;
	int i,j, k, new_pixel;

	int horizontal_count[output_width];


	// intiialize horizontal count to zero
	for(i=0;i<output_width;i++){
		horizontal_count[i] = 0;
	}

	//if input size > output size , zoom out
	if(input_height - 2 > output_height){

		// initialize output_template to zero
		for(i=0;i<output_height;i++){
			for(j=0;j<output_width;j++){

				output_template[i][j] = 0;

			}
		}

		int last_pixel = 0;
		for(i=0;i<input_height-2;i++){

			new_pixel = i*(output_height)/(input_height-2);


			for(j=0;j<input_width-2;j++){

				if(population==3){
					tile_population = grid[i+1][j+1].hare_new;
				}else{
					tile_population = grid[i+1][j+1].puma_new;
				}

				output_template[new_pixel][j*(output_width)/(input_width-2)] += tile_population;
				horizontal_count[j*(output_width)/(input_width-2)] += 1;
			}

			// chech if new i changes the pixel in the output file
			if(new_pixel > last_pixel || i == input_height-3){

				//if it does, before going on, average values that map to the same previous pixel
				for(k=0;k<output_width;k++){
					output_template[last_pixel][k] = output_template[last_pixel][k] / horizontal_count[k];
				}

				// re initialize horizontal count
				for(k=0;k<output_width;k++){
					horizontal_count[k] = 0;
				}

				//update boundary mapping values
				last_pixel = new_pixel;
			}
		}

	// if output size > input size, zoom in
	}else{

		for(i=0;i<output_height;i++){
			for(j=0;j<output_width;j++){

				if(population==3){
					tile_population = grid[1 + i*(input_height-2)/output_height][1 + j*(input_width-2)/output_width].hare_new;
				}else{
					tile_population = grid[1 + i*(input_height-2)/output_height][1 + j*(input_width-2)/output_width].puma_new;
				}

				output_template[i][j] = tile_population;

			}
		}

	}



}
