import ctypes
import pandas as pd

example = ctypes.cdll.LoadLibrary("C:/Program Files/PTV Vision/PTV Vissim 2023/API/DriverModel_DLL/x64/Debug/DriverModel.dll")

length = example.get_data_length
length.restype = ctypes.c_int

vector_length = length()

print(f"vector_length: {vector_length}")