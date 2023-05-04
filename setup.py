from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
import sys

sys.argv.append("build_ext")

ext_modules = [
    Extension("driver_data",
              sources=["driver_data.pyx"],
              language="c++",
              include_dirs=["C:/Programme/PTV Vision/PTV Vissim 2023/API/DriverModel_DLL/","C:/Users/uk093878/Anaconda3/Lib/site-packages/numpy/core/include/"],
              extra_compile_args=["-std=c++17"],
              extra_link_args=["-std=c++17"],
              )
]

setup(
    name="driver_data",
    ext_modules=cythonize(ext_modules),
)
