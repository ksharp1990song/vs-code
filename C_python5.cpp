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

    PyObject* module = PyImport_ImportModule("test7");
    if(!module) {
        PyErr_Print();
        return 1;
    }

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

    PyObject* pIndex = Py_BuildValue("[i]", 0);

    PyObject* pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, dict);
    PyObject* pKwargs = PyDict_New();
    PyDict_SetItemString(pKwargs, "index", pIndex);
    PyObject* data_frame = PyObject_Call(pd, pArgs, pKwargs);
    if(!data_frame) {
        PyErr_Print();
        return 1;
    }
    PyObject* pGlobals = PyModule_GetDict(module);
    PyDict_SetItemString(pGlobals, "df", data_frame);

    PyObject* pFunc = PyObject_GetAttrString(module, "print_df");
    if(!pFunc) {
        PyErr_Print();
        return 1;
    }
    PyObject* pValue = PyObject_CallObject(pFunc, NULL);
    if(!pValue) {
        PyErr_Print();
        return 1;
    }

    // Clean up
    Py_DECREF(pValue);
    Py_DECREF(pFunc);
    Py_DECREF(data_frame);
    Py_DECREF(pArgs);
    Py_DECREF(pKwargs);
    Py_DECREF(dict);
    Py_DECREF(pIndex);
    Py_DECREF(pd);
    Py_DECREF(pandas);
    Py_DECREF(module);

    // Finalizing the Python interpreter
    Py_Finalize();

    return 0;
}