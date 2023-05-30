CXX = g++
CXXFLAGS = -g -Wall -Werror -I $(INC)

TARGET = main
PROFILE = profile

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


########## PROFILING ##########

$(PROFILE) : $(BOARD_O) $(MINION_O) $(MINIONS_O) main.cc
	$(CXX) $(CXXFLAGS) -pg $^ -o $@

## after this, run ./profile and gprof profile gmon.out
## to get timing statistics


########## CLEAN ##########

clean : 
	rm -f $(TARGET)
	rm -f $(PROFILE)
	rm -f gmon.out
	rm -rf $(BIN)