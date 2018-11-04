/**
 * @brief File containing functions related to I/O
 *
 * File containing the actual functions that perform input readings and output writings.
 */

#include "puma_hare_funcs.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "io_funcs.h"

#define MAX_INPUT_SIZE 2000

void fill_halo_entries(int** square, int height, int width){

	int k;

	for(k=0;k<height;k++){
		square[k][0] = 0;
		square[k][width-1] = 0;
	}

	for(k=0;k<width;k++){
		square[0][k] = 0;
		square[height-1][k] = 0;
	}


}

void read_input(int** square, int height, int width, char* filename){
	FILE *file = fopen(filename, "r");      //open file & dont forget to change path
	int i = 0, j = 0, n = 0,k;
	if (file == NULL) {
		printf("cannot open this file:%s\n",filename);
		exit(1);

	}
	int first_line_placeholder;
	fscanf(file, "%d", &first_line_placeholder);                      //read the size of the map
    fscanf(file, "%d", &first_line_placeholder);

    for(i=1; i<height-1; i++)                        //read data
    {
        for(j=1; j<width-1; j++)
        {
            fscanf(file,"%d",&square[i][j]);
        }
    }


	fclose(file);                                 //close file
}

void get_input_size(char* filename, int* height, int* width){

	FILE *file = fopen(filename, "r");      //open file & dont forget to change path
	int height_, width_ ;
	int i = 0, j = 0,k;
	if (file == NULL) {
		printf("cannot open this file:%s\n",filename);
		exit(1);

	}
	fscanf(file, "%d", &width_);                      //read the size of the map
	fscanf(file, "%d", &height_);                      //read the size of the map

    if(height_ > 2000){
    	height_ = 2000;
    	printf("truncating grid height to 2000 (max allowed size)");
    }

    if(width_ > 2000){
    	width_ = 2000;
    	printf("truncating grid width to 2000 (max allowed size)");
    }


	*height = height_ ;
	*width = width_ ;
}


void write_grid(double** grid, int output_height, int output_width, char outfile[]){
	
	int i = 0, j = 0, n = 0;
	double max, min, step;
	FILE *file = fopen(outfile, "w");
	if (file == NULL) {
		printf("cannot open output file\n");
		exit(1);

	}

	fprintf(file, "P3\n%d %d\n255\n", output_width, output_height); //write header

	find_grid_extremes(grid,&max,&min,output_height, output_width);
	step = (max-min)/200;

	//allocate memory for color array
	double** color = malloc(sizeof(double*)*output_height);
	for(i=0;i<output_height;i++){
		color[i] = malloc(sizeof(double)*output_width);
	}

	map_to_color(grid, color, step, min, output_height, output_width);

	for(i=0;i<output_height;i++){
		for(j=0;j<output_width;j++){
			fprintf(file, "255 %.0f %.0f\n",color[i][j], color[i][j]);
		}
	}

	//liberate memory
	for(i=0;i<output_height;i++){
		free(color[i]);
	}
	free(color);

}

void generate_output(Square_Population** grid, double** output_template, int input_height, int input_width, int output_height, int output_width, char* output_name, int iter, char* output_folder){
	int population_number;

	sprintf(output_name,"%s/hare/hare%d",output_folder,iter);
	strcat(output_name,".ppm");
	population_number = 3;

	average_grid(grid,output_template, input_height, input_width, output_height, output_width, population_number);

	write_grid(output_template, output_height, output_width, output_name);


	sprintf(output_name,"%s/puma/puma%d",output_folder,iter);
	strcat(output_name,".ppm");
	population_number = 2;

	average_grid(grid,output_template, input_height, input_width, output_height, output_width, population_number);

	write_grid(output_template, output_height, output_width, output_name);
	
}

void find_grid_extremes(double** grid, double *max, double *min, int height, int width){
	int i,j;
	double aux_max = 0.0, aux_min=1.0;


	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if(grid[i][j] > aux_max) aux_max = grid[i][j];
			if(grid[i][j] < aux_min && grid[i][j] > 0) aux_min = grid[i][j];
		}
	}

	*max = aux_max;
	*min = aux_min;

}

void map_to_color(double** grid, double** color, double step, double min, int height, int width){
	
	int i,j;

	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if(grid[i][j]>0){
				color[i][j] = 200.0 - (grid[i][j] - min)/step;
			}else{
				if(grid[i][j] < 0){
					color[i][j] = 220.0;
				}else{
					color[i][j] = 255.0;
				}
			}
		}
	}

}
