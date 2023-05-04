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
T convertPythonTo(const string& pyName) {
    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    // Initializing the Python interpreter
    Py_Initialize();
    PyObject* numpy_module = PyImport_ImportModule("numpy");
    PyObject* numpy_array_type = PyObject_GetAttrString(numpy_module, "array");
    PyObject* pandas_module = PyImport_ImportModule("pandas");
    PyObject* pandas_dataframe_type = PyObject_GetAttrString(pandas_module,"DataFrame");

    // Importing the Python script
    PyObject* module = PyImport_ImportModule("test2");
    if(module == NULL)
        cout << "Error: script module not found" << endl;

    // Extracting the DataFrame from the script
    PyObject* obj = PyObject_GetAttrString(module, pyName.c_str());
    if(obj == NULL)
        cout << "Error: nparr1 variable not found" << endl;

    vector<variant<long,double,string>> v;

    if(PyObject_IsInstance(obj,pandas_dataframe_type)) {    
        cout << "this is a pandas dataframe" << endl;
        PyObject* nparr = PyObject_CallFunctionObjArgs(numpy_array_type, obj, NULL);
    
        void *data = PyArray_DATA(nparr);

    // Accessing the NumPy array data and printing the array elements
        size_t rows = PyArray_DIM(nparr,0);
        size_t cols = PyArray_DIM(nparr,1);
        
    // Print out the data
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                PyObject **obj_data = (PyObject **)data;
                PyObject *arr = obj_data[i+j*rows];
                if(PyUnicode_Check(arr)) {
                    PyObject *str = PyUnicode_AsUTF8String(arr);
                    v.push_back(string(PyBytes_AsString(str)));
                    Py_DECREF(str);
                } else if(PyLong_Check(arr)) {
                    v.push_back(PyLong_AsLong(arr));
                } else if(PyFloat_Check(arr)) {
                    v.push_back(PyFloat_AsDouble(arr));
                }
            }
        }
        Py_DECREF(nparr);
    } else if(PyObject_IsInstance(obj,numpy_array_type)){
        cout << "this is a numpy array" << endl;
        void *data = PyArray_DATA(obj);

    // Accessing the NumPy array data and printing the array elements
        size_t rows = PyArray_DIM(obj,0);
        size_t cols = PyArray_DIM(obj,1);
    // Print out the data
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                PyObject **obj_data = (PyObject **)data;
                PyObject *arr = obj_data[i+j*rows];
                if(PyUnicode_Check(arr)) {
                    PyObject *str = PyUnicode_AsUTF8String(arr);
                    v.push_back(string(PyBytes_AsString(str)));
                    Py_DECREF(str);
                } else if(PyLong_Check(arr)) {
                    v.push_back(PyLong_AsLong(arr));
                } else if(PyFloat_Check(arr)) {
                    v.push_back(PyFloat_AsDouble(arr));
                }
            }
        }
    } else {
        cout << "not succeeded" << endl;
    }

    // Don't forget to release the memory used by the PyObject pointers
    Py_DECREF(module);
    Py_DECREF(obj);
    Py_DECREF(numpy_module);
    Py_DECREF(numpy_array_type);
    Py_DECREF(pandas_module);
    Py_DECREF(pandas_dataframe_type);
    Py_Finalize();
    return v;
}

int main() {
    auto result = convertPythonTo<vector<variant<long,double,string>>>("x3");
    for(const auto& i : result) {
        if (auto l = get_if<long>(&i)) {
                cout << *l << endl;
            } else if (auto d = get_if<double>(&i)) {
                cout << *d << endl;
            } else if (auto s = get_if<string>(&i)) {
                cout << *s << endl;
            }
        }
    return 0;
    }

