CC = g++
CFLAGS = -Wall -pedantic
LDFLAGS = -framework GLUT -framework OpenGL -framework OpenCL
DEBUG_FLAGS = -g
OPTIMIZATION_FLAGS =

SOURCES = $(shell find . -name '*.cpp' | xargs)

OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = mandelbrot

all: $(SOURCES) $(EXECUTABLE)

compile: $(SOURCES) $(OBJECTS)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(DEBUG_FLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OPTIMIZATION_FLAGS) $< -c -o $@

clean:
	rm -rf $(shell find . -name '*.o' | xargs) $(EXECUTABLE)
