#include <Python.h>
#include <iostream>
#include <numpy/arrayobject.h>
#include <numpy/ndarrayobject.h>
#include <numpy/ndarraytypes.h>
using namespace std;


int main() {
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
    PyObject* module = PyImport_ImportModule("test1");
    if(module == NULL)
        cout << "Error: script module not found" << endl;

    // Extracting the DataFrame from the script
    PyObject* df1 = PyObject_GetAttrString(module, "df");
    if(df1 == NULL)
        cout << "Error: nparr1 variable not found" << endl;

    if(PyObject_IsInstance(df1,pandas_dataframe_type)) {    
        cout << "this is a pandas dataframe" << endl;
        PyObject* nparr1 = PyObject_CallFunctionObjArgs(numpy_array_type, df1, NULL);
    
        void *data = PyArray_DATA(nparr1);

    // Accessing the NumPy array data and printing the array elements
        size_t rows = PyArray_DIM(nparr1,0);
        size_t cols = PyArray_DIM(nparr1,1);
    
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
        Py_DECREF(nparr1);
    } else if(PyObject_IsInstance(df1,numpy_array_type)){
        cout << "this is a numpy array" << endl;
        void *data = PyArray_DATA(df1);

    // Accessing the NumPy array data and printing the array elements
        int rows = PyArray_DIM(df1,0);
        int cols = PyArray_DIM(df1,1);
    
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
    } else {
        cout << "not succeeded" << endl;
    }

    
    


    // Don't forget to release the memory used by the PyObject pointers
    Py_DECREF(module);
    Py_DECREF(df1);
    Py_DECREF(numpy_module);
    Py_DECREF(numpy_array_type);
    Py_DECREF(pandas_module);
    Py_DECREF(pandas_dataframe_type);
    Py_Finalize();
    return 0;
}