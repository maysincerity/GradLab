#include "getcurve.h"

myCurve::myCurve(QWidget * parent )
:QCustomPlot(parent), time(120), usage(120)
{
    axisRect()->setMinimumMargins(QMargins(0, 20, 0, 20));
    //labels
    for (int i = 0; i < 4; i++)
    {
        QLabel * label = new QLabel(this);
        label->show();
        label->setFixedWidth(100);
        label->setFixedHeight(20);
        label->setStyleSheet("color:rgba(0,0,0,120);");
        infoLabel.append(label);
    }
    //axis
    infoLabel[0]->setAlignment(Qt::AlignLeft);
    infoLabel[2]->setAlignment(Qt::AlignLeft);
    infoLabel[1]->setAlignment(Qt::AlignRight);
    infoLabel[3]->setText("0");
    infoLabel[3]->setAlignment(Qt::AlignRight);
    xAxis2->setVisible(true);
    yAxis2->setVisible(true);
    //no tick lables
    xAxis->setTickLabels(false);
    xAxis2->setTickLabels(false);
    yAxis->setTickLabels(false);
    yAxis2->setTickLabels(false);
    //range reverse
    xAxis->setRangeReversed(true);
    //set tick/subtic transparent
    xAxis->setTickPen(QPen(QColor(255, 255, 255, 0)));
    xAxis->setSubTickPen(QPen(QColor(255, 255, 255, 0)));
    xAxis2->setTickPen(QPen(QColor(255, 255, 255, 0)));
    xAxis2->setSubTickPen(QPen(QColor(255, 255, 255, 0)));
    yAxis->setTickPen(QPen(QColor(255, 255, 255, 0)));
    yAxis->setSubTickPen(QPen(QColor(255, 255, 255, 0)));
    yAxis2->setTickPen(QPen(QColor(255, 255, 255, 0)));
    yAxis2->setSubTickPen(QPen(QColor(255, 255, 255, 0)));
    addGraph();
    int index = 0;
    graph(index)->addData(time, usage);
}

void myCurve::resizeEvent(QResizeEvent *event)
{
    QCustomPlot::resizeEvent(event);//resize
    //set labels
    infoLabel[0]->move(0, 0);
    infoLabel[1]->move(event->size().width() - infoLabel[1]->width(), 0);
    infoLabel[2]->move(0, event->size().height() - infoLabel[2]->height());
    infoLabel[3]->move(event->size().width() - infoLabel[3]->width(), event->size().height() - infoLabel[3]->height());
}

void myCurve::setPlotName(const QString & name)
{
    infoLabel[0]->setText(name);
}

void myCurve::setUsageCalibration(const QString & calibration)
{
    infoLabel[1]->setText(QString("%1 %2").arg(yAxis2->range().upper).arg(calibration));
}

void myCurve::setMaximumTime(unsigned int max)
{
    time.clear();
    for (unsigned int i = 0; i <= max; i++){
        time.append(i);
    }
    //make time&usage same size
    usage.resize(max + 1);
    xAxis->setRange(0, max);
    xAxis2->setRange(0, max);
    infoLabel[2]->setText(QString("%1 s").arg(max));
}

void myCurve::setMaximumUsage(double max)
{
    yAxis->setRange(0, max);
    yAxis2->setRange(0, max);
}

void myCurve::setCurveColor(const QColor & curveColor)
{
    QColor color = curveColor;
    //transparent
    color.setAlpha(100);
    xAxis->grid()->setPen(QPen(color));
    yAxis->grid()->setPen(QPen(color));
    color.setAlpha(255);
    xAxis->setBasePen(QPen(color, 2));
    xAxis2->setBasePen(QPen(color, 2));
    yAxis->setBasePen(QPen(color, 2));
    yAxis2->setBasePen(QPen(color, 2));
    //set pen's color
    int index =0;
    color.setAlpha(255);
    graph(index)->setPen(QPen(color));
    //set brush's color
    color.setAlpha(20);
    graph(index)->setBrush(QBrush(color));
    replot();
}

void myCurve::addData(double data)
{
    usage.removeLast();
    usage.prepend(data);
    int index=0;
    graph(index)->setData(time, usage);
    replot();
}

void myCurve::replot()
{
    QCustomPlot::replot();
}
