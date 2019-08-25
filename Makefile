SRCS = engine.cc socket.cc connection.cc idle-engine.cc resource.cc 
all: engine

engine: engine.cc
	g++ -std=c++14 -Wall -Wextra -g $(SRCS) -o idle-engine

clear:
	rm idle
