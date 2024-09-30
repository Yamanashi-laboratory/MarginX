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

double calc_critical_bias(vector<ele_unit> &element){
    double critical = 100;
    int cri_num = 0;
    for(size_t j = 0; j < element.size(); j++){
        if(element[j].ide_num == 6 || element[j].ide_num == 7){
            if(-element[j].margin_L < critical){
                critical = -element[j].margin_L;
                cri_num = j;
            }
            if(element[j].margin_H < critical){
                critical = element[j].margin_H;
                cri_num = j;
            }
        }
    }
    return critical;
}

