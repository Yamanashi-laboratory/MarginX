#include "judge.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <thread>
#include <math.h>
#include <cstdlib>
#include <unistd.h>
#include "file.hpp"

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
int judge_operation (vector<int> &elej, vector<judge> &jud){
    int rows = 0;   /* 行の数 */
    int i, j;
    int x, y;
    int jud_sum = 0;
    int jud_sumall = 0;
    int lkey, ukey;
    int bline, eline;
    double judgephase;

    /* Josimの結果を配列に格納 */
    vector<vector<double>> data = readJOSIMData();

    //rowsにCSVファイルを格納した配列の要素数を格納（CSVファイルの行数 - 1）
    rows = data.size();
    //time_scaleに出力データの時間間隔を格納
    double time_scale = data[1][0] - data[0][0];


    /* 正常動作判定*/
    for(x = 1; x <= data[0].size() ; x++){
        jud_sum = 0;          //data[y * cols] == data[y][0]
        lkey = 0;       //data[y * cols] == data[y][x]
        ukey = 0;
        bline = (jud[jud_sumall + jud_sum].btime - data[0][0] )/ time_scale;  //該当範囲の開始行
        eline = (jud[jud_sumall + jud_sum].etime - data[0][0] )/ time_scale;  //該当範囲の終了行
        judgephase = jud[jud_sumall + jud_sum].phase;
        for(y = bline; y < eline; y++){                       //judgementファイルの各行に記載された該当範囲を探索
                if(lkey == 0 && data[y][x] < judgephase){     // x番目の素子について記述通りのスイッチをしているか確認
                    lkey++;
                    ukey = -1;
                }
                else if(lkey == 1 && judgephase < data[y][x]){
                    
                    lkey++;
                }
                else if(ukey == 0 && judgephase < data[y][x] ){    // lkey = 正方向へのスイッチ, ukey = 負方向へのスイッチ
                    ukey++;
                    lkey = -1;
                }
                else if(ukey == 1 && data[y][x] < judgephase){
    
                    ukey++;
                }
                else if(lkey == 2 || ukey == 2){
                    jud_sum++;
                    judgephase = jud[jud_sumall + jud_sum].phase;
                    bline = (jud[jud_sumall + jud_sum].btime - data[0][0] )/ time_scale;
                    eline = (jud[jud_sumall + jud_sum].etime - data[0][0] )/ time_scale;
                    lkey = 0;
                    ukey = 0;
                }
                if(jud_sum == elej[x - 1]){  // x = 1からスタートしている(二次元配列を一次元配列にしているため)関係で、判定している素子が 1 ずれている
                    jud_sumall += jud_sum;  //各判定範囲で全てクリアしたら...
                    break;
                }
        }
    }
    
    /* 条件を全て満たしていれば1を返す */
    if(jud_sumall == jud.size()){   // w と total of judgement の値が一致していたら...
        //free(data); 
        return 1;
    }
    
    /* 条件を満たしてないため0を返す */
    return 0;
}


