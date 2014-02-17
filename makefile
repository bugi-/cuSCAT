EXEC_NAME = main.out
serial_file = scat_new.cpp
headers = types.h constants.h

main: $(serial_file) $(headers)
	g++ $(serial_file) $(headers) -o $(EXEC_NAME)

run:
	./$(EXEC_NAME)

clean:
#	rm *.o
	rm *.gch

