# cython: language_level=3
# driver_data.pyx

cdef extern from *:
    ctypedef int SIZEOF_VOID_P

from cython cimport view
from libcpp.vector cimport vector as cpp_vector
from libcpp cimport bool as c_bool

cdef extern from "DriverModel.cpp":
    cpp_vector[int] final_vehicle_ids "final_vehicle_ids"
    cpp_vector[double] final_vehicle_speeds "final_vehicle_speeds"

cpdef get_vehicle_ids():
    cdef cpp_vector[int] c_vector = final_vehicle_ids
    return [x for x in c_vector]

cpdef get_vehicle_speeds():
    cdef cpp_vector[double] c_vector = final_vehicle_speeds
    return [x for x in c_vector]