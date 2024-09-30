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

void optimize_yield_up(vector<ele_unit> &element, vector<string> &data_cir, vector<vector<judge>> &jud, vector<string> &arg_arr){

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
    //cout << yield_log[0] << endl;

    cout << " Select the Kind of Score" << endl;
    cout << " 1: Only Critical Margin " << endl;
    cout << " 2: Only Bias Margin" << endl;
    cout << " 3: The Sum of Critical Margin and Bias Margin" << endl;
    cout << " 4: The Sum of Critical Margin and Bias Margin * 2 " << endl;
    cout << " 5: Others ( input yourself )" << endl << endl;
    cout << "  Selected Score : ";
    cin >> mode;

    switch(mode){
        case 1:
            CM_power = 1;
            BM_power = 0;
            break;
        case 2:
            CM_power = 0;
            BM_power = 1;
            break;
        case 3:
            CM_power = 1;
            BM_power = 1;
            break;
        case 4:
            CM_power = 1;
            BM_power = 2;
            break;
        case 5:
            cout << " Select Critical Margin Power" << endl;
            cout << " Critical Margin Power : ";
            cin >> CM_power;
            cout << " Select Bias Margin Power" << endl;
            cout << " Bias Margin Power : ";
            cin >> BM_power;
            break;
        default:
            cout << " Please Select a Correct Number" << endl;
            return;
            break;
    }

    start = time(NULL);

    
    opt_num *opt;
    int shmid;
    double cri_bias_sum = 0;
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
    Margin_low(element,  jud, data_cir, arg_arr, 2);
    make_cir_last(element, data_cir, arg_arr);            
    bias_margin =  min({-element[find_critical_bias(element)].margin_L, element[find_critical_bias(element)].margin_H});
    cri_bias_sum = CM_power * min({-element[find_critical(element)].margin_L, element[find_critical(element)].margin_H}) + BM_power * bias_margin;
    Margin_num++;

    //local_nd の初期値は最適化前のクリティカルマージンの値の半分(%)
    local_nd = round(min({-element[find_critical(element)].margin_L, element[find_critical(element)].margin_H}) / 2) / 100;
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
                    opt_ele_yield(element,data_cir,jud,shmaddr, global, local_nd, global_rand);
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
                                                    << "   ( success : " << opt->success << " )" << ", ND = " << local_nd << ", not_upd = " << not_upd << ", average =  "<<  reduce(begin(yield_his), end(yield_his)) / yield_his.size() << endl;
        cout << "\x1B[1B";    //カーソルを１行下に移動させる
        cout << "\x1B[1A";    //カーソルを１行上に移動させる

        
        fp_yield << m + 1 << ", " << opt->success << ", " << local_nd << ", " << element[13].value << endl;
        //yield_log.push_back(opt->success);

        yield_his[m % yield_his.size()] = opt->success;
        not_upd++;   //歩留まりが向上しなかったら、opt_num を+1する
        bunbo = MULTI_NUM - opt->success;
        if(opt->success == 100){
            bunbo = 1;
        }

        if( opt->success != 0 ){    // 開発者確認用
            lambda = (MULTI_NUM - opt->success) / MULTI_NUM;
            for(int i = 0; i < element.size(); i++){

                element[i].value = opt->sum_value[i] / opt->success + lambda * (element[i].value - opt->sum_value_f[i] / bunbo);   // 新たなパラメータに置き換える
                //element[i].value = opt->sum_value[i] / opt->success;
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

            if( opt->success > opt->suc_max){   // > か >= で大きく変わる
                //not_upd = 0;
                opt->suc_max = opt->success;

                for(int i = 0; i < element.size(); i++){
                    //element[i].value = opt->sum_value[i] / opt->success;
                    //element[i].value = opt->sum_value[i] / opt->success + lambda * (element[i].value - opt->sum_value_f[i] / bunbo);   // 新たなパラメータに置き換える
                    //opt->best_value[i] =element[i].value;
                }
            }
        }

        //cout << reduce(begin(yield_his), end(yield_his)) / yield_his.size() << endl;
        if(reduce(begin(yield_his), end(yield_his)) / yield_his.size() >= suc_th){  //直近 5 サイクルの歩留まり(success) の平均が 60 を超えたら、標準偏差を +0.01
            local_nd += 0.01;
            Margin_low(element,  jud, data_cir, arg_arr, 2);
            not_upd = 0;
            yield_his.assign(yield_his.size(), 0);

            cri_bias_sum = CM_power * min({-element[find_critical(element)].margin_L, element[find_critical(element)].margin_H}) + BM_power * min({-element[find_critical_bias(element)].margin_L, element[find_critical_bias(element)].margin_H});
            if( cri_bias_sum > opt->cri_bias_best ){   // マージンの評価値 cri_bias_sum が最大だったらそれを現時点の最良の回路として置き換える
                    for(int j = 0; j < element.size(); j++){
                        opt->best_value[j] = element[j].value;    // best_value配列に現時点の最良の回路のパラメータを格納
                    }
                    opt->best_value[element.size()] = Margin_num;  //best_value配列の最後の要素の次の要素に何番目の回路かを格納
                    opt->cri_bias_best = cri_bias_sum;   //cri_bias_best を更新
            }
            Margin_num++;

        }
        if(not_upd >= 100){  //直近 5 サイクルの歩留まり(success) の平均が 60 を超えない状況が50 回続いたら、標準偏差を -0.01
            not_upd = 0;
            yield_his.assign(yield_his.size(), 0);
            break;
            //goto END;
        }
        
        //全て初期化
        opt->success = 0;
        for(int i = 0; i < element.size(); i++){
            opt->sum_value[i] = 0;
            opt->sum_value_f[i] = 0;
        }
        //初期化終了


    }
    
    //END:
    Margin_low(element,  jud, data_cir, arg_arr, 2);
    //cri_bias_sum = min({-element[find_critical_bias(element)].margin_L, element[find_critical_bias(element)].margin_H});
    cri_bias_sum = CM_power * min({-element[find_critical(element)].margin_L, element[find_critical(element)].margin_H}) + BM_power * min({-element[find_critical_bias(element)].margin_L, element[find_critical_bias(element)].margin_H});
    // cri_bias_sum = その回路のマージンの評価 (クリティカルマージン + 2 * バイアスマージン)
    //cout << "cri_bias_sum : " << cri_bias_sum << endl; 
    if( cri_bias_sum > opt->cri_bias_best ){   // マージンの評価値 cri_bias_sum が最大だったらそれを現時点の最良の回路として置き換える
        for(int j = 0; j < element.size(); j++){
            opt->best_value[j] = element[j].value;    // best_value配列に現時点の最良の回路のパラメータを格納
        }
        opt->best_value[element.size()] = Margin_num;  //best_value配列の最後の要素の次の要素に何番目の回路かを格納
        opt->cri_bias_best = cri_bias_sum;   //cri_bias_best を更新
    }

    cout <<  " This is the " << GetOrdinalNumber(static_cast<int>(opt->best_value[element.size()])) << " value." << endl;
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

