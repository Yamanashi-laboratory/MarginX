#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <chrono> 
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <mutex>
#include "function.hpp"
#include <iomanip> 

using namespace std;


int get_digit(double num){  //桁数を取得する関数 (□ × 10 ^ ○) の ○ の部分を取得
    double digit;  //出力する桁数
    if(num == 0){
        return 0;
    }
    digit = floor(log10(abs(num))); //入力numの絶対値を常用対数にいることで桁数を取得
    return digit;
}
