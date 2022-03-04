from subprocess import call
from os import system
import sys

system('rm *.bin log_O3 log_normal -rf')
system('g++ -g complex_vector.cpp -o complextest_normal.bin')
system('g++ -g -O3 complex_vector.cpp -o complextest_O3.bin')

Vlen = 100
for Log in range(2, 6):
    Vlen *= 10
    for n in range(1, 10):
        NVlen = Vlen * n
        
        RunCmdNormal = f"./complextest_normal.bin {NVlen} >> log_normal"
        print(RunCmdNormal)
        system(RunCmdNormal)

        RunCmdO3 = f"./complextest_O3.bin     {NVlen} >> log_O3"
        print(RunCmdO3)
        system(RunCmdO3)

        # RunCmd = f"./test {NVlen} 2>&1 | tee log"
