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

void optimize(vector<ele_unit> &element, vector<string> &data_cir, vector<vector<judge>> &jud, vector<string> &arg_arr){


    int mode = 0;
    double CM_power = 1;
    double BM_power = 1;
    int count = 0;
    time_t start, now;
    string sharp = "";
    cout << " Select the Kind of Score" << endl;
    cout << " 1: Only Critical Margin " << endl;
    cout << " 2: Only Bias Margin" << endl;
    cout << " 3: The Sum of Critical Margin and Bias Margin" << endl;
    cout << " 4: The Sum of Critical Margin and Bias Margin * 2 " << endl;
    cout << " 5: Others (input yourself)" << endl;
    cout << "" << endl;
    cout << " Selected Score : ";
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
    int CMM_num = 1;
    vector<ele_unit> element_ini = element;
    cout << endl;
    if ((shmid = shmget(IPC_PRIVATE, sizeof(opt_num), 0666)) == -1) {
        cout << "can't make shared memory" << endl;
        exit(EXIT_FAILURE);
    }
    /*結果を格納する top に共有メモリをアタッチ*/
    opt = (opt_num *)shmat(shmid, NULL, 0);

    for (int m = 0; m < MONTE_CARLO; m++){
        if( m % (MONTE_CARLO / 5 ) == 0){
            critical_margin_method(element,  jud, data_cir, arg_arr);
            opt->suc_max = 0;
            cri_bias_sum = CM_power * min({-element[find_critical(element)].margin_L, element[find_critical(element)].margin_H}) + BM_power * min({-element[find_critical_bias(element)].margin_L, element[find_critical_bias(element)].margin_H});
            if( cri_bias_sum > opt->cri_bias_best ){
                for(int j = 0; j < element.size(); j++){
                    opt->best_value[j] = element[j].value;
                }
                opt->best_value[element.size()] = CMM_num;
                opt->cri_bias_best = cri_bias_sum;
            }
            //cout << "cri_bias_sum : " << cri_bias_sum << endl; 
            CMM_num++;
        }
        double global = rand_global(element);
        vector<int> pid;
        //マルチプロセス開始
        for (int i = 0; i < MULTI_NUM; ++i) { //MULTI_NUM = 1回のモンテカルロで生成するマルチプロセス数
                pid.emplace_back(fork());
                if(pid[i] == 0){       
                    opt_num  *shmaddr;
                    if ((shmaddr = (opt_num*)shmat(shmid, NULL, 0)) == (void *)-1) {
                        printf("childL can't load memory\n");
                        exit(EXIT_FAILURE);
                    }
                    opt_ele(element,data_cir,jud,shmaddr,m ,i, global);
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
        cout << " Optimizing...   "   << " [" << setw(50) << left <<  sharp << "]" <<setw(6) << right << progress << " % " <<  right << setw(5) <<static_cast<int>(static_cast<double>(now - start) / static_cast<double>(m+ 1) * (MONTE_CARLO - (m + 1))) << " seconds remaining"
                                                    << "   ( success : " << opt->success << " )" << endl;
        cout << "\x1B[1B";    //カーソルを１行下に移動させる
        cout << "\x1B[1A";    //カーソルを１行上に移動させる

        if( opt->success != 0 ){

            //if(opt->success >= opt->suc_max){  //成功数が過去最大以上だったら値を置き換える
                //opt->suc_max = opt->success;
                //cout << " changed value" << endl;
            for(int i = 0; i < element.size(); i++){
                element[i].value = opt->sum_value[i] / opt->success;
                
                if(element[i].value < element[i].MIN){
                        element[i].value = element[i].MIN;
                }
                if(element[i].value > element[i].MAX){
                        element[i].value = element[i].MAX;
                }
                if(element[i].FIX == 1){
                    element[i].value = element_ini[i].value;
                }
            }
            
        }
        opt->success = 0;
        for(int i = 0; i < element.size(); i++){
            opt->sum_value[i] = 0;
        }
        
    }
    critical_margin_method(element,  jud, data_cir, arg_arr);
    cri_bias_sum = CM_power * min({-element[find_critical(element)].margin_L, element[find_critical(element)].margin_H}) + BM_power * min({-element[find_critical_bias(element)].margin_L, element[find_critical_bias(element)].margin_H});
    //cout << "cri_bias_sum : " << cri_bias_sum << endl; 
    if( cri_bias_sum > opt->cri_bias_best ){
        for(int j = 0; j < element.size(); j++){
            opt->best_value[j] = element[j].value;
        }
        opt->best_value[element.size()] = CMM_num; // 配列best_valueの一番最後の要素の次の要素にスコアが最も良いCMMの回数を格納
        opt->cri_bias_best = cri_bias_sum;
    }
    cout <<  " This is the " << GetOrdinalNumber(static_cast<int>(opt->best_value[element.size()])) << " value." << endl;
    for(int i = 0; i < element.size(); i++){
        element[i].value = opt->best_value[i];
    }

    Margin(element,  jud, data_cir, arg_arr, 0);

    make_cir_last(element, data_cir, arg_arr);

    //確保していた共有メモリを解放
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        cout << "can't delete shared memory" << endl;
        exit(EXIT_FAILURE);
    }


}