# complex mul test

## 编译
### 无优化
```shell
g++ -O3 complex_vector.cpp -o test
```

### -O3
```shell
g++ -O3 complex_vector.cpp -o test
```

### 运行脚本
```python
from subprocess import call
from os import system

Vlen = 100
for Log in range(2, 8):
    Vlen *= 10
    for n in range(1, 10):
        NVlen = Vlen * n
        RunCmd = f"./test {NVlen} >> log"
        # RunCmd = f"./test {NVlen} 2>&1 | tee log"
        print(RunCmd)
        system(RunCmd)
```