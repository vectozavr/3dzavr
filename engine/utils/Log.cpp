#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <iomanip>
#include <fstream>
#include <iostream>

#include <utils/Time.h>
#include <utils/Log.h>
#include <Consts.h>
#include <utils/monitoring.h>

namespace Log {
    void log(const std::string &message) {
        if (Consts::USE_LOG_FILE) {
            auto dt = Time::getLocalTimeInfo();
            std::fstream file("engine.log", std::ios::out | std::ios::app);
            file << dt << " | Mem: " << getProcessSizeMB() << "MB " << "\t" << message << " (" << Time::fps() << " fps)" << std::endl;
            std::cout << dt << " | Mem: " << getProcessSizeMB() << "MB " << "\t" << message << " (" << Time::fps() << " fps)" << std::endl;
            file.close();
        }
    }
}
