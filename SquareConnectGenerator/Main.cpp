/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Util.hpp"
#include "Output.hpp"
#include "Generator.hpp"
#include "UserFilter.hpp"
#include "Filter.hpp"
#include "Profiler.hpp"
#include "Solver.hpp"
#include "Movement.hpp"
#include "Board.hpp"
#include "Square.hpp"
#include "Pos.hpp"

#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <stack>
#include <map>
#include <chrono>
#include <string>
#include <iostream>
#include <csignal>

static std::atomic<bool> _exitFlag{ false };

void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    _exitFlag.store(true);
}

static std::mutex _mutex;
static std::stack<Generator::Product> _product;

static void generatePuzzles(const Filter& filter)
{
    while (!_exitFlag.load()) {

        Generator::Product prod = Generator::Generate(filter);

        if (prod.GetStatus() != Generator::Product::Status::SUCCESS)
            continue;

        auto lock = std::scoped_lock{ _mutex };
        _product.push(prod);
    }
}

static std::chrono::time_point<std::chrono::steady_clock> _startTime;

static void printClear()
{
    std::cout << "\033c";
}

static void printStatus(int numThread, const std::map<std::string, int>& genCount)
{
    auto currTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currTime - _startTime).count();
    auto hour = elapsed / 3600;
    elapsed = elapsed % 3600;
    auto min = elapsed / 60;
    elapsed = elapsed % 60;
    auto sec = elapsed;

    printClear();
    std::cout << "############################################################" << std::endl;
    std::cout << "Elapsed Time: " << hour << "h " << min << "m " << sec << "s" << std::endl;
    std::cout << "Threads: " << numThread << std::endl;
    std::cout << "############################################################" << std::endl;
    for (const auto& gc : genCount)
        std::cout << gc.first << ": " << gc.second << std::endl;
    std::cout << "############################################################" << std::endl;
}

int main()
{
    // Handle signals
    
    signal(SIGINT, signalHandler);

    // Get number of threads

    int numThread{ 0 };

    while (!_exitFlag.load()) {

        std::cout << "Enter number of threads:" << std::endl;
        std::cin >> numThread;

        if (numThread <= 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input" << std::endl;
            continue;
        }

        break;
    }

    // Initialize common classes

    Output output;
    Filter filter;
    const Filter& filterCRef = filter;

    UserFilter::AddEntries(filter);

    // This is used to track the number of puzzles generated for each Filter entry
    std::map<std::string, int> genCount;
    for (int num = 0; num < filterCRef.GetNumEntry(); num++)
        genCount.insert(std::pair<std::string, int>(filterCRef.GetEntryTitle(num), 0));

    // Spawn Generator Threads

    std::vector<std::thread> thread;
    thread.reserve(numThread);
    for (int num = 0; num < numThread; num++)
        thread.emplace_back(std::thread(generatePuzzles, std::cref(filterCRef)));

    // Process generated puzzles from Generator Threads

    _startTime = std::chrono::steady_clock::now();

    while (!_exitFlag.load()) {

        std::this_thread::sleep_for(std::chrono::seconds(1));

        auto lock = std::scoped_lock{ _mutex };

        while (!_product.empty()) {

            const Generator::Product& prod = _product.top();

            int filNum = prod.GetFilterNum();
            const Board& brd = prod.GetBoard();
            const Square& sqr = prod.GetSquare();
            const Solver::Solution& sol = prod.GetSolution();

            auto gcIter = genCount.find(filterCRef.GetEntryTitle(filNum));
            assert(gcIter != genCount.end());
            gcIter->second++;

            int cnt = gcIter->second;
            std::string subDir = std::string("Depth_") + std::to_string(sol.GetDepth());
            std::string fileName = filterCRef.GetEntryTitle(filNum);

            output.AppendToFile(subDir, fileName, cnt, brd, sqr, sol);
            _product.pop();
        }

        printStatus(numThread, genCount);
    }

    // Wait for threads to exit

    for (int num = 0; num < numThread; num++)
        thread.at(num).join();

    std::cout << "Program exited" << std::endl;
    exit(0);
    return 0;
}
