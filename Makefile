CC = g++
SRC = Cdht22.cpp Commands.cpp MyLog.cpp myTime.cpp UDPServer.cpp 
OBJ = $(SRC:%.cpp=%.o)
HDR = $(SRC:%.cpp=%.h)
LIBS = -lwiringPi -lwiringPiDev -lpthread
FLAGS = -c

TempServer: $(OBJ) main.cpp
	$(CC) $(LIBS) -o $@ main.cpp $(OBJ)

%.o: %.cpp %.h
	$(CC) $(LIBS) $(FLAGS) $<

clean:
	- rm *.o
	- rm TempServer
