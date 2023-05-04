import ctypes
import pandas as pd

example = ctypes.cdll.LoadLibrary("C:/Programme/PTV Vision/PTV Vissim 2023/API/DriverModel_DLL/x64/Debug/DriverModel.dll")

get_data_length = example.get_data_length
get_data_length.restype = ctypes.c_int

get_data = example.get_data
get_data.argtypes = [
    ctypes.POINTER(ctypes.c_int),
    ctypes.c_int
]

num_elements = 10
ids_arr = (ctypes.c_int * num_elements)()

get_data(ids_arr, num_elements)

x2_list = [x2_arr[i] for i in range(num_elements)]
data = {"x2:" : x2_list}
df = pd.DataFrame(data)

print(df)