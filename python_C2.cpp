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
using namespace std;

template <typename T>
    // convert all datatypes from python into c++
T convertPyObjectTo(PyObject* obj)
{
    // check if the extract data is a int
    if constexpr (is_same<T, int>::value) {      
        return (int)PyLong_AsLong(obj);
    }
    // check if the extract data is a float
    else if constexpr (is_same<T, float>::value) {
        return (float)PyFloat_AsDouble(obj);
    }
    // check if the extract data is a double
    else if constexpr (is_same<T, double>::value) {
        return (double)PyFloat_AsDouble(obj);
    }
    // check if the extract data is a string
    else if constexpr (is_same<T, string>::value) {
        return PyUnicode_AsUTF8(obj);
    }
    // check if the extract data is in a list
    else if constexpr (is_same<T, vector<variant<long,float,double,string>>>::value) {
        if(!PyList_Check(obj)) {
            throw runtime_error("Object is not a list"); 
        }
        int len = PyList_Size(obj);
        vector<variant<long,float,double,string>> v;
        for(int i=0; i<len; i++) {
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
        return v;
    }
    // check if the extract data is in a dictionary
    else if constexpr (is_same<T, map<string,int>>::value) {
        if(!PyDict_Check(obj)) {
            throw runtime_error("Object is not a dictionary");
        }
        PyObject *key, *value;
        Py_ssize_t pos = 0;
        map<string,int> m;
        while(PyDict_Next(obj, &pos, &key, &value)) {
            string keystr = PyUnicode_AsUTF8(key);
            int val = PyLong_AsLong(value);
            m[keystr] = val;
        }
        return m;
    }
    // check if the extract data is in a tuple
    else if constexpr (is_same<T, vector<variant<long,float,double,string>>>::value) {
        if(!PyTuple_Check(obj)) {
            throw runtime_error("Object is not a tuple");
        }
        int len = PyTuple_Size(obj);
        vector<variant<long,float,double,string>> v;
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
                throw runtime_error("Unsupported data type in the tuple");
            }
        }
        return v;
    }
    // check of the extract data is in an numpy array
    else if constexpr (is_same<T, vector<variant<long,double,string>>>::value) {
        if(!PyArray_Check(obj)) {
            throw runtime_error("Object is not a numpy array");
        }
        void* data = PyArray_DATA(obj);
        int rows = PyArray_DIM(obj,0);
        int cols = PyArray_DIM(obj,1);
        vector<variant<long,double,string>> v;
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                PyObject **obj_data = (PyObject **)data;
                PyObject *obj_arr = obj_data[i+j*rows];
                if(PyUnicode_Check(obj_arr)) {
                    PyObject* str = PyUnicode_AsUTF8String(obj_arr);
                    v.push_back(string(PyBytes_AsString(str)));
                    Py_DECREF(str);
                } else if(PyLong_Check(obj_arr)) {
                    v.push_back(PyLong_AsLong(obj_arr));
                } else if(PyFloat_AsDouble(obj_arr)) {
                    v.push_back(PyLong_AsLong(obj_arr));
                } else {
                    throw runtime_error("Unsupported numpy array data type");
                }
            }
        }
        return v;
    }
    // Umsupported data type
    else {
        throw runtime_error("Unsupported data type in convertPyObjectTo");
    }
}

template <typename T>

T extractDatafromPython(const string& varName)
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
        return T();
    }

    // Import the script as a module
    PyObject* pmod = PyImport_ImportModule("test2");
    if (pmod == NULL) {
        cerr << "Error: module not found";
        return T();
    }

    // Extract the values from python
    PyObject* var_obj = PyObject_GetAttrString(pmod, varName.c_str());
    if (var_obj == NULL) {
        cerr << "Error: varName not found in module." << endl;
        return T();
    }

    // Define variables to receive the extracted data
    T var = convertPyObjectTo<T>(var_obj);

    // Clean up
    Py_DECREF(pmod);
    Py_DECREF(var_obj);
    // Finalize the interpreter
    Py_Finalize();

    return var;
}

int main() 
{
    try{
        auto nparr1 = extractDatafromPython<vector<variant<long,float,double,string>>>("x4");
        for(const auto &t : nparr1) {
            if(auto i = get_if<long>(&t)) {
                cout << *i << " ";
            } else if(auto f = get_if<double>(&t)) {
                cout << *f << " ";
            } else if(auto s = get_if<string>(&t)) {
                cout << *s << " ";
            }
            cout << endl;
        }
        cout << endl;
    }   
    catch(const runtime_error& e) {
        cout << e. what() << endl;
    }
    return 0;
}