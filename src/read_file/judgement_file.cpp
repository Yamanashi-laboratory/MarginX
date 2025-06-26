#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <cstdio>
#include <regex>
#include <algorithm>
#include <filesystem>
#include <cmath>
#include <math.h>
#include <iomanip> 
#include "function.hpp"

using namespace std;

/* 判定基準ファイルの読み込み */
int readJudgementFile(vector<vector<judge>> &jud, string &judgefilename, vector<string> arg_arr){
    string line, ignore;
    vector<judge> elej_unit;
    stringstream dvwords;

    int judge = search_command(arg_arr, "-j");  // コマンドラインに "-j" があれば、その次のコマンドラインの文字列をjudgementfile_name に格納
        if(judge != -1){
            if(judge + 1 < (int)arg_arr.size()){
                judgefilename = arg_arr[judge + 1];
            }
        }

    judgefilename += ".txt";
    int x = 0;
    int num_jud = 0; 
    int btime = 0, etime = 0, anti = 0;
    double phase = 0;

    /* ファイルのオープン */
    ifstream file(judgefilename);
    if (!file.is_open()) {
        cerr << "ERROR : Can't read JudgementFile" << endl;
        exit(1);
    }
            while ( getline(file,line) ) {
                    dvwords.clear();
                    dvwords.str("");
                    btime = 0;
                    etime = 0;
                    phase = 0;
                    anti = 0;
                    dvwords << line;
                    dvwords >> btime >> etime >> phase >> anti;
                    if(etime != 0){
                        jud[x - 1].push_back({btime, etime, phase, anti});
                        num_jud++;
                    }
                    if(line.find("B") != string::npos || line.find("b") != string::npos || line.find("e") != string::npos || line.find("E") != string::npos){
                        x++;
                        jud.push_back(elej_unit);     //vector ni youso wo tuika               
                    }
                }
    
    cout << " Number of Elements : " << x << endl;
    cout << " Total of Judgement : " << num_jud << endl;
    file.close();
    return 1;
}



