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

void search(vector<ele_unit> &element, vector<string> &data_cir, vector<vector<judge>> &jud, vector<string> &arg_arr){

    int mode = 0;
    double CM_power = 1;
    double BM_power = 1;
    int count = 0;
    int bias_margin = 0;
    double yield_ave = 0;
    double local_nd = 0.05;
    double lambda, global = 0;
    int not_upd = 0;
    int bunbo = 100;
    int suc_th = 60;
    int p_key = 0, m_key = 0, pm_key = 0;
    time_t start, now;
    string sharp = "";
    string yield = "yield.csv";
    string param = "param.csv";
    ofstream fp_yield(yield);
    ofstream fp_param(param);
    //vector<int> yield_log;
    //yield_log.push_back(50);
    double therm = 1;
    vector<int> yield_his;
    yield_his.resize(5,0);
    vector<double> global_rand;
    global_rand.resize(8, 0);
    vector<double> power;
    //cout << yield_log[0] << endl;



    start = time(NULL);

    
    opt_num *opt;
    int shmid;
    int Margin_num = 1;
    vector<ele_unit> element_ini = element;
    //opt->suc_max = 0;
    cout <<  endl;

    if ((shmid = shmget(IPC_PRIVATE, sizeof(opt_num), 0666)) == -1) {
        cout << "can't make shared memory" << endl;
        exit(EXIT_FAILURE);
    }
    //結果を格納する opt に共有メモリをアタッチ
    opt = (opt_num *)shmat(shmid, NULL, 0);

    // first margin check
    opt->suc_max = 0; 
    cout << " The first margin check   (the " << GetOrdinalNumber(static_cast<int>(Margin_num)) 
                 << " check)                                                                                             " << endl;
    //Margin_low(element,  jud, data_cir, arg_arr, 2);
    //make_cir_last(element, data_cir, arg_arr);            
    Margin_num++;

    //local_nd の初期値は最適化前のクリティカルマージンの値の半分(%)
    local_nd = 0.2;
    //local_nd = 0.1;

    for (int m = 0; m < MONTE_CARLO; m++){
        for (int r = 0; r < global_rand.size(); r++){
            global_rand[r] = rand_global_yield(local_nd);
        }
        //cout << endl;

        //double global = rand_global_yield(local_nd);  //グローバルな乱数計算 

        //cout << " global_rand = " << global << endl;

        //マルチプロセス開始
        vector<int> pid;        
        for (int i = 0; i < MULTI_NUM; i++) { // MULTI_NUM = 1回のモンテカルロで生成するマルチプロセス数
                pid.emplace_back(fork());
                if(pid[i] == 0){       
                    opt_num  *shmaddr;
                    if ((shmaddr = (opt_num*)shmat(shmid, NULL, 0)) == (void *)-1) {
                        cout << "childL can't load memory" << endl;  
                        exit(EXIT_FAILURE);
                    }
                    search_ele(element,data_cir,jud,shmaddr, global, local_nd, global_rand);
                    if (shmdt(shmaddr) == -1) {
                        exit(EXIT_FAILURE);
                    }
                    exit(0);
                }
                else if(pid[i] < 0){
                    cout << "can't make child process" << endl;
                    exit(0);
                }

        }
        //子プロセスの終了を待機
        for (int i = 0; i < MULTI_NUM; i++) {
            wait(NULL);
        }

        now = time(NULL);
        double progress = static_cast<double>(m + 1) / static_cast<double>(MONTE_CARLO) * 100;
        if(static_cast<int>(progress) - count >= 2){
            sharp += "O";
            count = static_cast<int>(progress);
        }
        //cout << "\x1B[1K";
        cout << " Optimizing...   "   <<  right << setw(5) <<static_cast<int>(static_cast<double>(now - start)) << " seconds elapsed"
                                                    << "   ( success : " << opt->success << " )" << ", ND = " << local_nd << endl;
        cout << "\x1B[1B";    //カーソルを１行下に移動させる
        cout << "\x1B[1A";    //カーソルを１行上に移動させる

        
        fp_yield << m + 1 << ", " << opt->success << ", " << local_nd << ", " << element[13].value << endl;
        //yield_log.push_back(opt->success);


        if( opt->success == 0 ){    // 開発者確認用
            lambda = (MULTI_NUM - opt->success) / MULTI_NUM;
            for(int i = 0; i < element.size(); i++){
                if(element[i].value < element[i].MIN){  //新たなパラメータが最小値を下回っていたらパラメータを最小値に置き換える
                        element[i].value = element[i].MIN;
                }
                if(element[i].value > element[i].MAX){  //新たなパラメータが最大値を上回っていたらパラメータを最大値に置き換える
                        element[i].value = element[i].MAX;
                }
                if(element[i].FIX == 1){
                    element[i].value = element_ini[i].value;
                }
            }
        }
        else if(opt->success > 0){
            for(int i = 0; i < element.size(); i++){
                element[i].value = opt->best_value[i];
            }
            break;
        }

        


        
        //全て初期化
        opt->success = 0;
        for(int i = 0; i < element.size(); i++){
            opt->sum_value[i] = 0;
        }
        //初期化終了


    }
    
    //END:
    Margin_low(element,  jud, data_cir, arg_arr, 2);
    //cri_bias_sum = min({-element[find_critical_bias(element)].margin_L, element[find_critical_bias(element)].margin_H});
    //cout << "cri_bias_sum : " << cri_bias_sum << endl; 
    for(int i = 0; i < element.size(); i++){
        element[i].value = opt->best_value[i];  //最終的な最良の回路のパラメータをelement配列に格納
    }

    //critical_margin_method(element,  jud, data_cir, arg_arr);

    Margin_low(element,  jud, data_cir, arg_arr, 0);

    make_cir_last(element, data_cir, arg_arr);

    //fp_param << element[0].element << ", " << element[1].element << ", " << "judge" << endl;

/*
    for(int col = 0; col < 50000; col++){
        for(int row = 0; row < 3; row++){
            if(row != 2){
                fp_param << (opt->param[col][row] - element_ini[row].value) / element_ini[row].value * 100;
                fp_param << ", ";
            }
            else{
                fp_param << opt->param[col][row];
            }
        }
        fp_param << endl;
    }
*/
    /*確保していた共有メモリを解放*/
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        cout << "can't delete shared memory" << endl;
        exit(EXIT_FAILURE);
    }
    fp_yield.close();        

}


