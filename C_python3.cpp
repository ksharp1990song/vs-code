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
#include <chrono>
using namespace std;

int main() 
{
    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    // Initializing the Python interpreter
    Py_Initialize();

    PyObject* pandas = PyImport_ImportModule("pandas");
    if(!pandas) {
        PyErr_Print();
        return 1;
    }

    PyObject* pd = PyObject_GetAttrString(pandas, "DataFrame");
    if(!pd) {
        PyErr_Print();
        return 1;
    }

    int x1 = 1;
    double x2 = 3.14;
    PyObject* dict = PyDict_New();
    PyDict_SetItemString(dict, "x1", PyLong_FromLong(x1));
    PyDict_SetItemString(dict, "x2", PyFloat_FromDouble(x2));

    PyObject* args_df = PyTuple_New(1);
    PyTuple_SetItem(args_df, 0, dict);
    PyObject* data_frame = PyObject_CallObject(pd, args_df);
    if(!data_frame) {
        PyErr_Print();
        return 1;
    }
    Py_DECREF(args_df);

    PyObject* module = PyImport_ImportModule("test5");
    if(!module) {
        PyErr_Print();
        return 1;
    }
    
    PyObject* process_dataframe_func = PyObject_GetAttrString(module, "process_dataframe");
    if(!process_dataframe_func) {
        PyErr_Print();
        return 1;
    }

    PyObject* args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, data_frame);
    PyObject* result = PyObject_CallObject(process_dataframe_func, args);

    Py_DECREF(args);
    Py_DECREF(result);
    Py_DECREF(process_dataframe_func);
    Py_DECREF(module);
    Py_DECREF(data_frame);
    Py_DECREF(pd);
    Py_DECREF(pandas);
    Py_DECREF(dict);

    Py_Finalize();
    return 0;
}