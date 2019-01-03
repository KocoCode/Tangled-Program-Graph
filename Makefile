FLAGS = -std=c++17
all: team_test
test: bidder_test team_test

bidder_test: bidder_test.o bidder.o instruction.o
	g++ $(FLAGS) -o $@ $^

team_test: team_test.o team.o bidder.o instruction.o
	g++ $(FLAGS) -o $@ $^

%.o: %.cpp
	g++ $(FLAGS) -c $^

clean:
	rm -f team_test bidder_test *.o
