# from subprocess import call
from os import system
import sys

cluster = "kunpeng"

log_nrml = "{0}_log_normal".format(cluster)
log_O3 = "{0}_log_O3".format(cluster)

system('rm *.bin *_log_* -rf')
system('g++ -O0 complex_vector.cpp -o complextest_normal.bin')
system('g++ -O3 complex_vector.cpp -o complextest_O3.bin')



########## Normal
# Warm
eN = 5  # MaxLen = 9e+eN
Vlen = 100
for Log in range(2, eN):
    Vlen *= 10
    for n in range(1, 10):
        NVlen = Vlen * n
        RunCmdNormal = "./complextest_normal.bin {0}".format(NVlen, log_nrml)
        system(RunCmdNormal)


# Run
eN = 8  # MaxLen = 9e+eN
Vlen = 100
for Log in range(2, eN):
    Vlen *= 10
    for n in range(1, 10):
        NVlen = Vlen * n
        RunCmdNormal = "./complextest_normal.bin {0} >> {1}".format(NVlen, log_nrml)
        print(RunCmdNormal)
        system(RunCmdNormal)

########## O3
# Warm
eN = 5  # MaxLen = 9e+eN
Vlen = 100
for Log in range(2, eN):
    Vlen *= 10
    for n in range(1, 10):
        NVlen = Vlen * n
        RunCmdO3 = "./complextest_O3.bin     {0}".format(NVlen, log_O3)
        system(RunCmdO3)

# Run
eN = 8  # MaxLen = 9e+eN
Vlen = 100
for Log in range(2, eN):
    Vlen *= 10
    for n in range(1, 10):
        NVlen = Vlen * n
        RunCmdO3 = "./complextest_O3.bin     {0} >> {1}".format(NVlen, log_O3)
        print(RunCmdO3)
        system(RunCmdO3)


        # RunCmd = f"./test {NVlen} 2>&1 | tee log"
