#ifndef LOOPWINDOW_H
#define LOOPWINDOW_H

#include <QMainWindow>
#include <QLabel>

class LoopWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoopWindow(QWidget *parent = 0);
    ~LoopWindow(void);

private:
    QLabel *label;
    int cnt;
    char *LoopMessage;
    QMainWindow &setText(const char *txt);
    const char* getLoop(void);

private slots:
    void LoopUpdate(void);
};

#endif
