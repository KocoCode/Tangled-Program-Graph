FLAGS = -std=c++11
all: bidder_test

bidder_test: bidder_test.o bidder.o instruction.o
	g++ $(FLAGS) -o $@ $^

%.o: %.cpp
	g++ $(FLAGS) -c $^

clean:
	rm -f bidder_test *.o