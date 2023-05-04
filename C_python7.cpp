#include <Python.h>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <vector>
#include <map>
#include <string>
#include <variant>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <numpy/arrayobject.h>
#include <numpy/ndarrayobject.h>
#include <numpy/ndarraytypes.h>
#include <chrono>
using namespace std;

int main()  
{
    int x1 = 1;
    double x2 = 3.14;

    cout << x1 << " " << x2 << endl;

    return 0;
}