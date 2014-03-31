EXEC_NAME = main.out
serial_file = main.cpp
dependencies = types.h cuScat.h Ray.o Cloud.o
others = scattering.c Cloud.cpp Ray.cpp
plot = plot.py
cpp_compiler = g++
editor = gedit

main_params = -O3

debug_params = -g -Wall

profile_params = -pg -Wall

main: $(serial_file) $(dependencies)
	$(cpp_compiler) $(serial_file) $(dependencies) $(main_params) -o $(EXEC_NAME)

debug: $(serial_file) $(dependencies)
	$(cpp_compiler) $(serial_file) $(dependencies) $(debug_params) -o $(EXEC_NAME)

profile:$(serial_file) $(dependencies)
	$(cpp_compiler) $(serial_file) $(dependencies) $(profile_params) -o $(EXEC_NAME)

# Opens all source and header files for editing
edit:
	$(editor) $(serial_file) $(dependencies) $(others)

gdb:
	gdb ./$(EXEC_NAME)

gprof:
	gprof $(EXEC_NAME)

run:
	./$(EXEC_NAME)

time:
	time ./$(EXEC_NAME)

plot:
	python $(plot)

Ray.o:
	gcc -c Ray.cpp

Cloud.o:
	gcc -c Cloud.cpp

clean:
	rm *.o
	rm *.gch

