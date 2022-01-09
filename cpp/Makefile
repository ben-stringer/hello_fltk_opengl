
UNAME:=$(shell uname -s)

ifeq ($(UNAME),Darwin)
	LDFLAGS=-framework GLUT -framework OpenGL
endif
ifeq ($(UNAME),Linux)
	LDFLAGS=-lglut -lOpenGL -lGLU
endif

CXXFLAGS=-g
CXX=/usr/bin/clang++
DEPS = assignment2.h
OBJ = assignment2.o \
	  assignment2_helper.o

%.o: %.c $(DEPS)
	$(CXX) -cpp -o $@ $< $(CXXFLAGS)

assignment2: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 

.PHONY: clean
clean:
	rm -f *.o assignment2
