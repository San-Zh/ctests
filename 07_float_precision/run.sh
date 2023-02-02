# gcc -O3 -Wall prec_float.c  2>&1 | tee log
gcc -O3 -Wall test.c  2>&1 | tee log
# -Ofast: 等效于 -O3 -ffast-math
# -march=native(core-avx2) -fdump-tree-vect -fopt-info-vec 
./a.out 10000000 1 2>&1 | tee -a log
