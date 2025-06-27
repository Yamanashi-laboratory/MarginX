
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

void search_ele(vector<ele_unit> &element, vector<string> &data_cir, vector<vector<judge>> &jud, opt_num *opt, double global, double local_nd, vector<double> &global_rand){

    std::random_device rnd;
    std::mt19937 mt(rnd()); 

    //double sigma = ( element[find_critical(element)].margin_H - element[find_critical(element)].margin_L ) / 2 / 1000 * 5;
    //double sigma = min({-element[find_critical(element)].margin_L, element[find_critical(element)].margin_H}) / 1000 * power;   // 3 ? 5 ? I dont know which number is correct.
    //cout << "sigma = " << sigma << endl;
    //sigma = 0.17;
    std::normal_distribution<> norm(1, local_nd);    // (平均, 標準偏差)
    vector<ele_unit> copy = element;   //現在の各素子の情報elementをcopyにコピー
    stringstream commandname, delete_cir, delete_out;
    commandname << JOSIM_COMMAND << " OPTIMIZE" << getpid() << ".cir > /dev/null"; 
    delete_cir << "rm -f OPTIMIZE" << getpid() << ".cir";
    delete_out << "rm -f CIRCUIT" << getpid() << ".CSV";

    double local;

    double judge = 0;

    for(size_t j = 0; j < copy.size(); j++){
        if(copy[j].FIX != 1){
            
            switch(copy[j].ide_num){
                case 0 :  
                    local = abs(norm(mt));
                    global = global_rand[copy[j].ide_num];
                    copy[j].value = copy[j].value * local * global;
                    break;
                case 1 :  
                    local = abs(norm(mt));
                    global = global_rand[copy[j].ide_num];
                    copy[j].value = copy[j].value * local * global;
                    break;
                case 2 :  
                    local = abs(norm(mt));
                    global = global_rand[copy[j].ide_num];
                    copy[j].value = copy[j].value * local * global;
                    break;
                case 3 :  
                    local = abs(norm(mt));
                    global = global_rand[copy[j].ide_num];
                    copy[j].value = copy[j].value * local * global;
                    break;
                case 4 :  
                    local = abs(norm(mt));
                    global = global_rand[copy[j].ide_num];
                    copy[j].value = copy[j].value * local * global;
                    break;
                case 5 :  
                    local = abs(norm(mt));
                    global = global_rand[copy[j].ide_num];
                    copy[j].value = copy[j].value * local * global;
                    break;
                case 6 :  
                    local = abs(norm(mt));
                    global = global_rand[copy[j].ide_num];
                    copy[j].value = copy[j].value * local * global;
                    break;
                case 7 :  
                    local = abs(norm(mt));
                    global = global_rand[copy[j].ide_num];
                    copy[j].value = copy[j].value * local * global;
                    break;
                default :  break;
            }
            //local = abs(norm(mt));
            //copy[j].value = copy[j].value * local * global;
            //copy[j].value = copy[j].value * local;
    /*
            if(copy[j].value < copy[j].MIN){  //新たなパラメータが最小値を下回っていたらパラメータを最小値に置き換える
                    copy[j].value = copy[j].MIN;
            }
            if(copy[j].value > copy[j].MAX){  //新たなパラメータが最大値を上回っていたらパラメータを最大値に置き換える
                    copy[j].value = copy[j].MAX;
            }
            if(copy[j].FIX == 1){
                copy[j].value = element[j].value;
            }
    */
            synchro_opt(copy, j);
        }
        //cout << copy[j].element << " : " << copy[j].value << endl;
    }
    make_cir_opt(copy,data_cir);
    if(system((commandname.str()).c_str()) == -1){
        cout << "error:1" << endl;
    }


    judge = judge_operation( jud, 0);
    if(judge == 1){ //正常動作したら
        opt->success += 1;
        for(size_t i = 0; i < copy.size(); i++){
        }
    }
    else if(judge == 0){
        //for(int i = 0; i < copy.size(); i++){
            //opt->sum_value_f[i] += copy[i].value;
        //}
    }

/*
    opt->param[m * 100 + i][0] = copy[0].value;
    opt->param[m * 100 + i][1] = copy[1].value;
    opt->param[m * 100 + i][2] = judge;*/
    system((delete_cir.str()).c_str());
    system((delete_out.str()).c_str());
}