#ifndef GETCURVE_H
#define GETCURVE_H

#include <QObject>
#include "qcustomplot.h"

class myCurve : public QCustomPlot
{
    Q_OBJECT
private:
    QVector<double> time, usage;
    QList<QLabel *> infoLabel;
protected:
    virtual void resizeEvent(QResizeEvent *event);
public:
    myCurve(QWidget * parent = 0);
    void setCurveColor(const QColor & curveColor);
    void setPlotName(const QString & name);
    void setMaximumTime(unsigned int max);
    void setMaximumUsage(double max);
    void setUsageCalibration(const QString & calibration);
    void addData(double data);
    void replot();
};
#endif // GETCURVE_H
