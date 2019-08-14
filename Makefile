all: engine

engine: engine.cc
	g++ -Wall -Wextra -g engine.cc socket.cc connection.cc -o idle

clear:
	rm idle
