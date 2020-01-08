#ifndef TIMEWINDOW_H
#define TIMEWINDOW_H

#include <QMainWindow>
#include <QLabel>

class TimeWindow : public QMainWindow
{
    Q_OBJECT

public:
    TimeWindow(QWidget *parent = 0);
    ~TimeWindow(void);

private:
    QLabel *label;
    QMainWindow &setText(const char *txt);
    const char* getTime(void);

private slots:
    void TimeUpdate(void);
};

#endif
