#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <thread>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include "function.hpp"
#include <iomanip> 

using namespace std;


/* Josimの結果を配列に格納 */
vector<vector<double>> readJOSIMData() {
    int i = 0;
    double out;
    double val;
    char *tok;
    stringstream outfile;
    string line;
    vector<double> pArray;
    vector<vector<double>> out_array;
    outfile << "CIRCUIT" << getpid() << ".CSV";
        
    /* テキストファイルの読み込み */
    ifstream fp_margin(outfile.str());
    string s;

    if (!fp_margin.is_open()) {
        cerr << "No josim output."  << endl;
    }

    // シーク位置を先頭の次の行に戻す(０行目がラベルであるため)
    getline(fp_margin, line);

    /* 配列に格納 */
    while(getline(fp_margin, line)){            // 行を上から順に選択
        stringstream outline(line);
        int time_flag = 0;
        vector<double> row;
        while(outline >> out){
            if (time_flag == 0){ 
                out = out * pow(10.0, 12.0);  //０列目(時間の列)を10^12 倍
                time_flag = 1;
            }
            pArray.emplace_back(out);
            row.emplace_back(out);
            if(outline.peek() == ','){
                outline.ignore();
            }
        }
        out_array.emplace_back(row);
    }
    //cout << out_array.size() << endl;
    fp_margin.close();
    return out_array;
}





/*正常動作の判定条件*/
int judge_operation (vector<vector<judge>> &jud, int mode){
    int x;
    int bline, eline;
    int ok_flg;
    int anti_flg;
    double judgephase;

    /* Josimの結果を配列に格納 */
    vector<vector<double>> data = readJOSIMData();

    //rowsにCSVファイルを格納した配列の要素数を格納（CSVファイルの行数 - 1）
    //time_scaleに出力データの時間間隔を格納
    double time_scale = data[1][0] - data[0][0];


    /* 正常動作判定*/
    for(x = 1; x < data[0].size()  ; x++){
        for(int num = 0; num < jud[x - 1].size(); num++){  // x番目の素子のnum回目のジャッジ
            bline = (jud[x - 1][num].btime - data[0][0] )/ time_scale;  //該当範囲の開始行
            eline = (jud[x - 1][num].etime - data[0][0] )/ time_scale;  //該当範囲の終了行
            judgephase = jud[x - 1][num].phase * M_PI;

            if(eline > data.size()){
                cerr << " ERROR (TIME) : Please check if the time in Judgement File is more than its simulation time in Circuit File" << endl;
                exit(1);
            }

            ok_flg = 0;  //shokika 
            if(data[bline][x] < judgephase){
                for(int line = bline; line < eline; line++){
                    if(data[line][x] > judgephase){
                        ok_flg = 1;
                        break;
                    }
                }
            }
            else {
                for(int line = bline; line < eline; line++){
                    if(data[line][x] < judgephase){
                        ok_flg = 1;
                        break;
                    }
                }
            }

            if(jud[x - 1][num].anti == 1){  //anti == 1 no tokiha ok_flg wo hanten
                ok_flg = 1 - ok_flg;
            }

            if(ok_flg == 0){
                if(mode == 1){
                        cout << "  ------NOT PASSED------" << endl;
                        cout << " A Violation was detected in : " << " element No. " << x << endl;
                        cout << "                                " << jud[x - 1][num].btime << " " << jud[x - 1][num].etime << " "<<  jud[x - 1][num].phase << endl;
                        return 0;
                }
                else{
                    return 0;
                }
            }

        }   
    }

    if(mode == 1){
        cout << " ------PASS------" << endl;
    }
    return 1;

}

