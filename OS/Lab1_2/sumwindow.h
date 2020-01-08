#ifndef SUMWINDOW_H
#define SUMWINDOW_H

#include <QMainWindow>
#include <QLabel>

class SumWindow : public QMainWindow
{
    Q_OBJECT

public:
    SumWindow(QWidget *parent = 0);
    ~SumWindow(void);

private:
    QLabel *label;
    int sum;
    int adder;
    char *SumMessage;
    QMainWindow &setText(const char *txt);
    const char *getSum(void);

private slots:
    void SumUpdate(void);
};

#endif
