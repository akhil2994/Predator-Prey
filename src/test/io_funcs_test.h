#ifndef IO_FUNCS_TEST_H_INCLUDED
#define IO_FUNCS_TEST_H_INCLUDED

int init_io_suite(void);

int clean_io_suite(void);

void test_get_input_size(void);

void test_read_input(void);

void test_fill_halo_entries(void);

void test_find_grid_extremes(void);

void test_map_to_color(void);

int run_io_tests();

#endif