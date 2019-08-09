all: engine

engine: engine.cc
	g++ -Wall -Wextra -g engine.cc -o idle

clear:
	rm idle
