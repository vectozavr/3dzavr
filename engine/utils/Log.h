#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <string>

namespace Log {
    /**
     * Logs a message to the appropriate logging output.
     *
     * This function is responsible for handling all logging operations within the application.
     * It can be used to log informational messages, warnings, errors, or any other types of messages
     * that need to be recorded.
     *
     * @param message The message to log.
     */
    void log(const std::string &message);
};


#endif //UTILS_LOG_H
