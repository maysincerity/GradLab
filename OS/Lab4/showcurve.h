#ifndef SHOWCURVE_H
#define SHOWCURVE_H

#include <QObject>
#include <QVariantList>
#include <QFile>
#include <QRegularExpression>
#include <QTime>

class curveObject : public QObject
{
    Q_OBJECT
private:
    QVariantList itemList;
    unsigned long oldCpuTotal;
    unsigned long newCpuTotal;
    unsigned long oldCpuIdle;
    unsigned long newCpuIdle;
    void refreshCpuUsage();
    void refreshMemoryInfo();

public:
    enum CurveInfo
    {
        CpuUsage, MemoryTotal, MemoryAvailable, SwapTotal, SwapFree, infoNums
    };
    curveObject(QObject * parent = 0);
    void refresh();

signals:
    void updateCurve(const QVariantList & property);
};
#endif // SHOWCURVE_H
