import ctypes
import pandas as pd
import numpy as np

example = ctypes.cdll.LoadLibrary("C:/Users/uk093878/source/repos/Project1/x64/Debug/Project1.dll")

get_data = example.get_data
get_data.argtypes = [
    ctypes.POINTER(ctypes.c_long),
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_char_p),
    ctypes.c_int
]

num_elements = 10
x1_arr = (ctypes.c_long * num_elements)()
x2_arr = (ctypes.c_double * num_elements)()
x3_arr = (ctypes.c_char_p * num_elements)()

get_data(x1_arr, x2_arr, x3_arr, num_elements)

data = [(x1_arr[i], x2_arr[i], x3_arr[i].decode()) for i in range(num_elements)]
df = pd.DataFrame(data, columns=["x1", "x2", "x3"])
print(df)