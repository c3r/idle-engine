all: engine

engine: engine.cc
	g++ -Wall -Wextra -g engine.cc socket.cc -o idle

clear:
	rm idle
