import ctypes
import pandas as pd
import numpy as np

example = ctypes.cdll.LoadLibrary("C:/Users/uk093878/source/repos/Project1/x64/Debug/Project1.dll")

update_values = example.update_values
update_values.argtypes = [
    ctypes.POINTER(ctypes.c_long),
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_char_p),
    ctypes.c_int,
    ctypes.POINTER(ctypes.c_int),
]

x1 = ctypes.c_long(1)
x2 = ctypes.c_double(3.14)
x3 = ctypes.c_char_p(b"Hello")
interval_ms = ctypes.c_int(1000)
count = ctypes.c_int(0)

data = []

for _ in range(10):
    update_values(ctypes.byref(x1), ctypes.byref(x2), ctypes.byref(x3), interval_ms, ctypes.byref(count))
    data.append([x1.value, x2.value, x3.value.decode()])
    
df = pd.DataFrame(data, columns=["x1", "x2", "x3"])
print(df)





