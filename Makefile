CC := g++

ifeq ($(OS),Windows_NT)
CFLAGS := -mwindows -pthread -lPdh -std=c++17
LIBS = -lws2_32
else
CFLAGS := -std=c++17
LIBS = -lws2_32
endif

SRC_DIR = ./src
NETWORK_DIR = $(SRC_DIR)/network

DEBUG_CFLAGS := -g

all: game

game:  $(SRC_DIR)/*.cpp $(NETWORK_DIR)/*.cpp
	$(CC) main.cpp $(CFLAGS) -o $@ $(SRC_DIR)/*.cpp $(NETWORK_DIR)/*.cpp $(LIBS)

debug: game_debug

game_debug: $(SRC_DIR)/*.cpp $(NETWORK_DIR)/*.cpp
	$(CC) $(DEBUG_CFLAGS) main.cpp $(CFLAGS) -o $@ $(SRC_DIR)/*.cpp $(NETWORK_DIR)/*.cpp $(LIBS)

clean:
ifeq ($(OS),Windows_NT)
	del game.exe
	del game_debug.exe
else
	rm -f game
	rm -f game_debug
endif