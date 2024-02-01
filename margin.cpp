#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <thread>
//#include <vector>
#include <chrono>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "margin.hpp"
#include "makecir.hpp"
#include "judge.hpp"
#include "file.hpp"
#include <iomanip> 


using namespace std;

void Margin(vector<ele_unit> &element, vector<string> &elej, vector<vector<judge>> &jud, vector<string> &data_cir, ele_cou *cou, vector<string> &arg_arr, int menu){
    if(menu == 0 || menu == 2){
        cout << " Calculating Margins...                                                                                                             " << endl << endl;
    }

    if(menu == 1){
        cout << " Checking Critical Margin                                                                                                           " << endl << endl;
    }
    int sum = element.size();
    int shmid;
    board *top;
    vector<int> pid_L, pid_K, pid_B, pid_BI, pid_PI, pid_R, pid_V, pid_I;
    
    /*共有メモリを確保し、IDをshmidに格納*/    //共有メモリはプロセス間で通信を行う（board構造体を用いた結果の挿入）ため必要である
    if ((shmid = shmget(IPC_PRIVATE, sizeof(board), 0666)) == -1) {
        cout << "can't make shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    
    /*結果を格納する top に共有メモリをアタッチ*/
    top = (board *)shmat(shmid, NULL, 0);
    top->cri_m = 100;

    int L_num = 0, K_num = 0, B_num = 0, BI_num = 0, PI_num = 0, R_num = 0, V_num = 0, I_num = 0;
    for(int i = 0; i < sum; i++){
            board  *shmaddr;
            switch(element[i].ide_num){
                //マルチプロセスL
                case 0:
                    pid_L.emplace_back(fork());
                    if(pid_L[L_num] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                        margin_ele(L_num, i,element, elej, jud, shmaddr, data_cir, cou);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        exit(0);
                    }
                    else if(pid_L[L_num] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }
                    L_num++;
                    break;
                //マルチプロセスK
                case 1:
                    pid_K.emplace_back(fork());
                    if(pid_K[K_num] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                            margin_ele(K_num, i,element, elej, jud, shmaddr, data_cir, cou);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        exit(0);
                    }
                    else if(pid_K[K_num] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }                    
                    K_num++;
                    break;
                //マルチプロセスB
                case 2:
                    pid_B.emplace_back(fork());
                    if(pid_B[B_num] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                            margin_ele(B_num, i,element, elej, jud, shmaddr, data_cir, cou);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        exit(0);
                    }
                    else if(pid_B[B_num] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }                    
                    B_num++;
                    break;
                //マルチプロセスBI
                case 3:
                    pid_BI.emplace_back(fork());
                    if(pid_BI[BI_num] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                            margin_ele(BI_num, i,element, elej, jud, shmaddr, data_cir, cou);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        exit(0);
                    }
                    else if(pid_BI[BI_num] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }                    
                    BI_num++;
                    break;
                //マルチプロセスPI
                case 4:
                    pid_PI.emplace_back(fork());
                    if(pid_PI[PI_num] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                            margin_ele(PI_num, i,element, elej, jud, shmaddr, data_cir, cou);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                    exit(0);
                    }
                    else if(pid_PI[PI_num] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }                    
                    PI_num++;
                    break;
                //マルチプロセスR
                case 5:
                    pid_R.emplace_back(fork());
                    if(pid_R[R_num] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                            margin_ele(R_num, i,element, elej, jud, shmaddr, data_cir, cou);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                    exit(0);
                    }
                    else if(pid_R[R_num] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }                    
                    R_num++;
                    break;
                //マルチプロセスV
                case 6:
                    pid_V.emplace_back(fork());
                    if(pid_V[V_num] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                            margin_ele(V_num, i,element, elej, jud, shmaddr, data_cir, cou);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        exit(0);
                    }
                    else if(pid_V[V_num] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }                    
                    V_num++;
                    break;
                //マルチプロセスI
                case 7:
                    pid_I.emplace_back(fork());
                    if(pid_I[I_num] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                            margin_ele(I_num, i,element, elej, jud, shmaddr, data_cir, cou);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        exit(0);
                    }
                    else if(pid_I[I_num] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }                    
                    I_num++;
                    break;
            }

    
    }

    /*親プロセス待機 及び インジケータの実装*/
    cout << " [                                                  ]    0 % ";

    for (int i = 0; i < sum; i++) {
        string str = "";
        for (int j = 0; j < static_cast<int>((i / static_cast<double>(sum) * 100) / 2); j++) {
            str += "#";
        }
        cout << "\r [" << setw(50) << left << str << "]  " << setw(3) << right << static_cast<int>(i / static_cast<double>(sum) * 100) << " % ";
        cout.flush();
        this_thread::sleep_for(chrono::microseconds(50));
        wait(NULL);

    }

    cout << "\r" << " [##################################################]  100 % ";

    //マージンの値をelementのmargin_Lとmargin_Hに格納
    for(int i = 0; i < element.size(); i++){
        element[i].margin_L = top->margin[i][0];
        element[i].margin_H = top->margin[i][1];
    }
    switch(menu){
        case 0:
            fig_out(top, sum, element);   
            file_out(top, sum, arg_arr[1], element);  
            break;
        case 1:
            goto free; 
            break;
        case 2:
            fig_out(top, sum, element);
            for (string cmd : arg_arr) {
                if (cmd == "-d") {    // -d があった場合、マージンの詳細情報を表示
                    detail_out(top, sum);      
                }
            }
            goto free;
            break;
        default:
            break;
    }
    /*
    //マージンの図を表示
    fig_out(top, sum, element);   
    //マージンが格納されたファイル(result.txt, result.csv)を出力
    file_out(top, sum, arg_arr[1], element);  
    */
    //コマンドライン引数を検索し、種類に応じて処理を実行
    for (string cmd : arg_arr) {
        if (cmd == "-d") {    // -d があった場合、マージンの詳細情報を表示
            detail_out(top, sum);      
        }
        if (cmd == "-f"){     // -f があった場合、 matplotlib を用いたグラフを出力
            cout << " Please wait for outputting the graph..." << endl;
            string path = PATH;              //23行目の #define PATH "展開したフォルダの絶対パス" を参照してgnuplot.pyまでの絶対パス文字列を作成
            string margin = "python " + path + "margin.py";     
            int result_py = system(margin.c_str());
            if (result_py != 0) {
                cerr << "Error executing Python script." << endl;
            }
        }
    }

    free:
    /*確保していた共有メモリを解放*/
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        cout << "can't delete shared memory";
        exit(EXIT_FAILURE);
    }

}


/* 全素子のマージン測定 */  
int margin_ele(int n, int ele_num, vector<ele_unit> &element, vector<string> &elej, vector<vector<judge>> &jud, board *top, vector<string> &data_cir, ele_cou *cou){
    int check = 0;
    int order_num = 0;
    int outline_num = 0;
    double value_h, value_l, HIGH, LOW, percent_HIGH, percent_LOW, median;
    string unit;
    stringstream commandname, delete_cir, delete_out, out_det, out_fig, out_csv;
    vector<ele_unit> copy = element;
    commandname << "josim MARGIN" << getpid() << ".cir > /dev/null"; 
    delete_cir << "rm -f MARGIN" << getpid() << ".cir";
    delete_out << "rm -f CIRCUIT" << getpid() << ".CSV";

    switch(element[ele_num].ide_num){
        case 0 :  
            order_num = 0;
            unit = " pH ";
            outline_num = n;
            break;
        case 1 :  
            order_num = -1;
            unit = "    ";
            outline_num = n + cou->count_L;
            break;
        case 2 :  
            order_num = -1;
            unit = "    ";
            outline_num = n + cou->sum_B;
            break;
        case 3 :  
            order_num = -1;
            unit = "    ";
            outline_num = n + cou->sum_BI;
            break;
        case 4 :  
            order_num = -1;
            unit = "    ";
            outline_num = n + cou->sum_PI;
            break;
        case 5 :  
            order_num = 0;
            unit = " ohm";
            outline_num = n + cou->sum_R;
            break;
        case 6 :  
            order_num = 0;
            unit = " mV ";
            outline_num = n + cou->sum_V;
            break;
        case 7 :  
            order_num = -1;
            unit = " mA ";
            outline_num = n + cou->sum_I;
            break;
        default :  break;
    }

 /*素子の値が正の場合*/
    if(element[ele_num].value > 0){                      
        value_h = element[ele_num].value * MARGIN_UPPER;
         //マージン上限探索
        HIGH = element[ele_num].value;
        while(check < 4){
            HIGH = HIGH + pow(10 , order_num - check);
            synchro(copy, ele_num, HIGH);
            make_cir(HIGH, ele_num, copy, data_cir, cou);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            else if(judge_operation(elej, jud, 0) == 0){
                HIGH = HIGH - pow(10 , order_num - check);
                synchro(copy, ele_num, HIGH);
                check += 1;
            }
            else if(HIGH > value_h){
                    HIGH = value_h;
                    check = 4;
                    break;
                }
            
        }
        
        //マージン下限探索
        check = 0;
        copy = element;
        LOW = element[ele_num].value;

        while(check < 4){
            LOW = LOW - pow(10 , order_num - check);
            synchro(copy, ele_num, LOW);
            if(LOW < 0.0001){
                LOW = LOW + pow(10 , order_num - check);
                synchro(copy, ele_num, LOW);
                check += 1;
            }
            make_cir(LOW, ele_num, copy, data_cir, cou);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation(elej, jud, 0) == 0){
                LOW = LOW + pow(10 , order_num - check);
                synchro(copy, ele_num, LOW);
                check += 1;
            }
        }
    percent_HIGH = ( (HIGH - element[ele_num].value) / element[ele_num].value) * 100;
    percent_LOW = -( (element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }
 /*素子の値が負の場合*/
    else{
         //マージン上限探索
        HIGH = element[ele_num].value;
        while(check < 4){
            HIGH = HIGH + pow(10 , order_num - check);
            synchro(element, ele_num, HIGH);
            if(HIGH > 0){
                HIGH = HIGH - pow(10 , order_num - check);
                synchro(copy, ele_num, HIGH);
                check += 1;
            }

            make_cir(HIGH, ele_num, element, data_cir, cou);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation(elej, jud, 0) == 0){
                HIGH = HIGH - pow(10 , order_num - check);
                synchro(copy, ele_num, HIGH);
                check += 1;
            }
        }
        //マージン下限探索
        check = 0;
        copy = element;
        LOW = element[ele_num].value;
        value_l = element[ele_num].value * MARGIN_UPPER;
        while(check < 4){
            LOW = LOW - pow(10 , order_num - check);
            synchro(element, ele_num, LOW);
            make_cir(LOW, ele_num, element, data_cir, cou);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            else if(judge_operation(elej, jud, 0) == 0){
                LOW = LOW + pow(10 , order_num - check);
                synchro(element, ele_num, LOW);
                check += 1;
            }
            else if(LOW < value_l){
                LOW = value_l;
                check = 4;
                
            }

        }

    percent_HIGH = ( -(HIGH - element[ele_num].value) / element[ele_num].value) * 100;
    percent_LOW = -( -(element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }
    median = (HIGH + LOW)/2;

    string name = element[ele_num].element;
    if(element[ele_num].ide_num == 2 ||element[ele_num].ide_num == 3){
        name.replace(0,1,"J");
    }

    out_det << setw(6) << name << " " << fixed  << right << setw(6) << setprecision(3) << element[ele_num].value  << unit << " Lower: "  << setprecision(3) << setw(6) << LOW << "   Upper: "
                << setw(6) << setprecision(3)  << HIGH << "   Median: " << setw(6) << setprecision(3)  << median << "  "  << setprecision(2) << setw(7) << percent_LOW << " % ~ "  << setprecision(2) << setw(6) << percent_HIGH << " %";

    strcpy(top->msg[outline_num], (out_det.str()).c_str());
    //top->msg[outline_num] = out_det.str();
    /*マージン図を作成*/
    vector<string> hush(2);
    vector<double> hush_num{-percent_LOW, percent_HIGH};
    for( int j = 0 ; j < 2 ; j++ ){
        if(hush_num[j] > 50){
                hush_num[j] = 50;
            }
        for ( int k = 0 ; k < (int)ceil(hush_num[j] / 2) ; k++){
            hush[j] += MARGIN_FIG;
            }
        }
    

    out_fig << setw(6) << name << "  :  " << fixed  << right << setprecision(2) << setw(6) << -hush_num[0] << " % ["
                << setw(25) << hush[0] << "|" << left << setw(25) << hush[1] << "]" << right << setprecision(2) << setw(6) << hush_num[1] << " %";
    strcpy(top->fig[outline_num], (out_fig.str()).c_str());

    out_csv << setw(5) << name << "," << setw(6) << setprecision(3) << percent_LOW << "," << setw(6) << setprecision(3) << percent_HIGH;
    strcpy(top->fig_csv[outline_num],  (out_csv.str()).c_str());

    top->margin[ele_num][0] = percent_LOW;
    top->margin[ele_num][1] = percent_HIGH;

    
    //cout << "  " << top->cri_m << "    " <<  top->line_num_cri <<  endl;
    //cout << "  " << top->bia_m[0] << ", "<< top->bia_m[1] << endl;

    //中間ファイル削除
    system((delete_cir.str()).c_str());
    system((delete_out.str()).c_str());
    
    return 0;
}

int find_critical(vector<ele_unit> &element){
    double critical = 100;
    int cri_num;
    for(int j = 0; j < element.size(); j++){
        if(element[j].ide_num != 6 && element[j].ide_num != 7){
            if(-element[j].margin_L < critical){
                critical = -element[j].margin_L;
                cri_num = j;
            }
            if(element[j].margin_H < critical){
                critical = element[j].margin_H;
                cri_num = j;
            }
        }
    }
    return cri_num;
}

int find_critical_bias(vector<ele_unit> &element){
    double critical = 100;
    int cri_num;
    for(int j = 0; j < element.size(); j++){
        if(element[j].ide_num == 6 || element[j].ide_num == 7){
            if(-element[j].margin_L < critical){
                critical = -element[j].margin_L;
                cri_num = j;
            }
            if(element[j].margin_H < critical){
                critical = element[j].margin_H;
                cri_num = j;
            }
        }
    }
    return cri_num;
}
