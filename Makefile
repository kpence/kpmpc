CC = g++
CFLAGS = -g -Wall
SRCS = main.cpp
MAIN = main
# LFLAGS = -L/home/kpence/lib
# INCLUDES = 
LIBS = -lm
LIBS += -lmpdclient
LIBS += -lGLEW -lglut
LIBS += -lpng -ljpeg
LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
LIBS += -lboost_filesystem
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
