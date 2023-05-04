#include <Python.h>
#include <vector>
#include <string>
#include <iostream>
#include <variant>
#include <numpy/arrayobject.h>
#include <numpy/ndarrayobject.h>
using namespace std;


template <typename T>
T convertarraytoC(PyObject* obj) {
    if constexpr (is_same<T, vector<variant<long,double,string>>>::value) {
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
}

template <typename T>
T extractdatafromPython(const string& varName) {
    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    // Initializing the Python interpreter
    Py_Initialize();

    // Importing the Python script
    PyObject* module = PyImport_ImportModule("test2");
    if(module == NULL)
        cout << "Error: script module not found" << endl;
        return T();

    // Extracting the DataFrame from the script
    PyObject* nparr1 = PyObject_GetAttrString(module, varName.c_str());
    if(nparr1 == NULL)
        cout << "Error: nparr1 variable not found" << endl;
        return T();

    T var = convertarraytoC<T>(nparr1);

    // Don't forget to release the memory used by the PyObject pointers
    Py_DECREF(module);
    Py_XDECREF(nparr1);
    Py_Finalize();
    return var;
}

int main() {
    try{
        auto arr = extractdatafromPython<vector<variant<long,double,string>>>("nparr1");
        for(const auto &t : arr) {
            if(auto i = get_if<long>(&t)) {
                cout << *i << " ";
            } else if(auto f = get_if<double>(&t)) {
                cout << *f << " ";
            } else if(auto s = get_if<string>(&t)) {
                cout << *s << " ";
            }
            
        }
        cout << endl;
    } 
    catch(const runtime_error& e) {
        cout << e. what() << endl;
    }
    return 0;
}