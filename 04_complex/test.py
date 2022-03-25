# from subprocess import call
from os import system
import sys

cluster = "kunpeng"

log_nrml = "{0}_log_normal".format(cluster)
log_O3 = "{0}_log_O3".format(cluster)

system('rm *.bin *_log_* -rf')
system('g++ -O0 complex_vector.cpp -o complextest_normal.bin')
system('g++ -O3 complex_vector.cpp -o complextest_O3.bin')



Vlen = 100
for Log in range(2, 6):
    Vlen *= 10
    for n in range(1, 10):
        NVlen = Vlen * n
        
        RunCmdNormal = "./complextest_normal.bin {0} >> {1}".format(NVlen, log_nrml)
        print(RunCmdNormal)
        system(RunCmdNormal)

        RunCmdO3 = "./complextest_O3.bin     {0} >> {1}".format(NVlen, log_O3)
        print(RunCmdO3)
        system(RunCmdO3)

        # RunCmd = f"./test {NVlen} 2>&1 | tee log"
