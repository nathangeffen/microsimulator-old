CC=g++
CFLAGS=-c -Wall -O2 -std=c++0x
LDFLAGS=
SOURCES=basesimulation.cpp simutils.cpp TbSimulation.cpp individual.cpp \
	state.cpp  tb_states.cpp main.cpp state_parameter.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=microsimulator

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm $(OBJECTS) $(EXECUTABLE)
