gcc -O3 -ffast-math -march=core-avx2 -fdump-tree-vect -fopt-info-vec  -Wall prec_float.c  2>&1 | tee log
# -Ofast: 等效于 -O3 -ffast-math -march=native(core-avx2) 
./a.out 100000 10 2>&1 | tee -a log
