#pragma once

#include <chrono>
#include <string>
#include <iostream>

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

class Timer {
public:
    Timer(const std::string& name = "Timer") : name_(name) {
        start_ = getTime();
    }

    double elapsed() const {
        return getTime() - start_;
    }

    void reset() {
        start_ = getTime();
    }

    void print() const {
        std::cout << name_ << ": " << elapsed() << " s\n";
    }

private:
    std::string name_;
    double start_;

    double getTime() const {
#ifdef ENABLE_MPI
        return MPI_Wtime();
#else
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
        return duration.count() * 1e-9;
#endif
    }
};
