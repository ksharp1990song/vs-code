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

    // Importing the numpy module
    //PyObject* numpy = PyImport_Import(PyUnicode_FromString("numpy"));

    // Importing the Python script
    PyObject* script = PyImport_Import(PyUnicode_FromString("test1"));

    // Extracting the DataFrame from the script
    PyObject* df = PyObject_GetAttrString(script, "df");

    // Converting the DataFrame to a NumPy array
    PyObject* np_array = PyArray_FromAny(df, NULL, 0, 0, NPY_ARRAY_ENSURECOPY, NULL);

    PyObject* list =PyObject_CallMethod(np_array,"tolist",NULL);
    PyObject_Print(list,stdout,0);

    // Accessing the NumPy array data and printing the array elements
    int nd = PyArray_NDIM(np_array);
    npy_intp* dims = PyArray_DIMS(np_array);
    long* data = (long*)PyArray_DATA(np_array);
    std::cout << "NumPy array elements: " << std::endl;
    for (int i = 0; i < dims[0]; i++) {
        for (int j = 0; j < dims[1]; j++) {
            std::cout << *(data + i*dims[1] + j) << " ";
        }
        std::cout << std::endl;
    }

    // Don't forget to release the memory used by the PyObject pointers
    //Py_DECREF(numpy);
    Py_DECREF(script);
    Py_XDECREF(df);
    Py_XDECREF(np_array);
    Py_Finalize();
    return 0;
}