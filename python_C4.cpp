#include <Python.h>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>
#include <variant>
#include <numpy/arrayobject.h>
#include <numpy/ndarrayobject.h>
#include <numpy/ndarraytypes.h>
using namespace std;

int main() 
{
     // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    // Initialize the Python interpreter
    Py_Initialize();
    if (!Py_IsInitialized()) {
        cerr << "Error: Python interpreter is not initialized." << endl;
        return -1;
    }

    // Import numpy and pandas mudule
    PyObject* numpy_module = PyImport_ImportModule("numpy");
    PyObject* numpy_array_type = PyObject_GetAttrString(numpy_module, "array");
    PyObject* pandas_module = PyImport_ImportModule("pandas");
    PyObject* pandas_dataframe_type = PyObject_GetAttrString(pandas_module,"DataFrame");

    // Import the script as a module
    PyObject* pmod = PyImport_ImportModule("test2");
    if (pmod == NULL) {
        cerr << "Error: module not found" << endl;
        return -1;
    }

    // Extract the values from python
    PyObject* var_obj = PyObject_GetAttrString(pmod, "x1");
    if (var_obj == NULL) {
        cerr << "Error: varName not found in module." << endl;
        return -1;
    }

    // Define variables to receive the extracted data
    if(PyLong_Check(var_obj)) {
        cout << PyLong_AsLong(var_obj) << endl;
    } else if(PyFloat_Check(var_obj)) {
        cout << PyFloat_AsDouble(var_obj) << endl;
    } else if(PyUnicode_Check(var_obj)) {
        cout << PyUnicode_AsUTF8String(var_obj) << endl;
    } else if(PyList_Check(var_obj)) {
        int len = PyList_Size(var_obj);
        for(int i=0; i<len; i++) {
            auto pyobj = PyList_GetItem(var_obj,i);
            if(PyLong_Check(pyobj)) {
                cout << PyLong_AsLong(pyobj) << " ";
            }
            else if(PyFloat_Check(pyobj)) {
                cout << PyFloat_AsDouble(pyobj) << " ";
            }
            else if(PyUnicode_Check(pyobj)) {
                cout << PyUnicode_AsUTF8(pyobj) << " ";
            }
            else {
                throw runtime_error("Unsupported data type in the list");
            }
        }
        cout << endl;
    } else if(PyDict_Check(var_obj)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        while(PyDict_Next(var_obj, &pos, &key, &value)) {
            PyObject *str = PyUnicode_AsUTF8String(key);
            cout << PyBytes_AsString(str) << ":" << PyLong_AsLong(value) << endl;
            Py_DECREF(str);
        }
        cout << endl;
    } else if(PyTuple_Check(var_obj)) {
        int len = PyTuple_Size(var_obj);
        for(int i = 0; i < len; i++) {
            auto pyobj = PyTuple_GetItem(var_obj,i);
            if(PyLong_Check(pyobj)) {
                cout << PyLong_AsLong(pyobj) << " ";
            }
            else if(PyFloat_Check(pyobj)) {
                cout << PyFloat_AsDouble(pyobj) << " ";
            }
            else if(PyUnicode_Check(pyobj)) {
                cout << PyUnicode_AsUTF8(pyobj) << " ";
            }
            else {
                throw runtime_error("Unsupported data type in the tuple");
            }
        }
        cout << endl;
    } else if(PyObject_IsInstance(var_obj,pandas_dataframe_type)){
        cout << "this is a pandas dataframe" << endl; 
        PyObject* nparr1 = PyObject_CallFunctionObjArgs(numpy_array_type, var_obj, NULL);
        void *data = PyArray_DATA(nparr1);
        int rows = PyArray_DIM(nparr1,0);
        int cols = PyArray_DIM(nparr1,1);
        cout << rows << "," << cols << endl;
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                PyObject **obj_data = (PyObject **)data;
                PyObject *arr = obj_data[i+j*rows];
                if(PyUnicode_Check(arr)) {
                    PyObject *str= PyUnicode_AsUTF8String(arr);
                    cout << PyBytes_AsString(str) << " ";
                    Py_DECREF(str);
                } else if(PyLong_Check(arr)) {
                    cout << PyLong_AsLong(arr) << " ";
                } else if(PyFloat_Check(arr)) {
                    cout << PyFloat_AsDouble(arr) << " ";
                } 
            }
            cout << endl;
        }
        Py_DECREF(nparr1);
    } else if (PyObject_IsInstance(var_obj,numpy_array_type)) {
        cout << "this is a numpy array" << endl;
        void *data = PyArray_DATA(var_obj);
        int rows = PyArray_DIM(var_obj,0);
        int cols = PyArray_DIM(var_obj,1);
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                PyObject **obj_data = (PyObject **)data;
                PyObject *arr = obj_data[i+j*rows];
                if(PyUnicode_Check(arr)) {
                    PyObject *str= PyUnicode_AsUTF8String(arr);
                    cout << PyBytes_AsString(str) << " ";
                    Py_DECREF(str);
                } else if(PyLong_Check(arr)) {
                    cout << PyLong_AsLong(arr) << " ";
                } else if(PyFloat_Check(arr)) {
                    cout << PyFloat_AsDouble(arr) << " ";
                } 
            }
            cout << endl;
        }
    } else {
        throw runtime_error("Unsupport data type from python");
    }
    // Clean up
    Py_DECREF(pmod);
    Py_DECREF(var_obj);
    Py_DECREF(numpy_module);
    Py_DECREF(numpy_array_type);
    Py_DECREF(pandas_module);
    Py_DECREF(pandas_dataframe_type);
    // Finalize the interpreter
    Py_Finalize();
    return 0;
}