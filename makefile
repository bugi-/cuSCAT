EXEC_NAME = main.out
serial_file = main.cpp
headers = types.h cuScat.h
obj_files = Ray.o Cloud.o
others = scattering.c Cloud.cpp Ray.cpp
plot = plot.py
cpp_compiler = g++
editor = gedit

main_params = -O3

debug_params = -g -Wall

profile_params = -pg -Wall

main: $(serial_file) $(dependencies) $(obj_files)
	$(cpp_compiler) $(serial_file) $(headers) $(main_params) -o $(EXEC_NAME)

debug: $(serial_file) $(dependencies) $(obj_files)
	$(cpp_compiler) $(serial_file) $(headers) $(debug_params) -o $(EXEC_NAME)

profile:$(serial_file) $(dependencies) $(obj_files)
	$(cpp_compiler) $(serial_file) $(headers) $(profile_params) -o $(EXEC_NAME)

# Opens all source and header files for editing
edit:
	$(editor) $(serial_file) $(headers) $(others) &

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

