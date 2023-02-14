CC       = gcc
CFLAGS   =
CXX      = g++
CXXFLAGS = 

SOURCE = ./
VPATH = $(SOURCE)

ALL:main.out
TARGET:solver.bin 


%.o: ${SOURCE}/%.c
	${CC} ${CFLAGS} -o $@ -c $<

%.o: ${SOURCE}/%.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $<


# main.out: main.o solver.o 
# 	${CXX} ${CXX_FLAGS} -o $@  $^



main.out: main.cpp solver.cpp
	${CXX} ${CXX_FLAGS} -o $@  $^



clean:
	rm *.o main.out -rf