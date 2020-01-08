#include <QTimer>
#include <QString>
#include <cstdio>
#include "sumwindow.h"

using namespace std;

SumWindow::SumWindow(QWidget *parent) : QMainWindow(parent)
{
    move(QPoint(1000, 200));
    label = new QLabel(this);
    label->setFixedSize(500,30);
    label->setText("");
    sum = 0;
    adder = 0;
    SumMessage = new char[10];

    //信号和槽
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(SumUpdate()));
    timer->start(1000);
}

SumWindow::~SumWindow(void)
{
    delete label;
    delete SumMessage;
}

const char *SumWindow::getSum(void) {
    if (adder < 1000) {
        adder++;
        sum += adder;
        sprintf(SumMessage,"%d", sum);
    }

    return SumMessage;
}

void SumWindow::SumUpdate(void) {
    label->setText(QString(getSum()));
}
