//
// Created by Иван Ильин on 13.01.2021.
//

#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <iomanip>
#include <fstream>
#include <iostream>

#include "Log.h"
#include "Time.h"
#include "../Consts.h"

namespace Log {
    void log(const std::string &message) {
        if (Consts::USE_LOG_FILE) {
            std::time_t const now_c = std::time(nullptr);
            auto dt = std::put_time(std::localtime(&now_c), "%F %T");

            std::fstream file("engine_log.txt", std::ios::out | std::ios::app);
            file << dt << "\t" << message << " (" << Time::fps() << " fps)" << std::endl;
            std::cout << dt << "\t" << message << " (" << Time::fps() << " fps)" << std::endl;
            file.close();
        }
    }
}
