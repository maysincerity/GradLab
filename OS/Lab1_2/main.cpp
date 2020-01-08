#include <QApplication>
#include <unistd.h>
#include <sys/wait.h>
#include "timewindow.h"
#include "loopwindow.h"
#include "sumwindow.h"

int main(int argc, char *argv[])
{
    pid_t timewindow_pid, loopwindow_pid, sumwindow_pid;

    while ((timewindow_pid = fork()) == -1);
    if (timewindow_pid == 0) {
        QApplication Qapp_time(argc, argv);
        TimeWindow timewindow;
        timewindow.show();
        Qapp_time.exec();
    } else {
        while ((loopwindow_pid = fork()) == -1);
        if (loopwindow_pid == 0) {
            QApplication Qapp_loop(argc, argv);
            LoopWindow loopwindow;
            loopwindow.show();
            Qapp_loop.exec();
        } else {
            while ((sumwindow_pid = fork()) == -1);
            if (sumwindow_pid == 0) {
                QApplication Qapp_sum(argc, argv);
                SumWindow sumwindow;
                sumwindow.show();
                Qapp_sum.exec();
            } else {
                for (int i = 0; i < 3; i++) {
                    waitpid(-1, NULL, 0);
                }
            }
        }
    }
}
