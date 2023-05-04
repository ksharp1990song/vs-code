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


int main() {
    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    // Initializing the Python interpreter
    Py_Initialize();


    // Importing the Python script
    PyObject* module = PyImport_ImportModule("test3");
    if(module == NULL)
        cout << "Error: script module not found" << endl;

    // Extracting the DataFrame from the script
    PyObject* nparr1 = PyObject_GetAttrString(module, "nparr");
    if(nparr1 == NULL)
        cout << "Error: nparr1 variable not found" << endl;


    // Accessing the NumPy array data and printing the array elements
    int rows = PyArray_DIM(nparr1,0);
    int cols = PyArray_DIM(nparr1,1);
    
    // Print out the data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            PyObject* obj = PyArray_GETITEM(nparr1, (char*)PyArray_GETPTR2(nparr1,i,j));
            if(PyLong_Check(obj)) {
                cout << PyLong_AsLong(obj) << " ";
            } else if (PyFloat_Check(obj)) {
                cout << PyFloat_AsDouble(obj) << " ";
            } else if (PyUnicode_Check(obj)) {
                cout << PyUnicode_AsUTF8(obj) << " ";
            }
        }
        cout << endl;
    }

    // Don't forget to release the memory used by the PyObject pointers
    Py_DECREF(module);
    Py_XDECREF(nparr1);
    Py_Finalize();
    return 0;
}