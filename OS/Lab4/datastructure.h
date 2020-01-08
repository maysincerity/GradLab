#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <string>

using namespace std;

class SystemInfo{
public:
    string hostName;
    string sysStartTime;
    string sysRunningTime;
    string kernVersion;
    string cpuModel;
    string coreNums;
};

class ProcInfo{
public:
    string name;
    string pid;
    string ppid;
    string rss;
    string priority;
};

#endif // DATASTRUCTURE_H
