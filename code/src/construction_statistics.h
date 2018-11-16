
/*
 * This file contains common functionality used by/for the *_statistic programs for each construction.
 */

#pragma once

#include "construction_executioner.h"


struct Statistics {
    std::string input_file;
    size_t original_size;
    size_t number_of_sccs;
    size_t number_of_colors;
    long milliseconds_taken;
    size_t new_size;
};

// Have to be defined by the includer of this file.
std::unique_ptr<tollk::ConstructionExecutor> CreateConstructionExecutor();

// Parses given args into options. In this process, ConstructionExecutor::InitializeFlags and ::ParseFlags are called.
std::unique_ptr<tollk::BaseOptions> ParseArgs(int argc, char** argv, tollk::ConstructionExecutor* specialization);

// Main
int main(int argc, char** argv);