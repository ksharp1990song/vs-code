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

void update_df(long x1, double x2)
{
    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    // Initializing the Python interpreter
    Py_Initialize();

    PyObject *pModule = PyImport_ImportModule("test4");

    if(pModule != nullptr)
    {
        PyObject* pFunc = PyObject_GetAttrString(pModule, "append_to_df");
        if(pFunc && PyCallable_Check(pFunc))
        {
            PyObject* pArgs = PyTuple_New(2);
            PyTuple_SetItem(pArgs, 0, PyLong_FromLong(x1));
            PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(x2));

            PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
            Py_XDECREF(pResult);
            Py_DECREF(pArgs);
        }
        Py_XDECREF(pFunc);
    }
    Py_XDECREF(pModule);
    Py_Finalize();
}

int main()
{
    
    long x1 = 1;
    double x2 = 3.14;

    chrono::milliseconds interval(1000);
    auto start = chrono::steady_clock::now();

    int count = 0;
    while (true)
    {
        auto now = chrono::steady_clock::now();

        if (now - start >= interval && count < 10)
        {
            x1 += 1;
            x2 *= 2;
            count++;
            cout << "Count: " << count << ", x1 = " << x1 << ", x2 = " << x2 << endl;
            update_df(x1, x2);
            start = now;
            if(count >= 10) {
                break;
            }
        } 
    }
    return 0;
}