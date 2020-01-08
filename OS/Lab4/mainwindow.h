#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <procdlg.h>
#include <QLabel>
#include "getcurve.h"
#include "showcurve.h"
#include "qcustomplot.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintCurve(const QVariantList & property);

private:
    Ui::MainWindow *ui;
    QMenu* menu[10];
    QAction* act[10];
    QMenuBar* menuBar ;
    QStatusBar* status ;
    QLabel *timeStatusLabel;
    QLabel *cpuStatusLabel;
    QLabel *memStatusLabel;
    QLabel *swapStatusLabel;
    curveObject performanceObject;
    void getSystemInfo();
    ProcDlg *procDlg;//new dialog

private slots:
    void updateSystemInfo();
    void updateProcInfo();
    void setInfoLabels();
    void refreshCurve();
    void trigerMenu(QAction* act);
};

#endif // MAINWINDOW_H
