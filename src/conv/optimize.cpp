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

/*
void opt_ele2(vector<ele_unit> &element, vector<string> &data_cir, ele_cou *cou, vector<vector<judge>> &jud, opt_num *opt, gauss *gau){

    std::random_device rnd;
    std::mt19937 mt(rnd()); 
    std::normal_distribution<> norm(1, 0.05);    // (平均, 分散)
    vector<ele_unit> copy = element;   //現在の各素子の情報elementをcopyにコピー
    stringstream commandname, delete_cir, delete_out;
            commandname << "josim OPTIMIZE" << getpid() << ".cir > /dev/null"; 
    delete_cir << "rm -f OPTIMIZE" << getpid() << ".cir";
    delete_out << "rm -f CIRCUIT" << getpid() << ".CSV";

    double local_L = abs(norm(mt));
    double local_K = abs(norm(mt));    
    double local_B = abs(norm(mt));    
    double local_BI = abs(norm(mt));    
    double local_PI = abs(norm(mt));    
    double local_R = abs(norm(mt));    
    double local_V = abs(norm(mt));   
    double local_I = abs(norm(mt));    
    //double local;



    for(int j = 0; j < copy.size(); j++){
        switch(copy[j].ide_num){
            case 0 :  
                copy[j].value = copy[j].value * gau->global_L * local_L;
                break;
            case 1 :  
                copy[j].value = copy[j].value * gau->global_K * local_K;
                break;
            case 2 :  
                copy[j].value = copy[j].value * gau->global_B * local_B;
                break;
            case 3 :  
                copy[j].value = copy[j].value * gau->global_BI * local_BI;
                break;
            case 4 :  
                copy[j].value = copy[j].value * gau->global_PI * local_PI;
                break;
            case 5 :  
                copy[j].value = copy[j].value * gau->global_R * local_R;
                break;
            case 6 :  
                copy[j].value = copy[j].value * gau->global_V * local_V;
                break;
            case 7 :  
                copy[j].value = copy[j].value * gau->global_I * local_I;
                break;
            default :  break;
        }
        //cout << copy[j].element << " : " << copy[j].value << endl;
    }
    make_cir_opt(copy,data_cir,cou);
    if(system((commandname.str()).c_str()) == -1){
        cout << "error:1" << endl;
    }
    else if(judge_operation( jud, 0) == 1){ //正常動作したら
        opt->success += 1;
        for(int i = 0; i < copy.size(); i++){
            opt->sum_value[i] += copy[i].value;
        }
    }

    system((delete_cir.str()).c_str());
    system((delete_out.str()).c_str());



}
*/