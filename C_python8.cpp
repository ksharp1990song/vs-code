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
#include <pybind11/pybind11.h>
using namespace std;

int main()
{
    long x1 = 1;
    double x2 = 3.14;
    string x3 = "Hello";

    // 定义计时器的时间间隔为1秒
    chrono::milliseconds interval(1000);

    // 获取当前时间
    auto start = chrono::steady_clock::now();

    // 开始计时器
    int count = 0;

    while (true)
    {
        // 获取当前时间
        auto now = chrono::steady_clock::now();

        // 如果已经过了指定时间间隔，则更新x1和x2的值
        if (now - start >= interval && count < 10)
        {
            x1 += 1;
            x2 *= 2;
            count++;

            std::cout << "Count: " << count << ", x1 = " << x1 << ", x2 = " << x2 << ", x3 = " << x3 << std::endl;

            // 重置计时器开始时间
            start = now;
        }

        // 当计数达到10时，跳出循环
        if (count >= 10)
            break;
    }


    return 0;
}