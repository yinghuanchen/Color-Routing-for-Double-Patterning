CC = g++ -std=c++11
CFLAGS = -c
OPTFLAGS = -O2
ARFLAGS = rcv

all	: color_route
	@echo -n ""

color_route	: Routing.o main.o
			$(CC) $(OPTFLAGS) Routing.o main.o -o color_route
			# rm main.o Routing.o
main.o 	   	: src/main.cpp
			$(CC) $(CFLAGS) $< -o $@
Routing.o	: src/Routing.cpp src/Routing.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*

