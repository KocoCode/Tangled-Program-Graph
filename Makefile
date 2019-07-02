FLAGS = -std=c++17 -O3 -Wall -Wextra -Wshadow -pedantic
OBJS = poolproxy.o bidder.o team.o instruction.o
TESTS = bidder_test team_test pool_test poolproxy_test tpg_test
ALEINCLUDES =
ALELIBS =
all: tpgale
tpgale: tpgale.cpp tpg.o $(OBJS)
	g++ $(FLAGS) -I$(ALEINCLUDES) -L$(ALELIBS) -lale -o $@ $^

test: $(TESTS)

bidder_test: bidder_test.o $(OBJS)
	g++ $(FLAGS) -o $@ $^

team_test: team_test.o $(OBJS)
	g++ $(FLAGS) -o $@ $^

pool_test: pool_test.o
	g++ $(FLAGS) -o $@ $^

poolproxy_test: poolproxy_test.o $(OBJS)
	g++ $(FLAGS) -o $@ $^

tpg_test: tpg_test.o tpg.o $(OBJS)
	g++ $(FLAGS) -o $@ $^

%.o: %.cpp
	g++ $(FLAGS) -c $^

clean:
	rm -f $(TESTS) *.o
