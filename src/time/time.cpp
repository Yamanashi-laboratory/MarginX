#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fstream>
#include <iomanip> 
#include <string>
#include <sstream>
#include <filesystem>
#include "function.hpp"

using namespace std;

void end_time(time_t start, time_t end){
    char date[100];                   //日付・日時を格納する文字列配列
    struct tm *tps, *tpe;             //開始時間と終了時間

    tps = localtime(&start);
    strftime(date, sizeof(date), "%Y/%m/%d %H:%M:%S", tps);

    time(&end);
    tpe = localtime(&end);
    strftime(date, sizeof(date), "%Y/%m/%d %H:%M:%S", tpe);
    /*終了時刻を表示*/
    cout << "\n END      : " << date << endl;

    /*経過時刻を⚪︎時間⚪︎分⚪︎秒のかたちで表示*/
    int hour = 0, min = 0, sec = end - start;
    if (sec >= 3600) {
        hour = sec / 3600;
        sec -= hour * 3600;
    }
    if (sec >= 60) {
        min = sec / 60;
        sec -= min * 60;
    }
    cout.fill('0'); 
    cout << " RUN TIME : " << setw(2) << hour << ":"  << setw(2) << min << ":"  << setw(2) << sec << endl << endl;
    cout.fill(' ');

}