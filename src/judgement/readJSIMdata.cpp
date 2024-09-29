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
vector<vector<double>> readJSIMData() {
    int i = 0;
    double out;
    double val;
    string line;
    vector<double> pArray;
    vector<vector<double>> out_array;
    stringstream outfile;
    outfile << "CIRCUIT" << getpid() << ".CSV";  // 拡張子を .CSV から .txt に変更
        
    /* テキストファイルの読み込み */
    ifstream fp_margin(outfile.str());
    
    if (!fp_margin.is_open()) {
        cerr << "No output of JoSIM." << endl;
    }


    /* 配列に格納 */
    while (getline(fp_margin, line)) {  // 行を上から順に選択
        stringstream outline(line);
        int time_flag = 0;
        vector<double> row;
        while (outline >> out) {  // スペースで区切られたデータを取得
            if (time_flag == 0) { 
                out = out * pow(10.0, 12.0);  // ０列目(時間の列)を10^12 倍
                time_flag = 1;
            }
            pArray.emplace_back(out);
            row.emplace_back(out);
        }
        out_array.emplace_back(row);
    }
    fp_margin.close();
    return out_array;
}


