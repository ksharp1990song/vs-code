#include <Python.h>
#include <vector>
#include <string>
#include <iostream>
#include <variant>
#include <type_traits>
#include <stdexcept>
#include <numpy/arrayobject.h>
#include <numpy/ndarrayobject.h>
using namespace std;

int main() {
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

    // Extracting the DataFrame from the script
    PyObject* nparr1 = PyObject_GetAttrString(module, "nparr1");
    if(nparr1 == NULL)
        cout << "Error: nparr1 variable not found" << endl;

    void *data = PyArray_DATA(nparr1);

    int rows = PyArray_DIM(nparr1,0);
    int cols = PyArray_DIM(nparr1,1);

    vector<variant<long,double,string>> arr_data;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            PyObject **obj_data = (PyObject **)data;
            PyObject *obj = obj_data[i+j*rows];
            if(PyUnicode_Check(obj)) {
                PyObject *str = PyUnicode_AsUTF8String(obj);
                arr_data.push_back(string(PyBytes_AsString(str)));
                Py_DECREF(str);
            } else if(PyLong_Check(obj)) {
                arr_data.push_back(PyLong_AsLong(obj));
            } else if(PyFloat_Check(obj)) {
                arr_data.push_back(PyFloat_AsDouble(obj));
            }
        }
    }

    for(const auto &t : arr_data) {
        if(auto i = get_if<long>(&t)) {
            cout << *i << " ";
        } else if(auto f = get_if<double>(&t)) {
            cout << *f << " ";
        } else if(auto s = get_if<string>(&t)) {
            cout << *s << " ";
        }
        
    }
    cout << endl;

    // Don't forget to release the memory used by the PyObject pointers
    Py_DECREF(module);
    Py_XDECREF(nparr1);
    Py_Finalize();
    return 0;
}