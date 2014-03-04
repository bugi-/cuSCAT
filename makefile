EXEC_NAME = main.out
serial_file = scat_new.cpp
dependencies = types.h scat.h constants.h
cpp_compiler = g++

main_params = -O3

debug_params = -g -Wall

main: $(serial_file) $(dependencies)
	$(cpp_compiler) $(serial_file) $(dependencies) $(main_params) -o $(EXEC_NAME)

debug: $(serial_file) $(dependencies)
	$(cpp_compiler) $(serial_file) $(dependencies) $(debug_params) -o $(EXEC_NAME)

gdb:
	gdb ./$(EXEC_NAME)

run:
	./$(EXEC_NAME)

clean:
#	rm *.o
	rm *.gch

