#include "tbb/tbb.h"
#include "tbb/blocked_range.h"
#include <iostream>

auto main(int argc, char* argv[])-> int {
    using namespace std;
    using namespace tbb;
    task_scheduler_init init;
    parallel_for(
        blocked_range<size_t>(0, 100),
        [=](const blocked_range<size_t>& r) { for(size_t i=r.begin(); i!=r.end(); ++i) {
            cout << i << endl;
        } }
    );
    cout << "ok" << endl;
    return 0;
}

#include <chrono>
#include <thread>
#include <optional>
#include <variant>