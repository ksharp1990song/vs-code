#include <iostream>
#include <numpy/arrayobject.h>
#include <Python.h>

int main() {
    // Set PYTHONHOME
    Py_SetPythonHome(L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai");

    // Set PYTHONPATH
    const wchar_t* pythonPath = L"PYTHONPATH";
    _wputenv_s(pythonPath, L"C:\\Users\\uk093878\\Anaconda3\\envs\\kai\\DLLs;C:\\Users\\uk093878\\python");

    // Initializing the Python interpreter
    Py_Initialize();

    //PyObject* numpy = PyImport_Import(PyUnicode_FromString("numpy"));

    // Importing the Python script
    PyObject* script = PyImport_Import(PyUnicode_FromString("Xtest"));

    // Extracting the DataFrame from the script
    PyObject* df = PyObject_GetAttrString(script, "df");

    // Converting the DataFrame to a NumPy array
    PyObject* np_array = PyArray_FromAny(df, NULL, 0, 0, NPY_ARRAY_ENSURECOPY, NULL);

    // Accessing the NumPy array data and printing the array elements
    int nd = PyArray_NDIM(np_array);
    npy_intp* dims = PyArray_DIMS(np_array);
    int data_type = PyArray_TYPE(np_array);
    void* data = PyArray_DATA(np_array);
    std::cout << "NumPy array elements: " << std::endl;
    for (int i = 0; i < dims[0]; i++) {
        for (int j = 0; j < dims[1]; j++) {
            if (data_type == NPY_LONG) {
                long* long_data = (long*)data;
                std::cout << *(long_data + i*dims[1] + j) << " ";
            }
            else if (data_type == NPY_DOUBLE) {
                double* double_data = (double*)data;
                std::cout << *(double_data + i*dims[1] + j) << " ";
            }
            else if (data_type == NPY_STRING) {
                char* string_data = (char*)data;
                std::cout << *(string_data + i*dims[1] + j) << " ";
            }
            else {
                std::cout << "Unsupported data type" << std::endl;
                break;
            }
        }
        std::cout << std::endl;
    }

    // Don't forget to release the memory used by the PyObject pointers
    Py_DECREF(script);
    Py_XDECREF(df);
    Py_XDECREF(np_array);
    Py_Finalize();
    return 0;
}