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

void convert_jsim(vector<string> &data_cir, string &filename){
    

    int line_num = 0;
    vector<string> data_cir_copy = data_cir;

    string filename_jsim = filename;
    filename_jsim.insert(filename_jsim.rfind(".") ,"_jsim");


    ifstream file(filename);
    string line, ignore, ele_name, subcir;
    stringstream liness;

    if (!file.is_open()) {
        cerr << "Error opening file2: " << filename << endl;
    }
    while( getline(file, line)) {
            liness.clear();
            liness.str("");

            if(line.find(".print") != string::npos || line.find(".PRINT") != string::npos){
                if(line.rfind(".") != string::npos && line.rfind(".") != 0){
                    line.insert(line.rfind(".") ,"  ");
                    line.insert(line.rfind(".") + 1 ,"  ");
                    liness << line;
                    liness >> ignore >> ignore >> ele_name >> ignore >> subcir;
                    data_cir_copy[line_num] = ".print phase " + subcir + "_" + ele_name;
                }
            }   
        line_num++;
    }
    file.close();

    ofstream fpin(filename_jsim);

        for (int x = 0; x < data_cir_copy.size(); x++) {     //内容を書き換えたdata_cir_copy(バッファ)を全て書く
            fpin << data_cir_copy[x] << endl;  
            data_cir[x] = data_cir_copy[x];
        }
    fpin.close();

    cout << " Converted your netlist into \"" << filename_jsim << "\"" << endl;
    filename = filename_jsim;

    }




