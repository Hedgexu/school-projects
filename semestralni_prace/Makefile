CXX = g++
CXXFLAGS = -std=c++11 -Wall -pedantic -Wno-long-long -O0 -ggdb
SRCDIR = ./src/
CXXOBJECTS = 	$(SRCDIR)client_listen.cpp \
				$(SRCDIR)client_write.cpp \
				$(SRCDIR)server.cpp \
				$(SRCDIR)room.cpp \
				$(SRCDIR)Mess.cpp



all: compile

compile: $(SRCDIR)main.cpp
	$(CXX) $(CXXFLAGS) -o novotad1 $(CXXOBJECTS) $< -lncurses -lpthread

doc:
	doxygen 2>/dev/null

clean:
	rm -f ./*~ $(SRCDIR)*~ ./novotad1
	rm -rf ./doc

run:
	gnome-terminal -x ./novotad1 -server
	gnome-terminal -x ./novotad1 Tester1 room1
	gnome-terminal -x ./novotad1 Tester2 room1
	gnome-terminal -x ./novotad1 Tester3 room2

