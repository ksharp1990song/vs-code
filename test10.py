import ctypes
import pandas as pd

example = ctypes.cdll.LoadLibrary("C:/Programme/PTV Vision/PTV Vissim 2023/API/DriverModel_DLL/x64/Debug/DriverModel.dll")

get_data_length = example.get_data_length
get_data_length.restype = ctypes.c_int

get_data = example.get_data
get_data.argtypes = [
    ctypes.POINTER(ctypes.c_int),
]

num_elements = get_data_length()
ids_arr = (ctypes.c_int * num_elements)()

get_data(ids_arr, num_elements)


data = [ids_arr[i] for i in range(num_elements)]
df = pd.DataFrame(data, columns=["ids:"])

print(df)

