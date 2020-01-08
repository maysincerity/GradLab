#include "procdlg.h"
#include "ui_procdlg.h"
#include <iostream>
#include <QTimer>
#include <QProcess>
#include <fstream>
#include <sstream>
#include <QtDebug>
#include <QString>

using namespace std;

extern int getProcInfo();
extern ProcInfo procInfo[20000];

ProcDlg::ProcDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcDlg)
{
    ui->setupUi(this);

    ui->lineEdit->setPlaceholderText("Input pid(name) to search or path to create process");
    ui->splitter->setStretchFactor(0,8);//set strechfactor

    QTimer *timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(searchProcess()));

    timer->start(1000);

}

void ProcDlg::searchProcess()
{
    QString inMessage;
    QStringList headers;
    QTableWidgetItem *nameItem;
    QTableWidgetItem *pidItem;
    QTableWidgetItem *ppidItem;
    QTableWidgetItem *rssItem;
    QTableWidgetItem *priorityItem;
    inMessage=ui->lineEdit->text();
    int totalProc;
    totalProc=getProcInfo();
    bool flag = false;
    int i=0;
    int j=0;
    int temp[20];
    for(i=0;i<totalProc;i++)
    {
        if(inMessage==QString::fromStdString(procInfo[i].name) || inMessage==QString::fromStdString(procInfo[i].pid)){
            flag=true;//find
            temp[j++]=i;
        }
    }
    if(!flag){
        i=10000;
        ui->tableWidget->setColumnCount(0);//clear
        ui->tableWidget->setRowCount(0);
    }
    else{
        ui->tableWidget->setColumnCount(5);
        ui->tableWidget->setRowCount(j);
        headers<<"name"<<"pid"<<"ppid"<<"rss"<<"priority";
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        for(int k=0;k<j;k++){
            nameItem=new QTableWidgetItem(QString::fromStdString(procInfo[temp[k]].name));
            ui->tableWidget->setItem(k,0,nameItem);
            nameItem->setTextAlignment(Qt::AlignCenter);
            pidItem=new QTableWidgetItem(QString::fromStdString(procInfo[temp[k]].pid));
            ui->tableWidget->setItem(k,1,pidItem);
            pidItem->setTextAlignment(Qt::AlignCenter);
            ppidItem=new QTableWidgetItem(QString::fromStdString(procInfo[temp[k]].ppid));
            ui->tableWidget->setItem(k,2,ppidItem);
            ppidItem->setTextAlignment(Qt::AlignCenter);
            rssItem=new QTableWidgetItem(QString::fromStdString(procInfo[temp[k]].rss));
            ui->tableWidget->setItem(k,3,rssItem);
            rssItem->setTextAlignment(Qt::AlignCenter);
            priorityItem=new QTableWidgetItem(QString::fromStdString(procInfo[temp[k]].priority));
            ui->tableWidget->setItem(k,4,priorityItem);
            priorityItem->setTextAlignment(Qt::AlignCenter);
        }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);;//fit automatically
    }
}

void ProcDlg::killProcess()
{
    QString inMessage;
    inMessage=ui->lineEdit->text();
    int totalProc;
    totalProc=getProcInfo();
    int killCmd=-1;

    for(int i=0;i<totalProc;i++)
    {
        if(inMessage==QString::fromStdString(procInfo[i].name)){
            killCmd=0;
            break;
        }
        if(inMessage==QString::fromStdString(procInfo[i].pid)){
            killCmd=1;
            break;
        }
    }

    QString command;
    switch (killCmd) {
    case 0:
        command=QString("pkill %1").arg(inMessage);
        system(command.toLatin1().data());
        break;
    case 1:
        command=QString("kill %1").arg(inMessage);
        system(command.toLatin1().data());
        break;
    default:
        break;
    }
}

void ProcDlg::createProcess()
{
    QProcess *pro=new QProcess;
    QString newProc;
    newProc=ui->lineEdit->text();
    char message[100];
    int totalProc=getProcInfo();
    int i;
    for(i=0;i<totalProc;i++)
    {
        if(newProc==QString::fromStdString(procInfo[i].name) || newProc==QString::fromStdString(procInfo[i].pid))
            break;
    }
    if(i>=totalProc){
        i=10000;
        string temp;
        temp=newProc.toStdString();
        sprintf(message,"%s",temp.c_str());
    }
    else{
        sprintf(message,"/proc/%s/cwd/%s",procInfo[i].pid.c_str(),procInfo[i].name.c_str());
    }

    newProc=QString::fromStdString(message);

    pro->start(newProc);
}

ProcDlg::~ProcDlg()
{
    delete ui;
}

void ProcDlg::on_search_proc_clicked()
{
    searchProcess();
}

void ProcDlg::on_killProc_clicked()
{
    killProcess();
}

void ProcDlg::on_createProc_clicked()
{
    createProcess();
}
