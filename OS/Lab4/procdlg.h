#ifndef PROCDLG_H
#define PROCDLG_H

#include <QDialog>
#include <datastructure.h>

namespace Ui {
class ProcDlg;
}

class ProcDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ProcDlg(QWidget *parent = 0);
    ~ProcDlg();

private:
    Ui::ProcDlg *ui;
private slots:
    void searchProcess();
    void killProcess();
    void createProcess();
    void on_search_proc_clicked();
    void on_killProc_clicked();
    void on_createProc_clicked();
};

#endif // PROCDLG_H
