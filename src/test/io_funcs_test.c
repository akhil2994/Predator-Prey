#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "cute.h"

#include "../puma_hare_funcs.h"
#include "../io_funcs.h"

#include "io_funcs_test.h"


static FILE* example1 = NULL;
static FILE* example2 = NULL;
static FILE* example3 = NULL;
static FILE* example4 = NULL;

int init_io_suite(void){ // read necesary files, to test io functions
	if(NULL == (example1 = fopen("src/test/test_data/example1.dat","r")) ||\
	   NULL == (example2 = fopen("src/test/test_data/example2.dat","r")) ||\
	   NULL == (example3 = fopen("src/test/test_data/example3.dat","r")) ||\
	   NULL == (example4 = fopen("src/test/test_data/example4.dat","r"))){
		return -1;
	}else{
		return 0;
	}
}

int clean_io_suite(void){ // closes the files we opened before once tests have finished
	if(0 != fclose(example1) ||\
	   0 != fclose(example2) ||\
	   0 != fclose(example3) ||\
	   0 != fclose(example4)
     ){
		return -1;
	}else{
		return 0;
	}
}

void test_get_input_size(void){

  int height, width;

  get_input_size("src/test/test_data/example1.dat", &height, &width);

  CU_ASSERT(2 == height);
  CU_ASSERT(4 == width);

  get_input_size("src/test/test_data/example2.dat", &height, &width);

  CU_ASSERT(5 == height);
  CU_ASSERT(9 == width);

  get_input_size("src/test/test_data/example3.dat", &height, &width);

  CU_ASSERT(3 == height);
  CU_ASSERT(3 == width);

  get_input_size("src/test/test_data/example4.dat", &height, &width);

  CU_ASSERT(3 == height);
  CU_ASSERT(3 == width);

}

void test_read_input(void){
  int i,j,input_height = 2, input_width = 4;
  input_height += 2;
  input_width += 2;

  int ones_array[2][4] = {{1,0,1,1},{1,1,0,1}};

  int** square = malloc((input_height)*sizeof(int*));

  for(i=0;i<input_height;i++){
      square[i]= malloc((input_width)*sizeof(int));
  }

  read_input(square, input_height, input_width, "src/test/test_data/example1.dat");

  for(i=1;i<input_height-1;i++){
  	for(j=1;j<input_width-1;j++){
  		CU_ASSERT(square[i][j] == ones_array[i-1][j-1]);
  	}
  }
   
}

void test_fill_halo_entries(void){

  int j,i, input_size = 5;

  int** square = malloc((input_size)*sizeof(int*));

  for(i=0;i<input_size;i++)
  {
      square[i]= malloc((input_size)*sizeof(int));
  }

  fill_halo_entries(square,input_size, input_size);

  for(i=0;i<input_size;i++){
    for(j=0;j<input_size;j++){
      if(i == input_size - 1 || i == 0 || j == input_size-1 || j == 0){
        CU_ASSERT(0 == square[i][j]);
      }
    }

  }


}

void test_find_grid_extremes(void){
	double max, min;
  int height = 4, width = 3;
	int i,j;
  double test_array_vals[4][3] = {{0.12, 0.15, 0.19},{0.0,0.1,0.2},{0.3,0.4,0.5},{0.6,0.7,0.8}};

  double** test_array = malloc((height)*sizeof(double*));

  for(i=0;i<height;i++){
        test_array[i]= malloc((width)*sizeof(double));
  }

  for(i=0;i<height;i++){
    for(j=0;j<width;j++){
      test_array[i][j] = test_array_vals[i][j]; 
    }
  }

	find_grid_extremes(test_array,&max,&min,height,width);

	CU_ASSERT(max == 0.8);

	CU_ASSERT(min == 0.1);

}

void test_map_to_color(void){
	double true_colors[3][3] = {{200,180,160},{140,120,100},{80,60,40}};
	double step=1, min=1;
	double grid_vals[3][3] = {{1,21,41},{61,81,101},{121,141,161}};
	int i,j,output_size = 3;

  double** grid = malloc((output_size)*sizeof(double*));

  for(i=0;i<output_size;i++){
        grid[i]= malloc((output_size)*sizeof(double));
  }

  double** color = malloc((output_size)*sizeof(double*));

  for(i=0;i<output_size;i++){
    for(j=0;j<output_size;j++){
      grid[i][j] = grid_vals[i][j];
    }
  }

  for(i=0;i<output_size;i++){
        color[i]= malloc((output_size)*sizeof(double));
  }

  map_to_color(grid, color, step, min, output_size, output_size);


	for(i=0;i<output_size;i++){
		for(j=0;j<output_size;j++){
			CU_ASSERT(color[i][j] == true_colors[i][j]);
		}
	}

}

int run_io_tests(){

	cute_suite pSuite = NULL;

	//if (CUE_SUCCESS != CU_initialize_registry()){ //initializes test registry
		//return CU_get_error();
//	}

	pSuite = CU_add_suite("io_suite", init_io_suite, clean_io_suite); // adds suite to registry

	if (NULL == pSuite) { // stop if suite could not be added
      CU_cleanup_registry();
      return CU_get_error();
   }

   if(NULL == CU_add_test(pSuite, "test of input size getter", test_get_input_size)){ // add test function to suite
   	  CU_cleanup_registry();
      return CU_get_error();
   }

   if(NULL == CU_add_test(pSuite, "test of input reader", test_read_input)){ // add test function to suite
   	  CU_cleanup_registry();
      return CU_get_error();
   }

   if(NULL == CU_add_test(pSuite, "test of halo entry filler", test_fill_halo_entries)){ // add test function to suite
      CU_cleanup_registry();
      return CU_get_error();
   }

   if(NULL == CU_add_test(pSuite, "test of grid extreme value finder", test_find_grid_extremes)){ // add test function to suite
   	  CU_cleanup_registry();
      return CU_get_error();
   }

   if(NULL == CU_add_test(pSuite, "test of grid color generator", test_map_to_color)){ // add test function to suite
   	  CU_cleanup_registry();
      return CU_get_error();
   }

	/* Run all tests */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
