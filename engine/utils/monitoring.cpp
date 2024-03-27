#include "monitoring.h"
#include "Consts.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <psapi.h>
int getProcessSizeMB() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / Consts::MB;
    }
    return -1;
}
#elif defined(__linux__)
#include <unistd.h>
#include <fstream>
int getProcessSizeMB() {
    std::ifstream statmFile("/proc/self/statm");
    long pageSizeMB = sysconf(_SC_PAGESIZE) / Consts::MB;
    unsigned long vmSize, residentSize;
    if (statmFile >> vmSize >> residentSize) {
        return residentSize * pageSizeMB;
    }
    return -1;
}
#elif defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>
int getProcessSizeMB() {
    mach_task_basic_info_data_t info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
        return info.resident_size / Consts::MB;
    }
    return -1;
}
#else
int getProcessSizeMB() {
    return -1;
}
#endif
