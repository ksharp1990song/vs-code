/*==========================================================================*/
/*  DriverModel.cpp                                  DLL Module for VISSIM  */
/*                                                                          */
/*  Interface module for external driver models.                            */
/*  Dummy version that does nothing (uses Vissim's internal model).         */
/*                                                                          */
/*  Version of 2021-02-24                                   Lukas Kautzsch  */
/*==========================================================================*/

#include "DriverModel.h"
#include <Python.h>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <cmath>
#include <set>
#include <stdio.h>
#include <string>
#include <type_traits>
#include <stdexcept>
#include <vector>
#include <variant>
#include <numpy/arrayobject.h>
#include <numpy/ndarrayobject.h>
#include <numpy/ndarraytypes.h>
using namespace std;

/*==========================================================================*/

double  desired_acceleration = 3.0;
double  desired_lane_angle = 0.0;
int     active_lane_change = 0;
int     rel_target_lane = 0;
double  desired_velocity = 15.0;
int     turning_indicator = 0;
int     vehicle_color = RGB(225, 225, 225);

/*==========================================================================*/
/*Create Variables*/

double  time_step;
double  current_time;
long    current_vehicle;
double  current_velocity;
double  current_acceleration;
double  max_acceleration;
long    vehicle_type;
double  signal_distance1;
double  signal_distance2;
long    signal_state1;
long    signal_state2;
double  confl_distance1;
double  confl_distance2;
long    confl_type1;
long    confl_type2;
//vector<variant<long, double, string, map<string, int>>> result;



namespace DLL_TO_TXT {

    template <typename T>

    // dll to text function
    void WriteToTXT(const std::string& name, T& value) 
    {
        ofstream file("test.txt", ios_base::app);
        file << name << ":" << value << endl;
        file.close();
    }

    // data extraction function
    
    vector<variant<long,double,string>> convertPythonTo(const string& moduleName, const string& varName) {

        vector<variant<long, double, string>> v;
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
        PyObject* pandas_dataframe_type = PyObject_GetAttrString(pandas_module, "DataFrame");

        // Importing the Python script
        PyObject* module = PyImport_ImportModule(moduleName.c_str());
        if (module == NULL) {
            cout << "Error: script module not found" << endl;
            return v;
        }

        // Extracting the DataFrame from the script
        PyObject* obj = PyObject_GetAttrString(module, varName.c_str());
        if (obj == NULL) {
            cout << "Error: nparr1 variable not found" << endl;
            return v;
        }

        if (PyLong_Check(obj)) {
            v.push_back(PyLong_AsLong(obj));
        }
        else if (PyFloat_Check(obj)) {
            v.push_back(PyFloat_AsDouble(obj));
        }
        else if (PyUnicode_Check(obj)) {
            v.push_back(PyUnicode_AsUTF8(obj));
        }
        else if (PyList_Check(obj)) {
            size_t len = PyList_Size(obj);
            for (int i = 0; i < len; i++) {
                auto pyobj = PyList_GetItem(obj, i);
                if (PyLong_Check(pyobj)) {
                    v.emplace_back(PyLong_AsLong(pyobj));
                }
                else if (PyFloat_Check(pyobj)) {
                    v.emplace_back(PyFloat_AsDouble(pyobj));
                }
                else if (PyUnicode_Check(pyobj)) {
                    v.emplace_back(PyUnicode_AsUTF8(pyobj));
                }
                else {
                    throw runtime_error("Unsupported data type in the list");
                }
            }
        }/*
        else if (PyDict_Check(obj)) {
            PyObject* key, * value;
            Py_ssize_t pos = 0;
            map<string, int> m;
            while (PyDict_Next(obj, &pos, &key, &value)) {
                string keystr = PyUnicode_AsUTF8(key);
                int val = PyLong_AsLong(value);
                m[keystr] = val;
                v.push_back(m);
            }
        }
        else if (PyTuple_Check(obj)) {
            size_t len = PyTuple_Size(obj);
            for (int i = 0; i < len; i++) {
                auto pyobj = PyTuple_GetItem(obj, i);
                if (PyLong_Check(pyobj)) {
                    v.emplace_back(PyLong_AsLong(pyobj));
                }
                else if (PyFloat_Check(pyobj)) {
                    v.emplace_back(PyFloat_AsDouble(pyobj));
                }
                else if (PyUnicode_Check(pyobj)) {
                    v.emplace_back(PyUnicode_AsUTF8(pyobj));
                }
                else {
                    throw runtime_error("Unsupported data type in the list");
                }
            }
        }
        else if (PyObject_IsInstance(obj, pandas_dataframe_type)) {
            cout << "dataframe succeed loaded" << endl;
            //  Transfer a dataframe into numpy array    
            PyObject* nparr = PyObject_CallFunctionObjArgs(numpy_array_type, obj, NULL);
            // Accessing the NumPy array data and printing the array elements
            size_t rows = PyArray_DIM(nparr, 0);
            size_t cols = PyArray_DIM(nparr, 1);
            // Print out the data
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    PyObject* arr = PyArray_GETITEM(nparr, (char*)PyArray_GETPTR2(nparr, i, j));
                    if (PyUnicode_Check(arr)) {
                        PyObject* str = PyUnicode_AsUTF8String(arr);
                        v.push_back(string(PyBytes_AsString(str)));
                        Py_DECREF(str);
                    }
                    else if (PyLong_Check(arr)) {
                        v.push_back(PyLong_AsLong(arr));
                    }
                    else if (PyFloat_Check(arr)) {
                        v.push_back(PyFloat_AsDouble(arr));
                    }
                }
            }
            Py_DECREF(nparr);
        }*/
        else if (PyObject_IsInstance(obj, numpy_array_type)) {
            cout << "succeed loaded" << endl;
            // Accessing the NumPy array data and printing the array elements
            size_t rows = PyArray_DIM(obj, 0);
            size_t cols = PyArray_DIM(obj, 1);
            // Print out the data
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    PyObject* arr = PyArray_GETITEM(obj, (char*)PyArray_GETPTR2(obj, i, j));
                    if (PyUnicode_Check(arr)) {
                        PyObject* str = PyUnicode_AsUTF8String(arr);
                        v.push_back(string(PyBytes_AsString(str)));
                        Py_DECREF(str);
                    }
                    else if (PyLong_Check(arr)) {
                        v.push_back(PyLong_AsLong(arr));
                    }
                    else if (PyFloat_Check(arr)) {
                        v.push_back(PyFloat_AsDouble(arr));
                    }
                }
            }
        }
        else {
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
}

BOOL APIENTRY DllMain(HANDLE  hModule,
    DWORD   ul_reason_for_call,
    LPVOID  lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

/*==========================================================================*/

DRIVERMODEL_API  int  DriverModelSetValue(int    type,
    int    index1,
    int    index2,
    int    int_value,
    double double_value,
    char* string_value)
{
    /* Sets the value of a data object of type <type>, selected by <index1> */
    /* and possibly <index2>, to <int_value>, <double_value> or             */
    /* <*string_value> (object and value selection depending on <type>).    */
    /* Return value is 1 on success, otherwise 0.                           */

    switch (type) {
    case DRIVER_DATA_TIMESTEP:
        time_step = double_value;
        return 1;
    case DRIVER_DATA_TIME:
        current_time = double_value;
        return 1;
    case DRIVER_DATA_USE_UDA:
        return 1; /* doesn't use any UDAs */
                  /* must return 1 for desired values of index1 if UDA values are to be sent from/to Vissim */
    case DRIVER_DATA_VEH_ID:
        current_vehicle = int_value;
        return 1;
    case DRIVER_DATA_VEH_LANE:
    case DRIVER_DATA_VEH_ODOMETER:
    case DRIVER_DATA_VEH_LANE_ANGLE:
    case DRIVER_DATA_VEH_LATERAL_POSITION:
    case DRIVER_DATA_VEH_VELOCITY:
        current_velocity = double_value;
        return 1;
    case DRIVER_DATA_VEH_ACCELERATION:
        current_acceleration = double_value;
        return 1;
    case DRIVER_DATA_VEH_LENGTH:
    case DRIVER_DATA_VEH_WIDTH:
    case DRIVER_DATA_VEH_WEIGHT:
    case DRIVER_DATA_VEH_MAX_ACCELERATION:
        max_acceleration = double_value;
        return 1;
    case DRIVER_DATA_VEH_TURNING_INDICATOR:
        turning_indicator = int_value;
        return 1;
    case DRIVER_DATA_VEH_CATEGORY:
    case DRIVER_DATA_VEH_PREFERRED_REL_LANE:
    case DRIVER_DATA_VEH_USE_PREFERRED_LANE:
    case DRIVER_DATA_VEH_DESIRED_VELOCITY:
        desired_velocity = double_value;
        return 1;
    case DRIVER_DATA_VEH_X_COORDINATE:
        return 1;   
    case DRIVER_DATA_VEH_Y_COORDINATE:
        return 1;                       
    case DRIVER_DATA_VEH_Z_COORDINATE:
    case DRIVER_DATA_VEH_REAR_X_COORDINATE:
    case DRIVER_DATA_VEH_REAR_Y_COORDINATE:
    case DRIVER_DATA_VEH_REAR_Z_COORDINATE:
    case DRIVER_DATA_VEH_TYPE:
        vehicle_type = int_value;
        return 1;
    case DRIVER_DATA_VEH_COLOR:
        vehicle_color = int_value;
        return 1;
    case DRIVER_DATA_VEH_CURRENT_LINK:
        return 1; /* (To avoid getting sent lots of DRIVER_DATA_VEH_NEXT_LINKS messages) */
                  /* Must return 1 if these messages are to be sent from VISSIM!         */
    case DRIVER_DATA_VEH_NEXT_LINKS:
    case DRIVER_DATA_VEH_ACTIVE_LANE_CHANGE:
    case DRIVER_DATA_VEH_REL_TARGET_LANE:
    case DRIVER_DATA_VEH_INTAC_STATE:
    case DRIVER_DATA_VEH_INTAC_TARGET_TYPE:
    case DRIVER_DATA_VEH_INTAC_TARGET_ID:
    case DRIVER_DATA_VEH_INTAC_HEADWAY:
    case DRIVER_DATA_VEH_UDA:
    case DRIVER_DATA_NVEH_ID:
    case DRIVER_DATA_NVEH_LANE_ANGLE:
    case DRIVER_DATA_NVEH_LATERAL_POSITION:
    case DRIVER_DATA_NVEH_DISTANCE:
    case DRIVER_DATA_NVEH_REL_VELOCITY:
    case DRIVER_DATA_NVEH_ACCELERATION:
    case DRIVER_DATA_NVEH_LENGTH:
    case DRIVER_DATA_NVEH_WIDTH:
    case DRIVER_DATA_NVEH_WEIGHT:
    case DRIVER_DATA_NVEH_TURNING_INDICATOR:
    case DRIVER_DATA_NVEH_CATEGORY:
    case DRIVER_DATA_NVEH_LANE_CHANGE:
    case DRIVER_DATA_NVEH_TYPE:
    case DRIVER_DATA_NVEH_UDA:
    case DRIVER_DATA_NVEH_X_COORDINATE:
    case DRIVER_DATA_NVEH_Y_COORDINATE:
    case DRIVER_DATA_NVEH_Z_COORDINATE:
    case DRIVER_DATA_NVEH_REAR_X_COORDINATE:
    case DRIVER_DATA_NVEH_REAR_Y_COORDINATE:
    case DRIVER_DATA_NVEH_REAR_Z_COORDINATE:
    case DRIVER_DATA_NO_OF_LANES:
    case DRIVER_DATA_LANE_WIDTH:
    case DRIVER_DATA_LANE_END_DISTANCE:
    case DRIVER_DATA_CURRENT_LANE_POLY_N:
    case DRIVER_DATA_CURRENT_LANE_POLY_X:
    case DRIVER_DATA_CURRENT_LANE_POLY_Y:
    case DRIVER_DATA_CURRENT_LANE_POLY_Z:
    case DRIVER_DATA_RADIUS:
        return 1;
    case DRIVER_DATA_MIN_RADIUS:
        return 1;
    case DRIVER_DATA_DIST_TO_MIN_RADIUS:
        return 1;
    case DRIVER_DATA_SLOPE:
        return 1;
    case DRIVER_DATA_SLOPE_AHEAD:
        return 1;
    case DRIVER_DATA_SIGNAL_DISTANCE:
        if (index1 == 1 && index2 == 1)
        {
            signal_distance1 = double_value;
            return 1;
        }
        if (index1 == 1 && index2 == 2)
        {
            signal_distance2 = double_value;
            return 1;
        }
        [[fallthrough]];
    case DRIVER_DATA_SIGNAL_STATE:
        if (index1 == 1 && index2 == 1)
        {
            signal_state1 = int_value;
            return 1;
        }
        if (index1 == 1 && index2 == 2)
        {
            signal_state2 = int_value;
            return 1;
        }
        [[fallthrough]];
    case DRIVER_DATA_SIGNAL_STATE_START:
    case DRIVER_DATA_SPEED_LIMIT_DISTANCE:
    case DRIVER_DATA_SPEED_LIMIT_VALUE:
    case DRIVER_DATA_PRIO_RULE_DISTANCE:
    case DRIVER_DATA_PRIO_RULE_STATE:
    case DRIVER_DATA_ROUTE_SIGNAL_DISTANCE:
    case DRIVER_DATA_ROUTE_SIGNAL_STATE:
    case DRIVER_DATA_ROUTE_SIGNAL_CYCLE:
    case DRIVER_DATA_CONFL_AREAS_COUNT:
        return 1;  /* (to avoid getting sent lots of conflict area data) */
    case DRIVER_DATA_CONFL_AREA_TYPE:
        if (index1 == 1 && index2 == 1)
        {
            confl_type1 = int_value;
            return 1;
        }
        if (index1 == 1 && index2 == 2)
        {
            confl_type2 = int_value;
            return 1;
        }
        [[fallthrough]];
    case DRIVER_DATA_CONFL_AREA_YIELD:
        return 1;
    case DRIVER_DATA_CONFL_AREA_DISTANCE:
        if (index1 == 1)
        {
            confl_distance1 = double_value;
            return 1;
        }
        [[fallthrough]];
    case DRIVER_DATA_CONFL_AREA_LENGTH:
    case DRIVER_DATA_CONFL_AREA_VEHICLES:
    case DRIVER_DATA_CONFL_AREA_TIME_ENTER:
    case DRIVER_DATA_CONFL_AREA_TIME_IN:
    case DRIVER_DATA_CONFL_AREA_TIME_EXIT:
    case DRIVER_DATA_DESIRED_ACCELERATION:
        desired_acceleration = double_value;
        return 1;
    case DRIVER_DATA_DESIRED_LANE_ANGLE:
        desired_lane_angle = double_value;
        return 1;
    case DRIVER_DATA_ACTIVE_LANE_CHANGE:
        active_lane_change = int_value;
        return 1;
    case DRIVER_DATA_REL_TARGET_LANE:
        rel_target_lane = int_value;
        return 1;
    default:
        return 0;
    }
}



/*--------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------*/

DRIVERMODEL_API  int  DriverModelSetValue3(int    type,
                                           int    index1,
                                           int    index2,
                                           int    index3,
                                           int    int_value,
                                           double double_value,
                                           char   *string_value)
{
    /* Sets the value of a data object of type <type>, selected by <index1>, */
    /* <index2> and <index3>, to <int_value>, <double_value> or              */
    /* <*string_value> (object and value selection depending on <type>).     */
    /* Return value is 1 on success, otherwise 0.                            */
    /* DriverModelGetValue (DRIVER_DATA_MAX_NUM_INDICES, ...) needs to set   */
    /* *int_value to 3 or greater in order to activate this function!        */

    switch (type) {
    case DRIVER_DATA_ROUTE_SIGNAL_SWITCH:
        return 0; /* don't send any more switch values */
    default:
        return 0;
    }
}

/*--------------------------------------------------------------------------*/

DRIVERMODEL_API  int  DriverModelGetValue(int    type,
                                          int    index1,
                                          int    index2,
                                          int    *int_value,
                                          double *double_value,
                                          char   **string_value)
{
    /* Gets the value of a data object of type <type>, selected by <index1> */
    /* and possibly <index2>, and writes that value to <*int_value>,        */
    /* <*double_value> or <**string_value> (object and value selection      */
    /* depending on <type>).                                                */
    /* Return value is 1 on success, otherwise 0.                           */

    switch (type) {
    case DRIVER_DATA_STATUS:
        *int_value = 0;
        return 1;
    case DRIVER_DATA_TIME:
        *double_value = current_time;
        return 1;
    case DRIVER_DATA_WANTS_ALL_SIGNALS:
        *int_value = 1; /* needs to be set to zero if no global signal data is required */
        return 1;
    case DRIVER_DATA_MAX_NUM_INDICES:
        *int_value = 3; /* because DriverModelSetValue3() and DriverModelSetValue3() exist in this DLL */
        return 1;
    case DRIVER_DATA_VEH_ID:
        *int_value = current_vehicle;
        return 1;
    case DRIVER_DATA_VEH_VELOCITY:
        *double_value = current_velocity;
        return 1;
    case DRIVER_DATA_VEH_ACCELERATION:
        *double_value = current_acceleration;
        return 1;
    case DRIVER_DATA_VEH_TURNING_INDICATOR:
        *int_value = turning_indicator;
        return 1;
    case DRIVER_DATA_VEH_DESIRED_VELOCITY:
        *double_value = desired_velocity;
        return 1;
    case DRIVER_DATA_VEH_TYPE:
        *int_value = vehicle_type;
        return 1;
    case DRIVER_DATA_VEH_COLOR:
        *int_value = vehicle_color;
        return 1;
    case DRIVER_DATA_VEH_UDA:
        return 1; /* doesn't set any UDA values */
    case DRIVER_DATA_SIGNAL_STATE:
        *int_value = signal_state1;
        *int_value = signal_state2;
        return 1;
    case DRIVER_DATA_SIGNAL_DISTANCE:
        *double_value = signal_distance1;
        *double_value = signal_distance2;
        return 1;
        [[fallthrough]];
    case DRIVER_DATA_WANTS_SUGGESTION:
        *int_value = 1;
        return 1;
    case DRIVER_DATA_DESIRED_ACCELERATION:
        *double_value = desired_acceleration;
        return 1;
    case DRIVER_DATA_DESIRED_LANE_ANGLE:
        *double_value = desired_lane_angle;
        return 1;
    case DRIVER_DATA_ACTIVE_LANE_CHANGE:
        *int_value = active_lane_change;
        return 1;
    case DRIVER_DATA_REL_TARGET_LANE:
        *int_value = rel_target_lane;
        return 1;
    case DRIVER_DATA_SIMPLE_LANECHANGE:
        *int_value = 1;
        return 1;
    case DRIVER_DATA_USE_INTERNAL_MODEL:
        *int_value = 0; /* must be set to 0 if external model is to be applied */
        return 1;
    case DRIVER_DATA_WANTS_ALL_NVEHS:
        *int_value = 0; /* must be set to 1 if data for more than 2 nearby vehicles per lane and upstream/downstream is to be passed from Vissim */
        return 1;
    case DRIVER_DATA_ALLOW_MULTITHREADING:
        *int_value = 0; /* must be set to 1 to allow a simulation run to be started with multiple cores used in the simulation parameters */
        return 1;
    default:
        return 0;
    }
}

/*--------------------------------------------------------------------------*/


/*==========================================================================*/

DRIVERMODEL_API  int  DriverModelExecuteCommand(int  number)
{
    /* Executes the command <number> if that is available in the driver */
    /* module. Return value is 1 on success, otherwise 0.               */

    switch (number) {
    case DRIVER_COMMAND_INIT:
        return 1;
    case DRIVER_COMMAND_CREATE_DRIVER:
        return 1;
    case DRIVER_COMMAND_KILL_DRIVER:
        return 1;
    case DRIVER_COMMAND_MOVE_DRIVER:
    {
        DLL_TO_TXT::WriteToTXT("current_velocity", current_velocity);
        //auto result = DLL_TO_TXT::convertPythonTo("test2", "x6");
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
        PyObject* pandas_dataframe_type = PyObject_GetAttrString(pandas_module, "DataFrame");

        // Importing the Python script
        PyObject* module = PyImport_ImportModule("test2");
        if (module == NULL) {
            cout << "Error: script module not found" << endl;
            return -1;
        }

        // Extracting the DataFrame from the script
        PyObject* obj = PyObject_GetAttrString(module, "rand_array");
        if (obj == NULL) {
            cout << "Error: nparr1 variable not found" << endl;
            return -1;
        }
        string str1 = "load succeed";

        if (PyLong_Check(obj)) {
            long x1 = PyLong_AsLong(obj);
            DLL_TO_TXT::WriteToTXT("x1", x1);
            DLL_TO_TXT::WriteToTXT("result", str1);
        }
        else if (PyFloat_Check(obj)) {
            double x2 = PyFloat_AsDouble(obj);
            DLL_TO_TXT::WriteToTXT("x2", x2);
            DLL_TO_TXT::WriteToTXT("result", str1);
        }
        else if (PyUnicode_Check(obj)) {
            PyObject* str = PyUnicode_AsUTF8String(obj);
            string x3 = PyBytes_AsString(str);
            Py_DECREF(str);
            DLL_TO_TXT::WriteToTXT("x3", x3);
            DLL_TO_TXT::WriteToTXT("result", str1);
        }
        else if (PyList_Check(obj)) {
            size_t len = PyList_Size(obj);
            vector<variant<long, double, string>> v;
            for (size_t i = 0; i < len; i++) {
                auto pyobj = PyList_GetItem(obj, i);
                if (PyLong_Check(pyobj)) {
                    v.emplace_back(PyLong_AsLong(pyobj));
                }
                else if (PyFloat_Check(pyobj)) {
                    v.emplace_back(PyFloat_AsDouble(pyobj));
                }
                else if (PyUnicode_Check(pyobj)) {
                    v.emplace_back(PyUnicode_AsUTF8(pyobj));
                }
            } 
            long x4 = get<long>(v.at(0));
            DLL_TO_TXT::WriteToTXT("x4", x4);
        }
        else if (PyObject_IsInstance(obj, numpy_array_type)) {
            size_t rows = PyArray_DIM(obj, 0);
            size_t cols = PyArray_DIM(obj, 1);
            vector<variant<long, double, string>> v;
            for (size_t i = 0; i < rows; i++) {
                for (size_t j = 0; j < cols; j++) {
                    PyObject* arr = PyArray_GETITEM(obj, (char*)PyArray_GETPTR2(obj, i, j));
                    if (PyLong_Check(arr)) {
                        v.emplace_back(PyLong_AsLong(arr));
                    }
                    else if (PyFloat_Check(arr)) {
                        v.emplace_back(PyFloat_AsDouble(arr));
                    }
                    else if (PyUnicode_Check(arr)) {
                        v.emplace_back(PyUnicode_AsUTF8(arr));
                    }
                }
            }
            double x5 = get<double>(v.at(0));
            DLL_TO_TXT::WriteToTXT("x5", x5);
        }
        else if (PyObject_IsInstance(obj, pandas_dataframe_type)) {
            PyObject* nparr = PyObject_CallFunctionObjArgs(numpy_array_type,obj,NULL);
            size_t rows = PyArray_DIM(nparr,0);
            size_t cols = PyArray_DIM(nparr,1);
            vector<variant<long, double, string>> v;
            for (size_t i = 0; i < rows; i++) {
                for (size_t j = 0; j < cols; j++) {
                    PyObject* arr = PyArray_GETITEM(nparr, (char*)PyArray_GETPTR2(nparr, i, j));
                    if (PyLong_Check(arr)) {
                        v.emplace_back(PyLong_AsLong(arr));
                    }
                    else if (PyFloat_Check(arr)) {
                        v.emplace_back(PyFloat_AsDouble(arr));
                    }
                    else if (PyUnicode_Check(arr)) {
                        v.emplace_back(PyUnicode_AsUTF8(arr));
                    }
                }
            }
            long x6 = get<long>(v.at(0));
            DLL_TO_TXT::WriteToTXT("x6", x6);
        }
        else if (PyDict_Check(obj)) {
            PyObject *key, *value;
            Py_ssize_t pos = 0;
            map<string, int> m;
            while (PyDict_Next(obj, &pos, &key, &value)) {
                string keystr = PyUnicode_AsUTF8(key);
                int val = PyLong_AsLong(value);
                m[keystr] = val;
            }
            for (auto const& pair : m) {
                string x7 = pair.first;
                int x8 = pair.second;
                DLL_TO_TXT::WriteToTXT("x7", x7);
                DLL_TO_TXT::WriteToTXT("x8", x8);
            }
        }
        else if (PyTuple_Check(obj)) {
            size_t len = PyTuple_Size(obj);
            vector<variant<long, double, string>> v;
            for (size_t i = 0; i < len; i++) {
                auto pyobj = PyTuple_GetItem(obj, i);
                if (PyLong_Check(pyobj)) {
                    v.emplace_back(PyLong_AsLong(pyobj));
                }
                else if (PyFloat_Check(pyobj)) {
                    v.emplace_back(PyFloat_AsDouble(pyobj));
                }
                else if (PyUnicode_Check(pyobj)) {
                    v.emplace_back(PyUnicode_AsUTF8(pyobj));
                }
            }
            long x9 = get<long>(v.at(4));
            DLL_TO_TXT::WriteToTXT("x9", x9);
        }
        //string x1 = get<string>(result.at(0));
        //DLL_TO_TXT::WriteToTXT("x1", x1);
        //vector<variant<long, double, string, map<string, int>>>(result).swap(result);
        if (vehicle_type == 700)
        {
            if (signal_distance1 > 0)
            {
                current_velocity += desired_acceleration;
            }
            if (signal_distance1 == 0)
            {
                if (signal_state1 == 1)
                {
                    current_velocity += desired_acceleration;
                }
                if (signal_state1 == 2 || signal_state1 == 3)
                {
                    desired_acceleration = -current_velocity;
                    current_velocity += desired_acceleration;
                }
            }
            if (signal_distance2 > 0)
            {
                current_velocity += desired_acceleration;
            }
            if (signal_distance2 == 0)
            {
                if (signal_state2 == 1)
                {
                    current_velocity += desired_acceleration;
                }
                if (signal_state2 == 2 || signal_state2 == 3)
                {
                    desired_acceleration = -current_velocity;
                    current_velocity += desired_acceleration;
                }
            }
        }
        return 1;
    }
    default:
        return 0;
    }
}

/*==========================================================================*/
/*  End of DriverModel.cpp                                                  */
/*==========================================================================*/

