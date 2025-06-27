#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
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
#include <iomanip> 
#include <cmath>
#include <random>
#include <regex>
#include <algorithm>
#include <ctime>
#include <filesystem>
#include <fstream>
#include "function.hpp"

using namespace std;

string GetOrdinalNumber(int num){
    string res = to_string(num);
    if(num == 11 || num == 12){
        res += "th";
    }
    else{
        switch (num % 10){
        case 1:
            res += "st";
            break;
        case 2:
            res += "nd";
            break;

        case 3:
            res += "rd";
            break;

        default:
            res += "th";
            break;
        }
    }
    return res;
} 