/**
 * @brief File containing prototypes of all I/O-related functions
 *
 * File containing prototypes of all I/O-related functions
 */


#ifndef IO_FUNCS_H_INCLUDED
#define IO_FUNCS_H_INCLUDED

/**
 * @brief Reads the first line of a terrain file and gets its grid size
 *
 * This function scans the first line of a terrain file and return its grid size
 * @param filename Character array that contains the input file name
 * @param height input height
 * @param width input width
 * @return @c void
 */
void get_input_size(char* filename, int* height, int* width);


/**
 * @brief Reads terrain files
 *
 * This function reads a space-separated terrain file (which has the grid dimensions in the first line).
 * The grid should contain only 0s and 1s (1 means land, 0 means water). The function reads the file and
 * store the values in a 2D array
 * @param square 2D integer array of the same size as the grid
 * @param height input height
 * @param width inout width
 * @param filename Character array that contains the input file name
 * @return @c void
 * @note to read just the grid size from the file, @see get_input_size.
 */
void read_input(int** square, int height, int width, char* filename);


/**
 * @brief Outputs a 2D array to a plain PPM file
 *
 * This function outputs a 2D array of doubles to a plain PPM file, in diferent tones of red, from white to dark red, depending on the value of a particular entry in the grid to the maximum and minimum values of the whole grid. Entries identical to zero are depicted as white, positive entries are depticed as different tones of red.
 * @param grid 2D double array of the same size as the output figure (in pixels)
 * @param output_height height of the @c grid parameter and of the PPM image
 * @param output_width width of the @c grid parameter and of the PPM image
 * @param outfile Character array that contains the output file name
 * @return @c void
 */
void write_grid(double** grid, int output_height, int output_width, char outfile[]);


/**
 * @brief Generates output snapshot for hare and puma populations
 *
 * This function is a wrapper of @c write_grid and @c map_to_color that generates  a plain PPM based on the hare and puma populations in a given step in the simulation.
 * @param grid 2D array that contains the state of the simulation
 * @param output_template 2D double array that will be used as the canvas for the plain PPM file
 * @param input_height height of the input grid
 * @param input_width width of the input grid
 * @param output_height height of the plain PPM file, in pixels
 * @param output_width width of the plain PPM file, in pixels
 * @param output_name character array that contains the output file name
 * @param iter current iteration in the simulation, relevant to name the output file
 * @param output_folder 
 * @return @c void
 */

void generate_output(Square_Population** grid, double** output_template, int input_height, int input_width, int output_height, int output_width, char* output_name, int iter, char* output_folder);


/**
 * @brief Finds the maximum and minimum (positive) values of a 2D array
 *
 * Scans a 2D array and finds the maximum value and minimum positive value in it. It is used to normalize the values and generate the plain PPM file
 * @param grid 2D array of double precission numbers
 * @param max pointer to the variable where the maximum will be stored
 * @param min pointer to the variable where the minimum will be stored
 * @param height height of the 2D array that is going to be scanned
 * @param width width of the 2D array that is going to be scanned
 * @return @c void
 */

void find_grid_extremes(double** grid, double *max, double *min, int height, int width);

/**
 * @brief generates a grid of values between 0 and 200
 *
 * Takes a grid of values and normalizes it to get double precission numbers between 0 and 200. This represents the intensity of the G and B (from RGB) colors to be used in the plain PPM file, so the population density can easily be observed.
 * @param grid 2D array that contains the state of the simulation
 * @param color 2D double array that will hold the color intensity
 * @param step normalizing constant 
 * @param min Minimum positive value in the @c grid 
 * @param height height of the 2D array holding the color intensities
 * @param width width of the 2D array holding the color intensities
 * @return @c void
 */

void map_to_color(double** grid, double** color, double step, double min, int height, int width);

/**
 * @brief Generates a null halo around the terrain in the grid
 *
 * Fills the border of the 2D array with zeros.
 * @param square 2D array whose border is going to be set to zero
 * @param height height of the sides of the 2D array
 * @param width width of the sides of the 2D array
 * @return @c void
 */

void fill_halo_entries(int** square, int height, int width);

#endif
