#include <QTimer>
#include <QString>
#include <ctime>
#include "timewindow.h"

using namespace std;

TimeWindow::TimeWindow(QWidget *parent) : QMainWindow(parent)
{
    move(QPoint(600, 200));
    label = new QLabel(this);
    label->setFixedSize(500,50);
    label->setText("");

    //信号和槽
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimeUpdate()));
    timer->start(1000);
}

TimeWindow::~TimeWindow(void)
{
    delete label;
}

const char* TimeWindow::getTime(void) {
    time_t ct;
    ct = time(NULL);
    return ctime(&ct);
}

void TimeWindow::TimeUpdate(void) {
    label->setText(QString(getTime()));
}
