/**
 * @brief File containing prototypes of all function related to population calculations
 *
 * File containing prototypes of all function related to calculating the number of hares and pumas
 */

#ifndef PUMA_HARE_H_INCLUDED
#define PUMA_HARE_H_INCLUDED

/**
 * @brief A structure to represent a single terrain square in the simulation.
 *
 * This structure represents a single grid's square and it containts information about its populations density at a given time and the terrain information.
 */
typedef struct square{

	double puma_old; ///< Puma population density in the last iteration
	double puma_new; ///< Puma population density in the current iteration

	double hare_old; ///< Hare population density in the last iteration
	double hare_new; ///< Hare population density in the current iteration

	int is_land; ///< this indicates if it is a land or water square

}Square_Population;

/**
 * @brief Updates the density of hares
 *
 * This function calculates the density of hares after a @delta_t timestep.
 * The grid should only contain non-negative density. Therefore, the function judge make a
 * judgment and then output the result.
 * @param H_i_j Former density of hares in some point
 * @param adjacent_hares Density of the hares which are adjacent to H_i_j
 * @param P_ij_old Former density of hares in some point
 * @param hare_birth_rate The birth rate of the hares, which can be changed in the parameter list outside the program
 * @param prey_consumption_rate The consumption rate of the preys, which can be changed in the parameter list outside the program
 * @param hare_difussion_rate The diffusion rate for hares, which can be changed in the parameter list outside the program
 * @param adjacent_land_squares The situation of the points which are adjacent to the point of H_i_j
 * @param float delta_t A very short time
 * @return a @c double containing the new population density
 */

double update_hare_population(double H_i_j, double adjacent_hares, double P_ij_old, float hare_birth_rate, float prey_consumption_rate, float hare_difussion_rate, int adjacent_land_squares, float delta_t);


/**
 * @brief Updates the density of pumas
 *
 * This function calculates the density of pumas after a @delta_t timestep.
 * The grid should only contain non-negative density. Therefore, the function judge make a
 * judgment and then output the result.
 * @param P_i_j Former density of pumas in some point
 * @param adjacent_pumas Density of the pumas which are adjacent to P_i_j
 * @param H_i_j Former density of hares in some point
 * @param puma_birth_rate The birth rate of the pumas, which can be changed in the parameter list outside the program
 * @param puma_mortality_rate The puma mortality rate, which can be changed in the parameter list outside the program
 * @param puma_difussion_rate The diffusion rate for pumas, which can be changed in the parameter list outside the program
 * @param adjacent_land_squares The situation of the points which are adjacent to the point of H_i_j
 * @param float delta_t A very short time
 * @return a @c double containing the new population density
*/
double update_puma_population(double P_i_j, double adjacent_pumas, double H_i_j, float puma_birth_rate, float puma_mortality_rate, float puma_difussion_rate, int adjacent_land_squares, float delta_t);


/**
 * @brief initialise population grid
 *
 * @param This function initialise grid with population of hares and pumas based on  a 2D array that represents the terrain
 * @param grid 2D array that initial population of hares and pumas will be set to
 * @param land 2D int array that indicates the state of the square, take values @0 or @1
 * @param height grid height
 * @param width grid width
 * @return @c void
 */
void initialize_population_grid(Square_Population** grid, int** land, int height, int width);

/**
 * @brief update populations in the grid given the parameters
 *
 * This function is a wrapper for @c update_puma_population and @update_hare_population that scans the whole grid and apply them to each square.
 * @param grid 2D array where new population of hares and pumas will be written
 * @param height grid height
 * @param width grid width
 * @param hare_birth_rate the rate of hares born. Only relevant when @update_hare_population is called
 * @param prey_consumption_rate the rate of hares being eaten.Only relevant when @update_hare_population is called
 * @param hare_difussion_rate the rate of hares spreading out. Only relevant when @update_hare_population is called
 * @param puma_birth_rate the rate of pumas born.Only relevant when @update_puma_population is called
 * @param puma_morality_rate the rate of pumas die.Only relevant when @update_puma_population is called
 * @param puma_difussion_rate the rate of pumas spreading out.Only relevant when @update_puma_population is called
 * @param t size of timestep
 * @return @c void
 */

void update_population_grid(Square_Population** grid, int input_height, int input_width, float hare_birth_rate, float prey_consumption_rate, float hare_difussion_rate, float puma_birth_rate, float puma_mortality_rate, float puma_difussion_rate, float t);



/**
 * @brief output results in grid to screens
 *
 * This function prints data in grid according to parameter idx
 * @param grid 2D array that contains the state of grid and new population of hares and pumas
 * @param height grid height
 * @param width grid width
 * @param idx integer index that determines output data, takes value @1 (land) or @ 2 (pumas) or @ 3 (hares)
 * @return @c void
 */
void print_grid(Square_Population **grid, int height, int width, int idx);



/**
 * @brief output average population of hares and pumas
 *
 * This function creates a 2D array template where averages of the actual population grid are stored. Its main funcitonality (the way in which it averages the grid) dependes on wether the input grid is bigger than the output image (defaul 350 x 350). If the population grid is bigger than this, then each population square is mapped to an output pixel by rounding, and if the input is smaller than the output image, the output image is mapped to the input image also by rounding.
 * @param grid 2D array that contains average population of hares and pumas
 * @param output_template 2D double array that will be used as the canvas for the plain PPM file
 * @param input_height input grid height
 * @param input_width input grid width
 * @param output_height output grid height
 * @param output_width output grid width
 * @param population that determines data to print out, takes values of @ 2 (pumas) or @ 3 (hares)
 * @return @c void
 */
void average_grid(Square_Population** grid, double** output_template, int input_height, int input_width, int output_height, int output_width, int population);
#endif 
