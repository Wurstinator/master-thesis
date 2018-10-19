
/*
 * This file contains common functionality used by/for the *_main programs for each construction.
 */

#pragma once

#include "construction_executioner.h"

// Has to be defined by the includer of this file.
std::unique_ptr<tollk::ConstructionExecutor> CreateConstructionExecutor();

// Parses given args into options. In this process, ConstructionExecutor::InitializeFlags and ::ParseFlags are called.
std::unique_ptr<tollk::BaseOptions> ParseArgs(int argc, char** argv, tollk::ConstructionExecutor* specialization);


// Main
int main(int argc, char** argv);