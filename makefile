EXEC_NAME = main.out
serial_file = main.cpp
dependencies = types.h cuScat.h constants.h
others = scattering.c
cpp_compiler = g++
editor = gedit

main_params = -O3

debug_params = -g -Wall

main: $(serial_file) $(dependencies)
	$(cpp_compiler) $(serial_file) $(dependencies) $(main_params) -o $(EXEC_NAME)

debug: $(serial_file) $(dependencies)
	$(cpp_compiler) $(serial_file) $(dependencies) $(debug_params) -o $(EXEC_NAME)

# Opens all source and header files for editing
edit:
	$(editor) $(serial_file) $(dependencies) $(others)
gdb:
	gdb ./$(EXEC_NAME)

run:
	./$(EXEC_NAME)

clean:
	rm *.o
	rm *.gch

