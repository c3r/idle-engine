all: engine

engine: engine.cc
	g++ -std=c++14 -Wall -Wextra -g engine.cc socket.cc connection.cc -o idle

clear:
	rm idle
