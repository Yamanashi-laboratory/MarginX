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
    double out;
    stringstream outfile;
    string line;
    vector<double> pArray;
    vector<vector<double>> out_array;
    outfile << "CIRCUIT" << getpid() << ".CSV";
        
    /* テキストファイルの読み込み */
    ifstream fp_margin(outfile.str());
    string s;

    if (!fp_margin.is_open()) {
        cerr << "No output of JoSIM."  << endl;
    }

    fp_margin.seekg(0, ios::end);
    if (fp_margin.tellg() == 0) {
        cerr << "ERROR : Output File(.csv) is empty. JoSIM execution may have some errors." << endl;
        fp_margin.close();
        exit(1);
    }
    fp_margin.seekg(0, ios::beg);



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

