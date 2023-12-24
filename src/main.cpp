#include <boost/program_options.hpp>

#include <iostream>
#include <iterator>

#include "checker.h"
using namespace boost::program_options;

void handle_thread_option(int val) {
    if (val <= 0)
        throw validation_error(validation_error::invalid_option_value);

    checker::settings::num_threads = val;
}

int main(int argc, char* argv[])
{
    options_description basic("Basic options");
    basic.add_options()("help", "produce help message");
    basic.add_options()("gen", value<std::string>(), "Path to the test case generator executable");
    basic.add_options()("bf", value<std::string>(), "Path to the brute force / ground truth executable");
    basic.add_options()("solver", value<std::string>(), "Path to the (potentially not correct) efficient solution's executable");

    options_description other("Other options (None for now)");
    // other.add_options()(
    //     "threads,t", 
    //     value<int>()->default_value(1)->notifier(&handle_thread_option), 
    //     "Specify amount of threads to use"
    // );

    options_description all("All options");
    all.add(basic).add(other);

    variables_map map;
    store(parse_command_line(argc, argv, all), map);
    notify(map);

    if (map.contains("help")) {
        std::cout << basic << '\n' << other << '\n';
        return 1;
    }

    std::string gen = map["gen"].as<std::string>();
    std::string bf = map["bf"].as<std::string>();
    std::string solver = map["solver"].as<std::string>();

    int tc_counter = 1;
    while (true) {
        checker::Result res = checker::check(gen, bf, solver);
        if (!res.verdict) {
            std::cout << "Wrong answer!\n";
            std::cout << "Test case: " << "\n";
            std::cout << res.tc;
            std::cout << "Ground truth: " << "\n";
            std::cout << res.groundtruth;
            std::cout << "Your answer: " << "\n";
            std::cout << res.solver_answer;
            return 1;
        } else {
            std::cout << "Test case #" << tc_counter << " passed!\n";
        }
        tc_counter++;
    }

    return 0;
}