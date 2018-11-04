CC=gcc

PHF=puma_hare_funcs
IOF=io_funcs
DEP_OBJECTS=obj/$(IOF).o obj/$(PHF).o

.PHONY: clean
.PHONY: help

## bin/run_simulation: compiles main program
bin/run_simulation: $(DEP_OBJECTS) src/main.c
	@if [ ! -d "bin" ]; then\
        mkdir bin;\
    fi
	@$(CC) $^ -o $@ -lm -lcunit

## help: prints each repice's purpose
help: Makefile
	@sed -n 's/^##//p' $<

## test_results.txt: run tests and save the results to test_results.txt
test_results.txt: $(DEP_OBJECTS) obj/$(PHF)_test.o obj/$(IOF)_test.o
	@if [ ! -d "bin" ]; then\
        mkdir bin;\
    fi

	@$(CC) $^ -o bin/run_tests src/test/run_tests.c  
	@bin/run_tests > $@ && printf "tests results saved to test_results.txt\n"

## obj/$(PHF)_test.o: buids puma_hare_funcs  tests
obj/$(PHF)_test.o: src/test/$(PHF)_test.c obj/$(PHF).o
	@if [ ! -d "obj" ]; then\
        mkdir obj;\
    fi
	@$(CC) -c -o $@ $< -lm -lcunit #build test object


## obj/$(IOF)_test.o: buids I/O funcs. tests
obj/$(IOF)_test.o: src/test/$(IOF)_test.c obj/$(IOF).o
	@if [ ! -d "obj" ]; then\
        mkdir obj;\
    fi
	@$(CC) -c -o $@ $< -lm -lcunit #build test object

## obj/$(IOF).o: compile io_funcs.c and send object files to obj/
obj/$(IOF).o : src/$(IOF).c
	@if [ ! -d "obj" ]; then\
        mkdir obj;\
    fi
	@$(CC) -c -o $@ $< -lm -lcunit #build source object

## obj/$(PHF).o: compile puma_hare_funcs.c and send object files to obj/
obj/$(PHF).o : src/$(PHF).c
	@if [ ! -d "obj" ]; then\
        mkdir obj;\
    fi
	@$(CC) -c -o $@ $< -lm -lcunit #build source object

## clean: remove files from bin/ and object files from obj/
clean:
	@rm -f obj/*.o
	@rm -f bin/*
