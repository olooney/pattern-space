# PatternSpace

# compilers with maxed-out debuging info
CPP  = g++
CC   = gcc

LINKOBJ  = main.o mass.o image.o solid.o universe.o factories.o ship.o
OBJ  = $(LINKOBJ)
LIBS =  -lSDLmain -lSDL -lSDL_gfx 
BIN  = PatternSpace

CXXFLAGS = -D__DEBUG__ -g3  
RM = rm -f

.PHONY: all clean

all: $(BIN)

clean: 
	$(RM) $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $@ $(LIBS)

%.o : %.cpp
	$(CPP) $(CXXFLAGS) -c $^ -o $@
