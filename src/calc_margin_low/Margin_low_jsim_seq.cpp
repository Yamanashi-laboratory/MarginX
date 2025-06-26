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

void Margin_low_jsim_seq(vector<ele_unit> &element, vector<vector<judge>> &jud, vector<string> &data_cir, vector<string> &arg_arr, int menu){
    if(menu == 0 || menu == 2){
        cout << " Calculating Margins...                                                                                                             " << endl << endl;
    }

    if(menu == 1){
        cout << " Checking Critical Margin                                                                                                           " << endl << endl;
    }
    int shmid;
    int sum = element.size();
    board *top;
    vector<int> pid;
    


    cout << " [                                                  ]    0 % ";
    for(int i = 0; i < element.size(); i++){
        margin_ele_low_jsim(i,element,  jud, top, data_cir);
        string str = "";
        for (int j = 0; j < static_cast<int>((i / static_cast<double>(sum) * 100) / 2); j++) {
            str += "#";
        }
        cout << "\r [" << setw(50) << left << str << "]  " << setw(3) << right << static_cast<int>(i / static_cast<double>(sum) * 100) << " % ";
        cout.flush();
    }

    /*親プロセス待機 及び インジケータの実装*/

    cout << "\r" << " [##################################################]  100 % ";

    //マージンの値をelementのmargin_Lとmargin_Hに格納
    for(size_t i = 0; i < element.size(); i++){
        element[i].margin_L = top->margin[i][0];
        element[i].margin_H = top->margin[i][1];
    }
    switch(menu){
        case 0:
            fig_out(element);   
            file_out(arg_arr[1], element);  
            break;
        case 1:
            return; 
            break;
        case 2:
            fig_out(element);
            for (string cmd : arg_arr) {
                if (cmd == "-d") {    // -d があった場合、マージンの詳細情報を表示
                    detail_out(element);      
                }
            }
            return;
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
            string path = PATH;              //23行目の #define PATH "展開したフォルダの絶対パス" を参照してgnuplot.pyまでの絶対パス文字列を作成
            string margin = "python " + path + "margin.py";      
            int result_py = system(margin.c_str());
            if (result_py != 0) {
                cerr << "Error executing Python script." << endl;
            }
        }
    }

    /*確保していた共有メモリを解放*/

}

