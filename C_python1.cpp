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
    long x1 = 1;
    double x2 = 3.14;
    vector<long> x1_values;
    vector<double> x2_values;

    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    // Initializing the Python interpreter
    Py_Initialize();

    if (_import_array() < 0)
    {
        throw std::runtime_error("Failed to import NumPy array.");
    }

    PyObject* pModule = PyImport_ImportModule("test4");

    PyObject* pDf = PyObject_GetAttrString(pModule, "df");

    npy_intp dims[] = {0, 2};
    PyObject* pArray = PyArray_SimpleNew(2, dims, NPY_OBJECT);

    chrono::milliseconds interval(1000);
    auto start = chrono::steady_clock::now();
    int count = 0;
    while(true) {
        auto now = chrono::steady_clock::now();

        if (now-start >= interval && count < 10) {
            x1 += 1;
            x2 *= 2;
            cout << "x1: " << x1 << ", x2: " << x2 << endl;
            vector<variant<long, double>> row = {x1, x2};

            PyArray_Dims new_dims;
            new_dims.len = 2;
            new_dims.ptr = dims;
            PyArray_Resize((PyArrayObject*)pArray, &new_dims, 1, NPY_CORDER);

            *(long*)PyArray_GETPTR2((PyArrayObject*)pArray, count, 0) = x1;
            *(double*)PyArray_GETPTR2((PyArrayObject*)pArray, count, 1) = x2;

            PyObject* pFunc = PyObject_GetAttrString(pDf, "append");
            if(pFunc && PyCallable_Check(pFunc)) 
            {
                PyObject* pArgs = PyTuple_New(1);
                PyTuple_SetItem(pArgs, 0, pArray);
                PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
                Py_DECREF(pArgs);
                Py_DECREF(pResult);
            }
            else
            {
                cerr << "Cannot find function" << endl;
                break;
            }
            //PyObject* pArgs = PyTuple_Pack(1, pArray);
            //PyObject_CallObject(pFunc, pArgs);
            
            //Py_DECREF(pFunc);
            //Py_DECREF(pArgs);
            
            count++;
            start = now;
        }
    }
    Py_DECREF(pArray);
    Py_DECREF(pDf);
    Py_DECREF(pModule);
    Py_Finalize();
    return 0;
}