#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/embed.h>
#include <Python.h>
#include <iostream>

namespace py = pybind11;


py::dict extract_dataframe(std::string module_name) {
    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");
    
    py::scoped_interpreter guard{};
    py::module_ module = py::module_::import(module_name.c_str());
    py::object dataframe = module.attr("df");
    return dataframe.cast<py::dict>();
}

int main() {
    // Extract the dataframe from Python
    py::dict dataframe = extract_dataframe("Xtest");
    // Print the dataframe
    std::cout << dataframe << std::endl;
    return 0;
}