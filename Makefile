FLAGS = -std=c++17 -O3 -Wall -Wextra -Wshadow -pedantic
TESTS = bidder_test team_test pool_test
all: test
test: $(TESTS)

bidder_test: bidder_test.o bidder.o instruction.o team.o
	g++ $(FLAGS) -o $@ $^

team_test: team_test.o team.o bidder.o instruction.o
	g++ $(FLAGS) -o $@ $^

pool_test: pool_test.o
	g++ $(FLAGS) -o $@ $^

%.o: %.cpp
	g++ $(FLAGS) -c $^

clean:
	rm -f $(TESTS) *.o
