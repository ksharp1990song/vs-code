#include <Python.h>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>
#include <variant>
#include <cstdio>
#include <numpy/arrayobject.h>
#include <numpy/ndarrayobject.h>
#include <numpy/ndarraytypes.h>
using namespace std;

template <typename T>
T convertPythonTo1(const string& moduleName,const string& varName) {
    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    // Initializing the Python interpreter
    Py_Initialize();

    // Importing the Python script
    PyObject* module = PyImport_ImportModule(moduleName.c_str());
    if(module == NULL) {
        cout << "Error: script module not found" << endl;
        return T();
    }

    // Extracting the DataFrame from the script
    PyObject* obj = PyObject_GetAttrString(module, varName.c_str());
    if(obj == NULL) {
        cout << "Error: nparr1 variable not found" << endl;
        return T();
    }    

    T result;
    if constexpr (is_same<T, long>::value) {
        result = PyLong_AsLong(obj);
    } else if constexpr (is_same<T, float>::value) {
        result = PyFloat_AsDouble(obj);
    } else {
        throw runtime_error("Unsupported python data type ");
        return T();
    }

    // Don't forget to release the memory used by the PyObject pointers
    Py_DECREF(module);
    Py_DECREF(obj);
    Py_Finalize();
    return result;
}

int main() {
    auto result = convertPythonTo1<float>("test2","x3");
    cout << result << endl;
    return 0;
}





