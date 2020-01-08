#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QTimer>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <time.h>
#include <dirent.h>
#include <QtDebug>
#include <QDateTime>
#include <unistd.h>
#include <QPainter>
#include "datastructure.h"

using namespace std;

SystemInfo systemInfo;
ProcInfo procInfo[20000];

int getProcInfo();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timeStatusLabel = new QLabel(this);
    cpuStatusLabel = new QLabel(this);
    memStatusLabel = new QLabel(this);
    swapStatusLabel = new QLabel(this);

    //SystemInfo
    getSystemInfo();
    ui->hN_txt->setText(systemInfo.hostName.c_str());
    ui->sT_txt->setText(systemInfo.sysStartTime.c_str());
    ui->rT_txt->setText(systemInfo.sysRunningTime.c_str());
    ui->oV_txt->setText(systemInfo.kernVersion.c_str());
    ui->cpu_txt->setText(systemInfo.cpuModel.c_str());
    ui->core_txt->setText(systemInfo.coreNums.c_str());

    //Update information
    QTimer *timer = new QTimer(this);

    //system information
    connect(timer,SIGNAL(timeout()),this,SLOT(updateSystemInfo()));
    //cpu mem swap
    connect(timer,SIGNAL(timeout()),this,SLOT(setInfoLabels()));
    connect(&performanceObject, &curveObject::updateCurve,this, &MainWindow::paintCurve);
    connect(timer,SIGNAL(timeout()),this,SLOT(refreshCurve()));
    //ProInfo
    connect(timer,SIGNAL(timeout()),this,SLOT(updateProcInfo()));

    timer->start(1000);



    //menubar
    menu[0] = new QMenu("File");
    menu[0]->addAction("Search Proccess");
    menu[0]->addSeparator();
    menu[0]->addAction("Shutdown");
    menuBar = new QMenuBar(this);
    menuBar->addMenu(menu[0]);
    menuBar->setGeometry(0,0,2000,25);
    connect(menuBar,SIGNAL(triggered(QAction*)),this,SLOT(trigerMenu(QAction*)));//react to click
}

void MainWindow::trigerMenu(QAction* act)
{
    if(act->text() == "Search Proccess")
    {
        procDlg = new ProcDlg(this);
        procDlg->setWindowTitle("Search Proccess");
        procDlg->show();
    }
    else if(act->text() == "Shutdown")
    {
        if(QMessageBox::Yes == QMessageBox::warning(this,"Shutdown","Are you sure to continue?",QMessageBox::Yes|QMessageBox::Cancel)){
            QStringList arguments;
            arguments << "-h" << "now";
            QProcess::execute("shutdown", arguments);
        }
    }
}


/**update system information
 */
void MainWindow::updateSystemInfo(){
    getSystemInfo();
    ui->hN_txt->setText(systemInfo.hostName.c_str());
    ui->sT_txt->setText(systemInfo.sysStartTime.c_str());
    ui->rT_txt->setText(systemInfo.sysRunningTime.c_str());
    ui->oV_txt->setText(systemInfo.kernVersion.c_str());
    ui->cpu_txt->setText(systemInfo.cpuModel.c_str());
}



/**update proc information
 */
void MainWindow::updateProcInfo(){
    QStringList headers;
    QTableWidgetItem *nameItem;
    QTableWidgetItem *pidItem;
    QTableWidgetItem *ppidItem;
    QTableWidgetItem *rssItem;
    QTableWidgetItem *priorityItem;
    int totalProc;//total num of proccesses
    totalProc=getProcInfo();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(totalProc);
    headers<<"name"<<"pid"<<"ppid"<<"rss"<<"priority";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    for(int i=0;i<totalProc;i++)
    {
        nameItem=new QTableWidgetItem(QString::fromStdString(procInfo[i].name));
        ui->tableWidget->setItem(i,0,nameItem);
        nameItem->setTextAlignment(Qt::AlignCenter);
        pidItem=new QTableWidgetItem(QString::fromStdString(procInfo[i].pid));
        ui->tableWidget->setItem(i,1,pidItem);
        pidItem->setTextAlignment(Qt::AlignCenter);
        ppidItem=new QTableWidgetItem(QString::fromStdString(procInfo[i].ppid));
        ui->tableWidget->setItem(i,2,ppidItem);
        ppidItem->setTextAlignment(Qt::AlignCenter);
        rssItem=new QTableWidgetItem(QString::fromStdString(procInfo[i].rss));
        ui->tableWidget->setItem(i,3,rssItem);
        rssItem->setTextAlignment(Qt::AlignCenter);
        priorityItem=new QTableWidgetItem(QString::fromStdString(procInfo[i].priority));
        ui->tableWidget->setItem(i,4,priorityItem);
        priorityItem->setTextAlignment(Qt::AlignCenter);
    }
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);// cannot edit
    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows);//select a row
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);;//fit automatically
}


/**get system information
 */
void MainWindow::getSystemInfo(){
    string info;
    string temp;

    //hostname
    ifstream if1("/proc/sys/kernel/hostname");
    if1>>info;
    systemInfo.hostName=info;
    if1.close();

    //start time
    time_t curTime;
    time_t startTime;
    long upTime;
    struct tm *ptm=NULL;
    char message[100];

    ifstream if2("/proc/uptime");
    if2>>upTime;
    curTime = time(NULL);
    if(curTime > upTime){
        startTime=curTime-upTime;
    }
    else{
        startTime=upTime-curTime;
    }
    ptm=gmtime(&startTime);
    sprintf(message,"%d-%-d-%d %d:%d:%d",ptm->tm_year + 1900,ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour+8, ptm->tm_min, ptm->tm_sec);
    info =message;
    systemInfo.sysStartTime=info;
    if2.close();

    //running time
    ifstream if3("/proc/uptime");
    long runTime;
    if3>>runTime;
    sprintf(message,"%ld d %ld h %ld m %ld s",runTime / 86400,(runTime % 86400)/3600,(runTime % 3600)/60,runTime % 60);
    info = message;
    systemInfo.sysRunningTime=info;
    if3.close();

    //kernel version
    ifstream if4("/proc/sys/kernel/ostype");
    if4>>info;
    if4.close();
    ifstream if5("/proc/sys/kernel/osrelease");
    if5>>temp;
    if5.close();
    systemInfo.kernVersion=info+" "+temp;

    //cpu info
    ifstream if6("/proc/cpuinfo");
    while(if6>>info)
    {
        if(info=="name")
        {
            if6>>info>>info;
            systemInfo.cpuModel = info;
            while(if6>>info&&info!="stepping")
            {
                systemInfo.cpuModel += " "+info;
            }
            continue;
        }
        if(info=="MHz")
        {
            if6>>info>>info;
            systemInfo.cpuModel += " /" + info + "MHz";
            break;
        }
    }
    if6.close();

    //get the amount of cpu core
    ifstream if7("/proc/cpuinfo");
    int core_temp=0;
    while(if7>>info)
    {
        if(info=="processor")
        {
            core_temp++;
            continue;
        }
    }
    sprintf(message,"%d",core_temp);
    systemInfo.coreNums=message;
    if7.close();
}


/**get proc information
 */
int getProcInfo(){
    DIR *dir;
    struct dirent *dirP;
    int i=0;
    if(!(dir=opendir("/proc"))){
        return 0;
    }
    while((dirP=readdir(dir))!=false)
    {
        if(dirP->d_name[0]>='1' && dirP->d_name[0]<='9')
        {
            string temp,procName;
            char pFileAddr[20];
            sprintf(pFileAddr,"/proc/%s/stat",dirP->d_name);
            ifstream infile(pFileAddr);
            infile>>(procInfo[i].pid);
            infile>>temp;
            procName=temp;
            if(temp.find(')')==temp.npos){//solve problkem like (Web Content)
                infile>>temp;
                procName=procName+" "+temp;
            }
            //solve probem like ((sd-pam))
            procInfo[i].name=procName.substr(procName.find('(',1)==temp.npos?1:2,procName.find(')')-(procName.find('(',1)==temp.npos?1:2));
            infile>>temp;
            infile>>(procInfo[i].ppid)>>temp>>temp>>temp>>temp>>temp>>temp>>temp>>temp>>temp>>temp>>temp>>temp>>temp;
            infile>>(procInfo[i].priority);//18
            infile>>temp>>temp>>temp>>temp>>temp;
            //get memeory useage
            float tempMem;
            char tMem[100];
            infile>>tempMem;
            tempMem = tempMem/1024;
            sprintf(tMem,"%.1f MB",tempMem);
            procInfo[i].rss=tMem;
            infile.close();
            i++;
        }
    }
    closedir(dir);
    return i;
}

void MainWindow::setInfoLabels()
{
    //cpu curve
    ui->cpuUsgCurve->setCurveColor(QColor("#87CEFA"));
    ui->cpuUsgCurve->setPlotName("% Usage");
    ui->cpuUsgCurve->setMaximumTime(120);
    ui->cpuUsgCurve->setMaximumUsage(100);
    ui->cpuUsgCurve->setUsageCalibration("%");

    //memory curve
    ui->memUsgCurve->setCurveColor(QColor("#9400D3"));
    ui->memUsgCurve->setPlotName("Memory Usage");
    ui->memUsgCurve->setMaximumTime(120);
    QRegularExpression temp;
    QFile meminfo("/proc/meminfo");
    if(meminfo.open(QIODevice::ReadOnly))
    {
        QString content(meminfo.readAll());
        temp.setPattern("MemTotal: (.*) kB\n");
        double totalMem = temp.match(content).captured(1).toDouble();
        totalMem = totalMem / 1024 / 1024;
        totalMem = (int)(totalMem * 10) / 10.0;
        ui->memUsgCurve->setMaximumUsage(totalMem);

        temp.setPattern("SwapTotal: (.*) kB\n");
        double totalSwap = temp.match(content).captured(1).toDouble();
        totalSwap = totalSwap / 1024 ;
        totalSwap = (int)(totalSwap * 10) / 10.0;
        ui->swapCurve->setMaximumUsage(totalSwap);
    }
    ui->memUsgCurve->setUsageCalibration(" GB");

    //swap curve
    ui->swapCurve->setCurveColor(QColor("#66CDAA"));
    ui->swapCurve->setPlotName("Swap Area");
    ui->swapCurve->setMaximumTime(120);
    ui->swapCurve->setUsageCalibration(" MB");

}


void MainWindow::paintCurve(const QVariantList &property)
{
    //Cpu usage
    ui->cpuUsgCurve->addData(property[curveObject::CpuUsage].toDouble());
    //Mem info
    double totalMem = property[curveObject::MemoryTotal].toDouble();
    double availableMem = property[curveObject::MemoryAvailable].toDouble();
    double usedMem = totalMem - availableMem;
    usedMem /= 1024*1024;
    usedMem = (int)(usedMem*10)/10.0;
    totalMem/= 1024*1024;
    totalMem = (int)(totalMem*10)/10.0;
    ui->memUsgCurve->addData(usedMem);
    //swap info
    double totalSwap = property[curveObject::SwapTotal].toDouble();
    totalSwap /= 1024;
    totalSwap = (int)(totalSwap*10)/10.0;
    double freeSwap = property[curveObject::SwapFree].toDouble();
    freeSwap /= 1024;
    freeSwap = (int)(freeSwap*10)/10.0;
    ui->swapCurve->addData(totalSwap - freeSwap);
    //status bar
    QTime time=QTime::currentTime();
    QString currentTime;
    currentTime=time.toString("hh:mm:ss");
    timeStatusLabel->setText("Time: " + currentTime + " ");
    cpuStatusLabel->setText("CPU Usage: " + QString::number(property[curveObject::CpuUsage].toUInt()) + "% ");
    memStatusLabel->setText("Mem Usage: " + QString("%1/%2 GB").arg(usedMem).arg(totalMem));
    swapStatusLabel->setText("Swap Usage: "+ QString("%1/%2 MB").arg(totalSwap - freeSwap).arg(totalSwap));
    ui->statusBar->addWidget(timeStatusLabel);
    ui->statusBar->addWidget(cpuStatusLabel);
    ui->statusBar->addWidget(memStatusLabel);
    ui->statusBar->addWidget(swapStatusLabel);
}


void MainWindow::refreshCurve()
{
    performanceObject.refresh();
}

MainWindow::~MainWindow()
{
    delete ui;
}
