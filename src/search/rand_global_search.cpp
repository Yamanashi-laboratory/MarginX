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

double rand_global_search(vector<ele_unit> &element){

    double sigma = 0.1;
    std::random_device rnd;
    std::mt19937 mt(rnd()); 

    std::normal_distribution<> norm(1, sigma);    // (平均, 分散)

    return  abs(norm(mt));
}