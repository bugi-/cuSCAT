EXEC_NAME = main.out
serial_file = scat_new.cpp
headers = types.h constants.h
cpp_compiler = g++

main_params = -O3

debug_params = -g -Wall

main: $(serial_file) $(headers)
	$(cpp_compiler) $(serial_file) $(headers) $(main_params) -o $(EXEC_NAME)

debug: $(serial_file) $(headers)
	$(cpp_compiler) $(serial_file) $(headers) $(debug_params) -o $(EXEC_NAME)

gdb:
	gdb ./$(EXEC_NAME)

run:
	./$(EXEC_NAME)

clean:
#	rm *.o
	rm *.gch

