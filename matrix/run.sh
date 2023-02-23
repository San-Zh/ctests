# 

TENSOR='-DCOLUMN=2 -DROW=8'
# PREC='-DSINGLE_PREC'
# GSIZE='-DSIZE'
CXXFLAG='-mfma -mavx512vl  -Ofast'

# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=64
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=128
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=256
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=512
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=16*16*16*16 
# ./a.out

# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=4
# ./a.out

# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=16
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=32
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=64
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=128
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=256
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=512
# ./a.out
#                                                       #       S    D
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024       # 256   64
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*2     # 128   32
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*4     # 64    16
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*8     # 32    8
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*16    # 16    4
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*32    # 8     2
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*64    # 4     1
./a.out

# Grid Size is important, because data mem shuuold mache cache size
