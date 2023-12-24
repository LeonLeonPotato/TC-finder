#ifndef CHECKER_H
#define CHECKER_H

#include <string>

namespace checker {
    namespace settings {
        extern int num_threads;
    };

    struct Result {
        bool verdict;
        std::string tc;
        std::string groundtruth;
        std::string solver_answer;
    };

    checker::Result check(std::string& generator, std::string& bruteforcer, std::string& solver);
}

#endif