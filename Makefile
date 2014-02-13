CC = g++
CFLAGS = -g -Wall
SRCS = main.cpp
MAIN = kpmpc
# LFLAGS = -L/home/kpence/lib
# INCLUDES = 
LIBS = -lm
LIBS += -lmpdclient
LIBS += -lsfml-graphics -lsfml-window -lsfml-system
LIBS += -lboost_system -lboost_filesystem
#
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

# $(MAIN): $(OBJS)
#	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

#EOF
