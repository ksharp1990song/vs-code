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

    PyObject* module = PyImport_ImportModule("test5");
    if(!module) {
        PyErr_Print();
        return 1;
    }

    PyObject* pGlobals = PyModule_GetDict(module);
    if(!pGlobals) {
        PyErr_Print();
        return 1;
    }

    PyObject* pDf = PyDict_GetItemString(pGlobals, "df");
    if(!pDf) {
        PyErr_Print();
        return 1;
    }

    PyObject* pandas = PyImport_ImportModule("pandas");
    if(!pandas) {
        PyErr_Print();
        return 1;
    }

    int x1 = 1;
    double x2 = 3.14;
    PyObject* dict = Py_BuildValue("{s: [i], s: [d]}", "x1", x1, "x2", x2);
    PyObject* pIndex = Py_BuildValue("[i]", 0);
    PyObject* pd = PyObject_GetAttrString(pandas, "DataFrame");
    if(!pd) {
        PyErr_Print();
        return 1;
    }
    PyObject* pDataFunc = PyObject_CallFunctionObjArgs(pd, dict, pIndex, NULL);
    if(!pDataFunc) {
        PyErr_Print();
        return 1;
    }
    Py_DECREF(pd);
    Py_DECREF(dict);
    Py_DECREF(pIndex);

    PyObject* pAppend = PyObject_GetAttrString(pDf, "append");
    if(!pAppend) {
        PyErr_Print();
        return 1;
    }

    PyObject* pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, pDataFunc);
    PyObject* pResult = PyObject_CallObject(pAppend, pArgs);
    if(!pResult) {
        PyErr_Print();
        return 1;
    }
    Py_DECREF(pAppend);
    Py_DECREF(pArgs);
    Py_DECREF(pDf);

    PyDict_SetItemString(pGlobals, "df", pResult);

    Py_DECREF(pResult);
    Py_DECREF(module);
    Py_DECREF(pandas);

    // Finalizing the Python interpreter
    Py_Finalize();

    return 0;
}