#include <Python.h>
#include <iostream>
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
    PyObject* module = PyImport_ImportModule("test1");
    if(module == NULL)
        cout << "Error: script module not found" << endl;

    // Extracting the DataFrame from the script
    PyObject* nparr = PyObject_GetAttrString(module, "nparr");
    if(nparr == NULL)
        cout << "Error: nparr variable not found" << endl;

    void *data = PyArray_DATA(nparr);
    


    // Accessing the NumPy array data and printing the array elements
    int rows = PyArray_DIM(nparr,0);
    int cols = PyArray_DIM(nparr,1);
    
    // Print out the data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            PyObject **obj_data = (PyObject **)data;
            PyObject *obj = obj_data[i+j*rows];
            if(PyUnicode_Check(obj)) {
                PyObject *str = PyUnicode_AsUTF8String(obj);
                cout << PyBytes_AsString(str) << " ";
                Py_DECREF(str);
            } else if(PyLong_Check(obj)) {
                cout << PyLong_AsLong(obj) << " ";
            } else if(PyFloat_Check(obj)) {
                cout << PyFloat_AsDouble(obj) << " ";
            }
        }
        cout << endl;
    }
        

    // Don't forget to release the memory used by the PyObject pointers
    Py_DECREF(module);
    Py_XDECREF(nparr);
    Py_Finalize();
    return 0;
}