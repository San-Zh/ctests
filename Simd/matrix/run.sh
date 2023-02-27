#

MSIZE=4

TENSOR='-DCOLUMN='$MSIZE' -DROW='$MSIZE
# TENSOR='-DCOLUMN=3 -DROW=3'
# PREC='-DSINGLE_PREC'
# GSIZE='-DSIZE'
CXXFLAG='-mfma -mavx512vl -Ofast'

# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=8
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=16
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=32
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=64
# ./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=128
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=256
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=512
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024       # 256   64
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*2     # 128   32
./a.out
g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*4     # 64    16
./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*8     # 32    8
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*16    # 16    4
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*32    # 8     2
# ./a.out
# g++ $CXXFLAG $PREC $TENSOR matrix.cpp -DSIZE=1024*64    # 4     1
# ./a.out

# Grid Size is important, because data mem shuuold mache cache size
