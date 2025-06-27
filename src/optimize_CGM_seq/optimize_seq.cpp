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

        // 書き換えが競合している可能性有
         // 並列処理ではないoptimize_monteを作って検証

void optimize_seq(vector<ele_unit> &element, vector<string> &data_cir, vector<vector<judge>> &jud, vector<string> &arg_arr){

    int count = 0;
    double local_nd = 0;
    double lambda, global = 0;;
    int not_upd = 0;
    int suc_th = 60;
    int bunbo = 100;
    time_t start, now;
    string sharp = "";
    string yield = "yield.csv";
    string param = "param.csv";
    ofstream fp_yield(yield);
    ofstream fp_param(param);
    vector<int> yield_log;
    yield_log.push_back(50);
    //cout << yield_log[0] << endl;
    vector<int> yield_his;
    yield_his.resize(5,0);
    vector<double> global_rand;  //素子の種類ごとにグローバルな乱数を格納
    global_rand.resize(8, 0);
    vector<double> power;

    power = select_score();   //スコアを選択 → power配列に格納

    start = time(NULL);
    
    opt_num *opt = new opt_num;
    double cri_bias_sum = 0;
    int Margin_num = 1;
    vector<ele_unit> element_ini = element;
    //opt->suc_max = 0;
    cout <<  endl;

    opt->suc_max = 0; 
    cout << " This is the progress midway   (the " << GetOrdinalNumber(static_cast<int>(Margin_num)) 
                 << " check)                                                                                             " << endl;
    Margin_low(element,  jud, data_cir, arg_arr, 2);
    cri_bias_sum = calc_score(element, power);
    Margin_num++;
    //ローカルな乱数をクリティカルマージンから決定
    local_nd = round(min({-element[find_critical(element)].margin_L, element[find_critical(element)].margin_H}) / 2) / 100;
    
    for (int m = 0; m < MONTE_CARLO; m++){
        //グローバルな乱数を設定
        for (size_t r = 0; r < global_rand.size(); r++){
            global_rand[r] = rand_global_yield(local_nd);
        }
        //double global = rand_global_yield(local_nd);  //グローバルな乱数計算
        for (int i = 0; i < MULTI_NUM; i++) { // MULTI_NUM = 1回のモンテカルロで生成するマルチプロセス数
            opt_ele_yield(element,data_cir,jud, opt, global, local_nd, global_rand);
        }
        //子プロセスの終了を待機
        now = time(NULL);

        double progress = static_cast<double>(m + 1) / static_cast<double>(MONTE_CARLO) * 100;
        if(static_cast<int>(progress) - count >= 2){
            sharp += "O";
            count = static_cast<int>(progress);
        }
        yield_his[m % yield_his.size()] = opt->success;

        //cout << "\x1B[1K";
        cout << " Optimizing...   "   <<  right << setw(5) <<static_cast<int>(static_cast<double>(now - start)) << " seconds left"
                                                    << "  (Yield: " << (double)opt->success / MULTI_NUM * 100 << "%" << ", Average yield: "<<  reduce(begin(yield_his), end(yield_his)) / yield_his.size() << "% , Normal Distribution: " << local_nd * 100 << "%)" << endl;
        cout << "\x1B[1B";    //カーソルを１行下に移動させる
        cout << "\x1B[1A";    //カーソルを１行上に移動させる
        //cout << "\r";



        /*
        for (const ele_unit ele : element){
            cout << ele.element << " : " << ele.value << endl;
        }

       for(int k = 0; k < element.size(); k++){
            cout << element[k].element << " : " << opt->sum_value[k]  << ", average : "  << opt->sum_value[k] / opt->success << endl;     
       }///////////////////////

*/
        
        fp_yield << m + 1 << ", " << opt->success << ", " << element[1].value << endl;    //歩留まり出力(開発用)

        not_upd++;   //歩留まりが向上しなかったら、not_upd を+1する
        bunbo = MULTI_NUM - opt->success;
        if(opt->success == 100){
            bunbo = 1;
        }

        if( opt->success != 0 ){    // 開発者確認用
            lambda = (MULTI_NUM - opt->success) / MULTI_NUM;
            for(size_t i = 0; i < element.size(); i++){

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

            if( opt->success >= opt->suc_max){
                not_upd = 0;
                opt->suc_max = opt->success;

                for(size_t i = 0; i < element.size(); i++){
                    //element[i].value = opt->sum_value[i] / opt->success;
                    //element[i].value = opt->sum_value[i] / opt->success + lambda * (element[i].value - opt->sum_value_f[i] / bunbo);   // 新たなパラメータに置き換える
                    //opt->best_value[i] =element[i].value;
                }
            }
        }


        if(reduce(begin(yield_his), end(yield_his)) / (int)yield_his.size() > suc_th){  //直近 5 サイクルの歩留まり(success) の平均が 60 を超えたら、標準偏差を +0.01
            local_nd += 0.01;
            Margin_low(element,  jud, data_cir, arg_arr, 2);
            not_upd = 0;
            yield_his.assign(yield_his.size(), 0);

            cri_bias_sum = calc_score(element, power);
            if( cri_bias_sum > opt->cri_bias_best ){   // マージンの評価値 cri_bias_sum が最大だったらそれを現時点の最良の回路として置き換える
                    for(size_t j = 0; j < element.size(); j++){
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





        opt->success = 0;
        for(size_t i = 0; i < element.size(); i++){
            opt->sum_value[i] = 0;
            opt->sum_value_f[i] = 0;
        }


    }






    //cri_bias_sum = min({-element[find_critical_bias(element)].margin_L, element[find_critical_bias(element)].margin_H});
    cri_bias_sum = calc_score(element, power);
    // cri_bias_sum = その回路のマージンの評価 (クリティカルマージン + 2 * バイアスマージン)
    //cout << "cri_bias_sum : " << cri_bias_sum << endl; 
    if( cri_bias_sum > opt->cri_bias_best ){   // マージンの評価値 cri_bias_sum が最大だったらそれを現時点の最良の回路として置き換える
        for(size_t j = 0; j < element.size(); j++){
            opt->best_value[j] = element[j].value;    // best_value配列に現時点の最良の回路のパラメータを格納
        }
        opt->best_value[element.size()] = Margin_num;  //best_value配列の最後の要素の次の要素に何番目の回路かを格納
        opt->cri_bias_best = cri_bias_sum;   //cri_bias_best を更新
    }

    cout <<  " This is the " << GetOrdinalNumber(static_cast<int>(opt->best_value[element.size()])) << " value." << endl;
    for(size_t i = 0; i < element.size(); i++){
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
    fp_yield.close();        

}

