# This Makefile is used to compile and run the gemm.c program.

# Compiler and linker options
CC = gcc
CFLAGS = -O3 -Wall -fopenmp -DV0 -mavx2 -mfma -march=native
LDFLAGS = -lm -lgomp

HAVE_BLAS = No

ifeq ($(HAVE_BLAS), Yes)
# BLAS library path
	BLAS_PATH = /usr/local
# Add the BLAS library to the linker options
	LDFLAGS += -L$(BLAS_PATH)/lib -Wl,-rpath=$(BLAS_PATH)/lib -lcblas
# Add the BLAS library include path to the compiler options
	CFLAGS += -I$(BLAS_PATH)/include
endif

# Targets
all: gemm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

gemm: gemm.c
	$(CC) $(CFLAGS) -o gemm gemm.c $(LDFLAGS)

clean:
	rm -f ./gemm *.o

run: gemm
	export OMP_NUM_THREADS=4
	./gemm 16

.PHONY: clean run
