//
// Created by Иван Ильин on 13.01.2021.
//

#include "Log.h"
#include "Time.h"
#include <ctime>
#include <fstream>
#include <iostream>

namespace Log
{
    void log(const std::string& message) {
        time_t now = time(nullptr);
        char* dt = ctime(&now);

        std::fstream file("3dzavr_log.txt", std::ios::out | std::ios::app);
        file << dt << "\t" << message << " (" << Time::fps() << " fps)" << std::endl;
        std::cout << dt << "\t" << message << " (" << Time::fps() << " fps)" << std::endl;
        file.close();
    }
}
