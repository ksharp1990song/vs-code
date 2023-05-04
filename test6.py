import os
import subprocess

x3 = None
x4 = None


# 调用 C++ 可执行文件，它将设置 x3 和 x4 的值
result = subprocess.run(["C:/Users/uk093878/C++/C_python7.exe"], capture_output=True, text=True)

# 从 C++ 可执行文件的输出中获取 x3 和 x4 的值
x3_str, x4_str = result.stdout.strip().split()
x3 = int(x3_str)
x4 = float(x4_str)

# 打印 x3 和 x4 的值
print(x3)
print(x4)