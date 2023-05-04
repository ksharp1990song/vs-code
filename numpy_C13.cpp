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


vector<variant<long,double,string,map<string,int>>> convertPythonTo(const string& moduleName,const string& varName) {
    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    vector<variant<long,double,string,map<string,int>>> v;

    // Initializing the Python interpreter
    Py_Initialize();
    PyObject* numpy_module = PyImport_ImportModule("numpy");
    PyObject* numpy_array_type = PyObject_GetAttrString(numpy_module, "array");
    PyObject* pandas_module = PyImport_ImportModule("pandas");
    PyObject* pandas_dataframe_type = PyObject_GetAttrString(pandas_module,"DataFrame");

    // Importing the Python script
    PyObject* module = PyImport_ImportModule(moduleName.c_str());
    if(module == NULL) {
        cout << "Error: script module not found" << endl;
        return v;
    }

    // Extracting the DataFrame from the script
    PyObject* obj = PyObject_GetAttrString(module, varName.c_str());
    if(obj == NULL) {
        cout << "Error: nparr1 variable not found" << endl;
        return v;
    }    

    if(PyLong_Check(obj)) {
        v.push_back(PyLong_AsLong(obj));
    } else if(PyFloat_Check(obj)) {
        v.push_back(PyFloat_AsDouble(obj));
    } else if(PyUnicode_Check(obj)) {
        v.push_back(PyUnicode_AsUTF8(obj));
    } else if(PyList_Check(obj)) {
        size_t len = PyList_Size(obj);
        for(int i = 0; i < len; i++) {
            auto pyobj = PyList_GetItem(obj,i);
            if(PyLong_Check(pyobj)) {
                v.emplace_back(PyLong_AsLong(pyobj));
            }
            else if(PyFloat_Check(pyobj)) {
                v.emplace_back(PyFloat_AsDouble(pyobj));
            }
            else if(PyUnicode_Check(pyobj)) {
                v.emplace_back(PyUnicode_AsUTF8(pyobj));
            }
            else {
                throw runtime_error("Unsupported data type in the list");
            }
        }    
    } else if(PyDict_Check(obj)) {
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        map<string,int> m;
        while(PyDict_Next(obj, &pos, &key, &value)) {
            string keystr = PyUnicode_AsUTF8(key);
            int val = PyLong_AsLong(value);
            m[keystr] = val;
            v.push_back(m);
        }
    } else if(PyTuple_Check(obj)) {
        size_t len = PyTuple_Size(obj);
        for(int i = 0; i < len; i++) {
            auto pyobj = PyTuple_GetItem(obj,i);
            if(PyLong_Check(pyobj)) {
                v.emplace_back(PyLong_AsLong(pyobj));
            }
            else if(PyFloat_Check(pyobj)) {
                v.emplace_back(PyFloat_AsDouble(pyobj));
            }
            else if(PyUnicode_Check(pyobj)) {
                v.emplace_back(PyUnicode_AsUTF8(pyobj));
            }
            else {
                throw runtime_error("Unsupported data type in the list");
            }
        }
    }
    else if(PyObject_IsInstance(obj,pandas_dataframe_type)) {  
        cout << "dataframe succeed loaded" << endl; 
    //  Transfer a dataframe into numpy array    
        PyObject* nparr = PyObject_CallFunctionObjArgs(numpy_array_type, obj, NULL);
    // Accessing the NumPy array data and printing the array elements
        size_t rows = PyArray_DIM(nparr,0);
        size_t cols = PyArray_DIM(nparr,1);
    // Print out the data
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                PyObject *arr = PyArray_GETITEM(nparr, (char*)PyArray_GETPTR2(nparr,i,j));
                if(PyUnicode_Check(arr)) {
                    v.push_back(PyUnicode_AsUTF8(arr));
                } else if(PyLong_Check(arr)) {
                    v.push_back(PyLong_AsLong(arr));
                } else if(PyFloat_Check(arr)) {
                    v.push_back(PyFloat_AsDouble(arr));
                }
            }
        }
        Py_DECREF(nparr);
    } else if(PyObject_IsInstance(obj,numpy_array_type)){
        cout << "succeed loaded" << endl;
    // Accessing the NumPy array data and printing the array elements
        size_t rows = PyArray_DIM(obj,0);
        size_t cols = PyArray_DIM(obj,1);
    // Print out the data
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                PyObject *arr = PyArray_GETITEM(obj, (char*)PyArray_GETPTR2(obj,i,j));
                if(PyUnicode_Check(arr)) {
                    v.push_back(PyUnicode_AsUTF8(arr));
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
    vector<variant<long,double,string,map<string,int>>> result = convertPythonTo("test2","x2");
    for(const auto& i : result) {
        if (auto l = get_if<long>(&i)) {
            cout << *l << endl;
        } else if (auto d = get_if<double>(&i)) {
            cout << *d << endl;
        } else if (auto s = get_if<string>(&i)) {
            cout << *s << endl;
        } else if (auto m = get_if<map<string, int>>(&i)) {
            for(const auto& kv : *m) {
                string str = kv.first;
                int val = kv.second;
                cout << str << ":" << val << endl;
            }
        }
    }
    
    return 0;
    }





