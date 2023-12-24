#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <array>
#include <sys/wait.h>
#include <memory>
#include <stdexcept>

#include "checker.h"

inline std::string readonly_execute(std::string& path) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(path.c_str(), "r"), pclose);

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

inline std::string execute(std::string& path, std::string& inputs) {
    int to_child_pipe[2];
    int from_child_pipe[2];
    pipe(to_child_pipe);
    pipe(from_child_pipe);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(to_child_pipe[0], STDIN_FILENO);
        close(to_child_pipe[0]);
        close(to_child_pipe[1]);

        dup2(from_child_pipe[1], STDOUT_FILENO);
        close(from_child_pipe[0]);
        close(from_child_pipe[1]);

        execl(path.c_str(), "");
    }

    // Only parent will execute this since the child gets replaced
    close(to_child_pipe[0]);
    close(from_child_pipe[1]);

    FILE* write_fp = fdopen(to_child_pipe[1], "w");
    FILE* read_fp = fdopen(from_child_pipe[0], "r");

    fprintf(write_fp, inputs.c_str());
    fflush(write_fp);

    std::string output;
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), read_fp) != NULL) {
        output += std::string(buffer);
    }

    fclose(write_fp);
    fclose(read_fp);

    waitpid(pid, NULL, 0); // wait in case

    return output;
}

inline bool cmp_strs(std::string& st1, std::string& st2) {
    int p1 = 0;
    int p2 = 0;
    int c1 = 0;
    int c2 = 0;

    while (p1 != st1.length() && p2 != st2.length()) {
        bool flag = 0;

        if (st1[p1] == ' ' || st1[p1] == '\n') {
            ++p1; flag = 1;
        } else ++c1;
        if (st2[p2] == ' ' || st2[p2] == '\n') {
            ++p2; flag = 1;
        } else ++c2;

        if (!flag) {
            if (st1[p1] != st2[p2]) {
                return false;
            }
            ++p1; ++p2;
        }
    }

    return c1 == c2;
}

namespace checker {
    namespace settings {
        int num_threads = 1;
    }

    // true = passed, false = failed
    checker::Result check(std::string& generator, std::string& bruteforcer, std::string& solver) {
        checker::Result ans;

        std::string tc = readonly_execute(generator);
        std::string groundtruth = execute(bruteforcer, tc);
        std::string solver_truth = execute(solver, tc);

        ans.verdict = cmp_strs(groundtruth, solver_truth);

        // to speed up
        if (!ans.verdict) {
            ans.tc = tc;
            ans.groundtruth = groundtruth;
            ans.solver_answer = solver_truth;
        }

        return ans;
    }
}