#include "showcurve.h"
#include <QDebug>
#include<iostream>
#include <sstream>
#include <fstream>

using namespace std;


curveObject::curveObject(QObject * parent)
    :QObject(parent)
{
    for(int i = 0; i < infoNums; i ++){
        itemList.append(0);
    }
    oldCpuTotal = newCpuTotal = 0;
    oldCpuIdle = newCpuIdle = 0;
}

void curveObject::refresh()
{
    refreshCpuUsage();
    refreshMemoryInfo();
    unsigned long total = newCpuTotal - oldCpuTotal;
    unsigned long idle = newCpuIdle - oldCpuIdle;
    itemList[CpuUsage] = 100 * (float)(total - idle) / total;
    updateCurve(itemList);
}

void curveObject::refreshCpuUsage()
{
    oldCpuTotal = newCpuTotal;
    oldCpuIdle = newCpuIdle;
    newCpuTotal = 0;
    ifstream if1("/proc/stat");
    char temp[50];
    if1>>temp;
    if1>>temp;
    newCpuTotal += atoi(temp);
    if1>>temp;
    newCpuTotal += atoi(temp);
    if1>>temp;
    newCpuTotal += atoi(temp);
    if1>>temp;
    newCpuTotal += atoi(temp);
    newCpuIdle = atoi(temp);
    if1>>temp;
    newCpuTotal += atoi(temp);
    if1>>temp;
    newCpuTotal += atoi(temp);
    if1>>temp;
    newCpuTotal += atoi(temp);
    if1>>temp;
    newCpuTotal += atoi(temp);
    if1>>temp;
    newCpuTotal += atoi(temp);
    if1>>temp;
    newCpuTotal += atoi(temp);
    if1.close();
}

void curveObject::refreshMemoryInfo()
{
    QRegularExpression matchMessage;
    QFile meminfo("/proc/meminfo");
    if(meminfo.open(QIODevice::ReadOnly))
    {
        QString content(meminfo.readAll());
        matchMessage.setPattern("MemTotal:(.*) kB\n");
        itemList[MemoryTotal] = matchMessage.match(content).captured(1).toUInt();
        matchMessage.setPattern("MemAvailable:(.*) kB\n");
        itemList[MemoryAvailable] = matchMessage.match(content).captured(1).toUInt();
        matchMessage.setPattern("SwapTotal:(.*) kB\n");
        itemList[SwapTotal] = matchMessage.match(content).captured(1).toUInt();
        matchMessage.setPattern("SwapFree:(.*) kB\n");
        itemList[SwapFree] = matchMessage.match(content).captured(1).toUInt();
    }

}
