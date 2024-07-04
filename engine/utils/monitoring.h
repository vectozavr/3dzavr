#ifndef UTILS_MONITORING_H
#define UTILS_MONITORING_H

/**
 * @brief Retrieves the current process's memory usage in megabytes.
 *
 * This function calculates and returns the amount of memory currently being used by the process
 * that calls it. The memory usage is reported in megabytes (MB). This can be useful for monitoring
 * and debugging purposes, to track how much memory an application is consuming over time.
 *
 * @return The memory usage of the current process in megabytes (MB).
 */
int getProcessSizeMB();

#endif //UTILS_MONITORING_H
