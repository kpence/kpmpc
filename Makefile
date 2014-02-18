# Configuration
BIN_DIR = ~/bin/
BIN = kpmpc

# Libraries
LIBS = -lm
LIBS += -lmpdclient
LIBS += -lsfml-graphics -lsfml-window -lsfml-system
LIBS += -lboost_system -lboost_filesystem

# Other variables
CC = g++
CFLAGS = -g -Wall
SRCS = main.cpp

# Helper
OBJS = $(SRCS:.cpp=.o)

.PHONY: all install clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(BIN)

#EOF
