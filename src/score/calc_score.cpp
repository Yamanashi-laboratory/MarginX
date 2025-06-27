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


double calc_score(vector<ele_unit> &element, vector<double> power){
    double score = 0;   // power : [CM_power, BM_power, upper_power, lower_power]
    score = power[0] * calc_critical(element) + power[1] * calc_critical_bias(element) 
            + power[2] * calc_critical_upper(element) + power[3] * calc_critical_lower(element);

    return score;
}