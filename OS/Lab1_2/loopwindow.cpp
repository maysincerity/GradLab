#include <QString>
#include <QTimer>
#include <cstdio>
#include <unistd.h>
#include "loopwindow.h"

using namespace std;

LoopWindow::LoopWindow(QWidget *parent) : QMainWindow(parent)
{
    move(QPoint(800, 200));
    label = new QLabel(this);
    label->setFixedSize(500,30);
    label->setText("");
    LoopMessage = new char[10];

    //信号和槽
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(LoopUpdate()));
    timer->start(1000);
}

LoopWindow::~LoopWindow(void)
{
    delete label;
    delete LoopMessage;
}

const char* LoopWindow::getLoop(void)
{
    sprintf(LoopMessage,"%d",(cnt++)%10);
    return LoopMessage;
}

void LoopWindow::LoopUpdate(void)
{
    label->setText(QString(getLoop()));
}
