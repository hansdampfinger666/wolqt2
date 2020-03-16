#include <benchmark.h>


std::chrono::steady_clock::time_point Benchmarker::begin;
std::chrono::steady_clock::time_point Benchmarker::end;


void Benchmarker::start_timer()
{
    begin = std::chrono::steady_clock::now();
}


void Benchmarker::end_timer(const std::string &func)
{
    end = std::chrono::steady_clock::now();

    std::cout << "Begin stats for: " << func << std::endl
              << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << "[ms]" << std::endl
              << "Time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
              << "[µs]" << std::endl
              << "Time: "
              << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count()
              << "[ns]" << std::endl
              << "End stats for " << func << std::endl;
}
