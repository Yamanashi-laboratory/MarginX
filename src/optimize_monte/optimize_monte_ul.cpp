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

void optimize_monte_ul(vector<ele_unit> &element, vector<string> &data_cir, vector<vector<judge>> &jud, vector<string> &arg_arr){
    int mode = 0;
    int CM_choice, BM_choice;
    double CM_th = 0;
    double BM_th = 0;
    double CM_power = 1;
    double BM_power = 1;
    time_t start, now;
    string sharp = "";

    cout << " Do you Set the MIN of the Critical Margin ?" << endl;
    cout << " 1 : Yes" << endl << " 2 : No" << endl << " Selected : ";
    cin >> CM_choice;
    if(CM_choice == 1){
        cout << endl << " Please input the threshold of Critical Margin : " ;
        cin >> CM_th;
    }
    cout << " Do you Set the MIN of the Bias Margin ?" << endl;
    cout << " 1 : Yes" << endl << " 2 : No" << endl << " Selected : ";
    cin >> BM_choice;
    if(BM_choice == 1){
        cout << endl << " Please input the threshold of Bias Margin : " ;
        cin >> BM_th;
    }
    if(CM_choice != 1 && BM_choice != 1){
        cout << endl <<  " Please set the threshold of Margin!" << endl;
        return;
    }

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
    int m = -1;
    vector<ele_unit> element_ini = element;
    cout <<  endl;
    if ((shmid = shmget(IPC_PRIVATE, sizeof(opt_num), 0666)) == -1) {
        cout << "can't make shared memory" << endl;
        exit(EXIT_FAILURE);
    }
    //結果を格納する top に共有メモリをアタッチ
    opt = (opt_num *)shmat(shmid, NULL, 0);
    //Margin(element,  jud, data_cir, cou, arg_arr, 0);
    //Margin(element,  jud, data_cir, cou, arg_arr, 0);
    while(1){
        m++;
        //途中経過
        if( m % 100 == 0){
            opt->suc_max = 0; 
            cout << " This is the progress midway   ( the " << GetOrdinalNumber(static_cast<int>(m / 100 + 1)) 
                 << " check )                                                                                             " << endl;
            Margin(element,  jud, data_cir, arg_arr, 2);
            if( min({-element[find_critical(element)].margin_L, element[find_critical(element)].margin_H}) > CM_th  &&  min({-element[find_critical_bias(element)].margin_L, element[find_critical_bias(element)].margin_H}) > BM_th ){
                make_cir_last(element, data_cir, arg_arr);            
                cout << " Achieved the goal!!!" << endl;
                break;
            }         
            cri_bias_sum = CM_power * min({-element[find_critical(element)].margin_L, element[find_critical(element)].margin_H}) + BM_power * min({-element[find_critical_bias(element)].margin_L, element[find_critical_bias(element)].margin_H});
            if( cri_bias_sum > opt->cri_bias_best ){
                for(int j = 0; j < element.size(); j++){
                    opt->best_value[j] = element[j].value;
                }
                make_cir_last(element, data_cir, arg_arr);            
                opt->best_value[element.size()] = Margin_num;
                opt->cri_bias_best = cri_bias_sum;
            }
            //cout << "cri_bias_sum : " << cri_bias_sum << endl; 
            Margin_num++;
        }
        double global = rand_global(element);
        vector<int> pid;
        //マルチプロセス開始
        for (int i = 0; i < MULTI_NUM; ++i) { // MULTI_NUM = 1回のモンテカルロで生成するマルチプロセス数
                pid.emplace_back(fork());
                if(pid[i] == 0){       
                    opt_num  *shmaddr;
                    if ((shmaddr = (opt_num*)shmat(shmid, NULL, 0)) == (void *)-1) {
                        cout << "childL can't load memory" << endl;
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

        //cout << "\x1B[1K";
        cout << " Optimizing...   "   << "Monte Carlo Count : " << m + 1 <<  right << setw(10) <<static_cast<int>(static_cast<double>(now - start)) << " seconds passed"
                                                    << "   ( success : " << opt->success << " )" << endl;
        cout << "\x1B[1B";    //カーソルを１行下に移動させる
        cout << "\x1B[1A";    //カーソルを１行上に移動させる
        //cout << "\r";

        if( opt->success != 0 ){    // 開発者確認用
            //cout << "success = " << left << setw(5) << opt->success;
            //for(int s = 0; s < opt->success; s++){
                //cout << "#";
            //}
            //cout << endl;

            //if(opt->success >= opt->suc_max){  //成功数が過去最大以上だったら値を置き換える <- 毎回置き換えた方がいい結果が出ていたので不要(?)
                //opt->suc_max = opt->success;
            //cout << " changed value" << endl;

        for(int i = 0; i < element.size(); i++){
            element[i].value = opt->sum_value[i] / opt->success;   // 新たなパラメータに置き換える
            
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
        /*
        if( opt->success >= opt->suc_max){
            opt->suc_max = opt->success;
            for(int i = 0; i < element.size(); i++){
                opt->best_value[i] =element[i].value;
            }
        }*/
        opt->success = 0;
        for(int i = 0; i < element.size(); i++){
            opt->sum_value[i] = 0;
        }


    }


    /*確保していた共有メモリを解放*/
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        cout << "can't delete shared memory" << endl;
        exit(EXIT_FAILURE);
    }


}
