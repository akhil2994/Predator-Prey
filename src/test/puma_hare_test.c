#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "../puma_hare.h"
#include "puma_hare_test.h"

const double epsilon = 1E-06;

void test_update_hare_population(void){

/*update_hare_population(double H_i_j, double adjacent_hares, double P_ij_old, float hare_birth_rate, float prey_consumption_rate, float hare_difussion_rate, int adjacent_land_squares, float delta_t)*/
  // delta_t test

  CU_ASSERT_DOUBLE_EQUAL(update_hare_population(3.4,1.0,1.0,1.0,1.0,1.0,1, 0.0), 3.4, epsilon);

  //adjacent_population tezt
  CU_ASSERT_DOUBLE_EQUAL(update_hare_population(0.0, 4.0, 3.0, 2.0, 1.5, 1.0, 3, 1.0), 4.0, epsilon);

  //birth vs consumption test
  CU_ASSERT_DOUBLE_EQUAL(update_hare_population(1.0, 1.1, 1.0, 0.8, 0.5, 0.0, 4, 1.0),1.0 + 0.8 - 0.5,epsilon);

  // non-negativity test
  CU_ASSERT_DOUBLE_EQUAL(update_hare_population(1.0, 1.1, 4.0, 0.1, 0.5, 0.0, 4, 1.0),0.0,epsilon);
  
}


void test_update_puma_population(void){

  // delta_t test
  CU_ASSERT_DOUBLE_EQUAL(update_puma_population(3.4,1.0,1.0,1.0,1.0,1.0,1, 0.0), 3.4, epsilon);

  //adjacent_population tezt
  CU_ASSERT_DOUBLE_EQUAL(update_hare_population(0.0, 4.0, 3.0, 2.0, 1.5, 1.0, 3.0, 1.0), 4.0, epsilon);

  //birth vs consumption test
  CU_ASSERT_DOUBLE_EQUAL(update_hare_population(1.0, 1.1, 1.0, 0.8, 0.5, 0.0, 4.0, 1.0), 1.0 + 0.8 - 0.5, epsilon);

  // non-negativity test
  CU_ASSERT_DOUBLE_EQUAL(update_hare_population(1.0, 1.1, 1.0, 0.1, 1.2, 0.0, 4.0, 1.0), 0.0, epsilon);

   
}

void test_initialize_population_grid(void){

	int i,j,input_size = 3;

  int** land = (int**) malloc((input_size)*sizeof(int*));

  for(i=0;i<input_size;i++){
      land[i]= (int*) malloc((input_size)*sizeof(int));
  }

  for(i=0;i<input_size;i++){
    for(j=0;j<input_size;j++){
      land[i][j] = 1;
    }
  }

	Square_Population** grid = (Square_Population**) malloc(sizeof(Square_Population*)*input_size);

	for(i=0;i<input_size;i++){
        grid[i] = (Square_Population*) malloc((input_size)*sizeof(Square_Population));
    }

	initialize_population_grid(grid, land, input_size, input_size);

	for(i=0;i<input_size;i++){
		for(j=0;j<input_size;j++){

			CU_ASSERT(grid[i][j].hare_old > 0);

      if(grid[i][j].puma_old>epsilon){

        CU_ASSERT(1.0 == grid[i][j].puma_old);
      }

		}
	}

	for(i=0;i<input_size;i++){
    for(j=0;j<input_size;j++){
      land[i][j] = 0;
    }
  }

	initialize_population_grid(grid, land, input_size,input_size);

	for(i=0;i<input_size;i++){
		for(j=0;j<input_size;j++){
			CU_ASSERT_DOUBLE_EQUAL(grid[i][j].puma_old, 0, epsilon);
			CU_ASSERT_DOUBLE_EQUAL(grid[i][j].hare_old, 0, epsilon);
		}
	}
}

void test_average_grid(){

  //test grid
  Square_Population test_grid[2][2] = {{{.puma_old = 1.0, .puma_new = 1.0, .hare_old = 2.0, .hare_new = 2.0, .is_land = 1},\
                                       {.puma_old = 2.0, .puma_new = 2.0, .hare_old = 4.0, .hare_new = 4.0, .is_land = 1}},\
                                       {{.puma_old = 3.0, .puma_new = 3.0, .hare_old = 6.0, .hare_new = 6.0, .is_land = 1},\
                                       {.puma_old = 4.0, .puma_new = 4.0, .hare_old = 8.0, .hare_new = 8.0, .is_land = 1}}};


  int i, j, input_height = 4, input_width = 4, output_height = 1, output_width = 1;

  Square_Population** grid = (Square_Population**) malloc(sizeof(Square_Population*)*(input_height));

  for(i=0;i<input_height;i++){
        grid[i] = (Square_Population*) malloc((input_width)*sizeof(Square_Population));
  }
  // initialize test grid
  for(i=1;i<3;i++){
    for(j=1;j<3;j++){
      grid[i][j] = test_grid[i-1][j-1];
    }
  }

  for(i=0;i<input_height;i++){
    for(j=0;j<input_width;j++){
      if(i==0 || i == input_height-1 || j == 0 || j == input_width - 1){
        grid[i][j].is_land = 0;
        grid[i][j].puma_old = 0;
        grid[i][j].puma_new = 0;
        grid[i][j].hare_old = 0;
        grid[i][j].hare_new = 0;
      }
    }
  }
  int population = 2;

  double** output_template1 = malloc(sizeof(double*)*1);
  output_template1[0] = malloc(sizeof(double)*1);

  average_grid(grid, output_template1, input_height, input_width, output_height, output_width, population);

  CU_ASSERT_DOUBLE_EQUAL(output_template1[0][0],2.5, epsilon);

  population = 3;

  average_grid(grid, output_template1, input_height, input_width, output_height, output_width, population);


  CU_ASSERT_DOUBLE_EQUAL(output_template1[0][0],5.0, epsilon);

  population = 2;
  output_width = 2;
  output_height = 2;

  double** output_template2 = malloc(sizeof(double*)*output_height);
  output_template2[0] = malloc(sizeof(double)*output_width);
  output_template2[1] = malloc(sizeof(double)*output_width);

  average_grid(grid, output_template2, input_height, input_width, output_height, output_width, population);

  for(i=0;i<output_height;i++){
    for(j=0;j<output_width;j++){
      CU_ASSERT_DOUBLE_EQUAL(output_template2[i][j],grid[i+1][j+1].puma_new, epsilon);
    }
  }

  population = 3;

  average_grid(grid, output_template2, input_height, input_width, output_height, output_width, population);

  for(i=0;i<output_height;i++){
    for(j=0;j<output_width;j++){
      CU_ASSERT_DOUBLE_EQUAL(output_template2[i][j],grid[i+1][j+1].hare_new, epsilon);
    }
  }

}

int run_puma_hare_tests(){

	CU_pSuite pSuite = NULL;

	if (CUE_SUCCESS != CU_initialize_registry()){ //initializes test registry
		return CU_get_error();
	}

	pSuite = CU_add_suite("puma_hare_suite", NULL, NULL); // adds suite to registry

	if (NULL == pSuite) { // stop if suite could not be added
      CU_cleanup_registry();
      return CU_get_error();
   }

   if(NULL == CU_add_test(pSuite, "test of hare population update", test_update_hare_population)){ // add test function to suite
   	  CU_cleanup_registry();
      return CU_get_error();
   }

   if(NULL == CU_add_test(pSuite, "test of puma population update", test_update_puma_population)){ // add test function to suite
   	  CU_cleanup_registry();
      return CU_get_error();
   }

   if(NULL == CU_add_test(pSuite, "test of grid initializer", test_initialize_population_grid)){ // add test function to suite
   	  CU_cleanup_registry();
      return CU_get_error();
   }

   if(NULL == CU_add_test(pSuite, "test of grid averaging", test_average_grid)){ // add test function to suite
      CU_cleanup_registry();
      return CU_get_error();
   }
   
	/* Run all tests */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}