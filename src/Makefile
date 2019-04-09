CXXFLAGS = -std=c++11 -O3 -W -Wall -Wextra -Wno-unused-parameter
ifeq ($(opt), mingw)
	LD_FLAGS = -lmingw32 -lSDL2main -lSDL2 
else
	LD_FLAGS = -lSDL2 
endif

TARGET_O = main.o ai.o board.o game.o piece.o situation.o
TARGET = gobang

.PHONY : all clean

all : $(TARGET)

$(TARGET) : $(TARGET_O)
	g++ -o $(TARGET) $(TARGET_O) $(LD_FLAGS)


main.o : main.cpp game.h board.h situation.h
ai.o : ai.cpp ai.h piece.h
board.o : board.cpp board.h game.h
game.o : game.cpp game.h
piece.o : piece.cpp piece.h
situation.o : situation.cpp situation.h game.h piece.h ai.h

clean :
	rm -rf $(TARGET_O)