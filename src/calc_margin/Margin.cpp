#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
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
#include <mutex>
#include "function.hpp"
#include <iomanip> 

using namespace std;

void Margin(vector<ele_unit> &element, vector<vector<judge>> &jud, vector<string> &data_cir, vector<string> &arg_arr, int menu){
    if(menu == 0 || menu == 2){
        cout << " Calculating Margins...                                                                                                             " << endl << endl;
    }

    if(menu == 1){
        cout << " Checking Critical Margin                                                                                                             " << endl << endl;
    }
    int sum = element.size();
    int shmid;
    board *top = nullptr;
    vector<int> pid;
    stringstream out_det, out_fig, out_csv;

    /*共有メモリを確保し、IDをshmidに格納*/    //共有メモリはプロセス間で通信を行う（board構造体を用いた結果の挿入）ため必要である
    if ((shmid = shmget(IPC_PRIVATE, sizeof(board), 0666)) == -1) {
        cout << "can't make shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    
    /*結果を格納する top に共有メモリをアタッチ*/
    top = (board *)shmat(shmid, NULL, 0);
    //top->cri_m = 100;

    for(size_t i = 0; i < element.size(); i++){
            board  *shmaddr;
                //マルチプロセスL
                    pid.emplace_back(fork());
                    if(pid[i] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                        margin_ele(i,element,  jud, shmaddr, data_cir);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        exit(0);
                    }
                    else if(pid[i] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
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
    for(size_t i = 0; i < element.size(); i++){
        element[i].margin_L = top->margin[i][0];
        element[i].margin_H = top->margin[i][1];
    }

    switch(menu){
        case 0:    //
            fig_out(element);   
            file_out(arg_arr[1], element);  
            break;
        case 1:    // CCM の最初でCMをチェックする用
            goto free; 
            break;
        case 2:    // 最適化の途中でマージンをチェックする用
            fig_out(element);
            for (string cmd : arg_arr) {
                if (cmd == "-d") {    // -d があった場合、マージンの詳細情報を表示
                    detail_out(element);      
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
            detail_out(element);      
        }
        if (cmd == "-f"){     // -f があった場合、 matplotlib を用いたグラフを出力
            cout << " Please wait for outputting the graph..." << endl;
            margin_py();
            /*
            string path = PATH;              //23行目の #define PATH "展開したフォルダの絶対パス" を参照してgnuplot.pyまでの絶対パス文字列を作成
            string margin = "python " + path + "margin.py";     

            int result_py = system(margin.c_str());
            if (result_py != 0) {
                cerr << "ERROR : executing Python script." << endl;
            }*/
        }
    }

    free:
    /*確保していた共有メモリを解放*/
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        cout << "can't delete shared memory";
        exit(EXIT_FAILURE);
    }

}
