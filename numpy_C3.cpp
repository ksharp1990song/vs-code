#include <Python.h>
#include <iostream>
#include <numpy/arrayobject.h>
#include <numpy/ndarrayobject.h>
#include <typeinfo>
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
    PyObject* nparr1 = PyObject_GetAttrString(module, "x5");
    if(nparr1 == NULL)
        cout << "Error: nparr1 variable not found" << endl;

    
    
    void *data = PyArray_DATA(nparr1);
    


    // Accessing the NumPy array data and printing the array elements
    int rows = PyArray_DIM(nparr1,0);
    int cols = PyArray_DIM(nparr1,1);
    
    // Print out the data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            PyObject **obj_data = (PyObject **)data;
            PyObject *obj = obj_data[i+j*rows];
            if(PyLong_Check(obj)) {
                cout << "Long check succeed" << " ";
                cout << PyLong_AsLong(obj) << " ";
                cout << typeid(PyLong_AsLong(obj)).name() << " ";
            } else if(PyFloat_Check(obj)) {
                cout << "Float check succeed" << " ";
                cout << PyFloat_AsDouble(obj) << " ";
                cout << typeid(PyFloat_AsDouble(obj)).name() << " ";
            } else if(PyUnicode_Check(obj)) {
                cout << "String check succeed" << " ";
                cout << PyUnicode_AsUTF8(obj) << " ";
                cout << typeid(PyUnicode_AsUTF8(obj)).name() << " ";
            } 
        }
        cout << endl;
    }

    // Don't forget to release the memory used by the PyObject pointers
    Py_DECREF(module);
    Py_XDECREF(nparr1);
    Py_Finalize();
    return 0;
}