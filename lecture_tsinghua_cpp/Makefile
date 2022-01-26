
CXX = g++
CXXFLAGS = -g -O3

OBJS = main.o clock.o

HDRS = clock.h point.h

TARGET = main.bin

all: ${TARGET}

main.bin: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(HDRS) -o $@

%.o : %.cc ${HDRS}
	$(CXX) ${CXXFLAGS} -c $< -o $@

clean:
	rm -f ${OBJS} ${TARGET}

.PHONEY: all clean
