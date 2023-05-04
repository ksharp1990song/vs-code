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

int main() {
    size_t x1 = 1;
    double x2 = 3.14;

    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    // Initializing the Python interpreter
    Py_Initialize();

    PyObject* module = PyImport_ImportModule("test4");
    PyObject* pFunc = PyObject_GetAttrString(module, "CtoPython");

    npy_intp dims[2] = {0, 2};
    PyObject* pArray = PyArray_SimpleNew(2, dims, NPY_OBJECT);

    chrono::milliseconds interval(1000);

    auto start = chrono::steady_clock::now();

    int count = 0;
    while(true)
    {
        auto now = chrono::steady_clock::now();

        if(now-start >= interval && count < 10) {
            x1 += 1;
            x2 *= 2;
            count++;
            
            PyObject* pX1 = PyLong_FromLong(x1);
            PyObject* pX2 = PyFloat_FromDouble(x2);

            PyArray_SETITEM(pArray, PyArray_GETPTR2((PyArrayObject*)pArray, count - 1, 0), pX1);
            PyArray_SETITEM(pArray, PyArray_GETPTR2((PyArrayObject*)pArray, count - 1, 1), pX2);

            Py_DECREF(pX1);
            Py_DECREF(pX2);


            start = now;        
        }

    }

    Py_DECREF(pArray);
    Py_DECREF(pFunc);
    Py_DECREF(module);

    Py_Finalize();
    return 0;
}