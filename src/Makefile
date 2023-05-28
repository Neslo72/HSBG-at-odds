CXX = g++
CXXFLAGS = -g -Wall -Werror -I $(INC)

TARGET = main

INC = include
BIN = bin

########## MAIN ##########

all : directories $(TARGET)

directories :
	$(shell mkdir -p bin)

BOARD_O = $(BIN)/board.o
MINION_O = $(BIN)/minion.o 
# BEASTS_O = $(BIN)/beasts.o
MINIONS_O = $(BIN)/minions.o 

$(TARGET) : $(BOARD_O) $(MINION_O) $(MINIONS_O) main.cc
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BOARD_O) : board.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(MINION_O) : minion.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

# $(BEASTS_O) : beasts.cc
# 	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(MINIONS_O) : beasts.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

########## CLEAN ##########

clean : 
	rm -f $(TARGET)
	rm -rf $(BIN)