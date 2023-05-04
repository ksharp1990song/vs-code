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
    PyObject* pmod = PyImport_ImportModule("Xtest");
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
        auto int_list = extractDatafromPython<vector<variant<long,float,double,string>>>("int_list");
        for(auto& i : int_list) {
            visit([](auto&& arg) {
                using T = decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, long>) {
                    cout << "The element is an int: " << arg << endl;
                }
                else if constexpr (std::is_same_v<T, float>) {
                    cout << "The element is a float: " << arg << endl;
                }
                else if constexpr (std::is_same_v<T, double>) {
                    cout << "The element is a double: " << arg << endl;
                }
                else if constexpr (std::is_same_v<T, string>) {
                    cout << "The element is a string: " << arg << endl;
                }
            }, i);
        }
    }
    catch(const runtime_error& e) {
        cout << e. what() << endl;
    }
    return 0;
}