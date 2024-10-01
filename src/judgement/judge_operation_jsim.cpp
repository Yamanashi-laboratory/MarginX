#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include "function.hpp"
#include <iomanip> 

using namespace std;

/*正常動作の判定条件*/
int judge_operation_jsim(vector<vector<judge>> &jud, int mode){
    int x;
    int bline, eline;
    int ok_flg;
    int anti_flg;
    double judgephase;

    /* Josimの結果を配列に格納 */
    vector<vector<double>> data = readJSIMData();

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

