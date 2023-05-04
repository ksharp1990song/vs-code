#include <Python.h>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>
#include <variant>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>
#include <pybind11/embed.h>
using namespace std;
namespace py = pybind11;

template <typename T>
T convertPyObjectTo(py::object obj) {
    // check if the extract data is a DataFrame
    if constexpr (std::is_same<T, py::object>::value) {
        // check if the passed object is a DataFrame
        if(py::isinstance(obj, py::module::import("pandas").attr("DataFrame"))) 
        {
            // return the dataframe as a pandas DataFrame
            return obj;
        } 
        else 
        {
            throw std::runtime_error("Object is not a DataFrame");
        }
    }
}

int main() {

    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    py::scoped_interpreter guard{};

    py::module_ script = py::module_::import("Xtest");

    py::object df = script.attr("df");

    // pass the DataFrame to the convertPyObjectTo function
    py::object dataframe = convertPyObjectTo<py::object>(df);
    // use the returned DataFrame as a pandas DataFrame
    cout << dataframe << endl;
    return 0;
}
