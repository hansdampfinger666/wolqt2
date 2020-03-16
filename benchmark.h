#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <iostream>


class Benchmarker
{
public:
    static void start_timer();
    static void end_timer(const std::string &func);

private:
    static std::chrono::steady_clock::time_point begin;
    static std::chrono::steady_clock::time_point end;
};

#endif // BENCHMARK_H
