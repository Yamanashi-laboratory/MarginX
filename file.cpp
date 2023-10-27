#include "file.hpp"
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
#include "margin.hpp"
#define PYTHON_EXE_PATH u8"Path to python"
using namespace std;

/* 判定基準ファイルの読み込み */
int readJudgementFile(vector<int> &elej, vector<judge> &jud, string &judgefilename){
    //FILE *fp = NULL;
    string line, ignore;
    stringstream dvwords;
    judgefilename += ".txt";
    int x = 0;
    int y = 0;
    int num_jud = 0;
    int btime = 0, etime = 0, phase = 0;

    /* ファイルのオープン */
    ifstream file(judgefilename);
    if (!file.is_open()) {
        cerr << "Please select judgement file."  << endl;
        return 0;
    }
    while ( getline(file,line) ) {
        element:
        if( line.find("element") != string::npos ){
            x++;
            num_jud = 0;
            while ( getline(file,line) ) {
                    dvwords.clear();
                    dvwords.str("");
                    btime = 0;
                    etime = 0;
                    phase = 0;
                    dvwords << line;
                    dvwords >> btime >> etime >> phase;
                    if(btime == 0){
                        elej.emplace_back(num_jud);                                    
                        goto element;
                    }
                    jud.push_back({btime, etime, phase * M_PI});
                    num_jud++;
                    y++;
                }
            elej.emplace_back(num_jud);   //file yomiowatta atoni saigono elej wo sounyuu

        }
    }
    cout << " Number of Elements : " << x << endl;
    cout << " Total of Judgement : " << y << endl;
    file.close();
    return 1;
}


double triple_digits(double num){
    num = num * 1000;
    num = lround(num);
    num = num / 1000;
    return num;
}



/* 素子判定 */
int judge_element(string &line){
    string ele,ele_bi;
    ele = line.substr(0,1);
    ele_bi = line.substr(0,2);
    switch(*ele.c_str()){
        case 'l' :  return 1;
        case 'k' :  return 2;
        case 'b' :  
        if(ele_bi == "bi" || ele_bi == "bI"){
                    return 4;  //JJインダクタンス
            }
            else{
                    return 3;  //通常のJJ
            }
        case 'p' :  return 5;
        case 'r' :  return 6;
        case 'v' :  return 7;
        case 'i' :  return 8;
        default  :  return 0;


    }
    return 0;
}

//circuitファイルの読み込み */
void make_data_cir(vector<string> &data_cir, string &filename, vector<ele_unit> &element, ele_cou *cou){
    
    int flg_shunt_B = 0;
    int flg_shunt_P = 0;
    int line_num = 0;
    int i, i1, i2, i3;

    string cirfilename = filename + ".cir";
    string inpfilename = filename + ".inp";

    filename = cirfilename;

    if(!filesystem::is_regular_file(cirfilename)){
        cirfilename = inpfilename;
        filename = inpfilename;
    }

    ifstream file(cirfilename);
    string line, name, node1, node2, ignore;
    double value;
    stringstream liness;

    if (!file.is_open()) {
        cerr << "Error opening file2: " << cirfilename << endl;
    }
    while( getline(file, line)) {
            liness.clear();
            liness.str("");
            data_cir.emplace_back(line); // insert line to data_cir
            if (flg_shunt_B == 1){
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    stringstream liness(line);
                    liness >> ignore >> ignore >> ignore >> ignore >> ignore >> value;
                    cou->IcRs = value;
                    flg_shunt_B = 2;
            }
            else if (flg_shunt_P == 1){
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    liness << line;
                    liness >> ignore >> ignore >> ignore >> ignore >> ignore >> value;
                    cou->IcRs_PI = value;
                    cou->count_PI++;
                    flg_shunt_P = 2;
            }
            if(line.find("*MIN") != string::npos){
                double MIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  MIN;
                (element.back()).MIN = MIN;
            }
            if(line.find("*MAX") != string::npos){
                double MAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  MAX;
                (element.back()).MAX = MAX;
            }
            if(line.find("*FIX") != string::npos){
                (element.back()).FIX = 1;
            }
            if(line.find("*SYN") != string::npos){
                double SYN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  SYN;
                (element.back()).SYN = SYN;
            }
            switch (judge_element(line)) {
                case 1:
                    i1 = line.rfind("p");
                    i2 = line.rfind("P");
                    if ( i1 != string::npos || i2 != string::npos){
                        i = max({i1,i2,i3});
                        line.insert(i," ");     
                    }
                    liness << line;
                    liness >> name >> node1 >> node2 >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({0, line_num, name, node1, node2, triple_digits(value), 0, 0, 0, 1000, 0, 0});
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_L++;
                    break;
                case 2:
                    liness << line;
                    liness >> name >> node1 >> node2 >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({1, line_num, name, node1, node2, triple_digits(value), 0, 0, -1, 1000, 0, 0});
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_K++;
                    break;
                case 3:
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    liness << line; 
                    liness >> name >> node1 >> node2 >> ignore >> ignore >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({2, line_num, name, node1, node2, triple_digits(value), 0, 0, 0, 1000, 0, 0});
                    flg_shunt_B += 1;
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_B++;
                    break;
                case 4:
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    liness << line;
                    liness >> name >> node1 >> node2 >> ignore >> ignore >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({3, line_num, name, node1, node2, triple_digits(value), 0, 0, 0, 1000, 0, 0});
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_BI++;
                    break;
                case 5:
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    liness << line;
                    liness >> name >> node1 >> node2 >> ignore >> ignore >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({4, line_num, name, node1, node2, triple_digits(value), 0, 0, 0, 1000, 0, 0});
                    flg_shunt_P = 1;
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_PI++;
                    break;
                case 6:
                    i1 = line.rfind("ohm");
                    i2 = line.rfind("OHM");
                    if ( i1 != string::npos || i2 != string::npos){
                        i = max({i1,i2});
                        line.insert(i," ");     
                    }
                    liness << line;
                    liness >> name >> node1 >> node2 >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({5, line_num, name, node1, node2, triple_digits(value), 0, 0, 0, 1000, 0, 0});
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_R++;
                    break;
                case 7:
                    i1 = line.rfind("m");
                    i2 = line.rfind("M");
                    if ( i1 != string::npos || i2 != string::npos){
                        i = max({i1,i2});
                        line.insert(i," ");     
                    }
                    liness << line;
                    liness >> name >> node1 >> node2 >> ignore >> ignore  >> ignore >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({6, line_num, name, node1, node2, triple_digits(value), 0, 0, 0, 1000, 0, 0});
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_V++;
                    break;
                case 8:
                    i1 = line.rfind("m");
                    i2 = line.rfind("M");
                    if ( i1 != string::npos || i2 != string::npos){
                        i = max({i1,i2});
                        line.insert(i," ");     
                    }
                    liness << line;
                    liness >> name >> node1 >> node2 >> ignore >> ignore  >> ignore >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({7, line_num, name, node1, node2, triple_digits(value), 0, 0, 0, 1000, 0, 0});
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_I++;
                    break;
                    
                default:
                    break;

            }
        if(line.find(".FILE") != string::npos ){
            cou->fileoutline = line_num;
        }
        line_num++;
    }
    if(element.size() == 0){
        cout << " Error : No target element." << endl;
        return;
    }
    cou->sum_B  = cou->count_L + cou->count_K;
    cou->sum_BI = cou->count_L + cou->count_K + cou->count_B;
    cou->sum_PI = cou->count_L + cou->count_K + cou->count_B + cou->count_BI;
    cou->sum_R  = cou->count_L + cou->count_K + cou->count_B + cou->count_BI + cou->count_PI;
    cou->sum_V  = cou->count_L + cou->count_K + cou->count_B + cou->count_BI + cou->count_PI + cou->count_R;
    cou->sum_I  = cou->count_L + cou->count_K + cou->count_B + cou->count_BI + cou->count_PI + cou->count_R + cou->count_V;
    file.close();
}

void detail_out(board *top, int sum){
    for(  int i = 0 ; i < sum ; i++ ){
        cout << top->msg[i] << endl;
    }
    cout << endl;
}

void fig_out(board *top, int sum, vector<ele_unit> &element){
    int num_cri = find_critical(element);
    int num_bias = find_critical_bias(element);
    cout << "\n" << "                  -50%                                                 50%\n";
    cout << "                    +---------------------------------------------------+\n";
    for(  int i = 0 ; i < sum ; i++ ){
        cout << top->fig[i] << endl;
    }
    cout << "                    +---------------------------------------------------+\n";
    cout << "                  -50%                                                 50%\n";
    cout << "    Critical Margin : " << fixed << setprecision(2) << min({-element[num_cri].margin_L, element[num_cri].margin_H}) << " %" << " ('" << element[num_cri].element << "') " << endl;
    cout << "    Bias Margin     : " << fixed << setprecision(2) << min({-element[num_bias].margin_L, element[num_bias].margin_H}) << " %" << " ('" << element[num_bias].element << "') " << endl;
    cout << endl;
}

void file_out(board* top, int sum, string &filename, vector<ele_unit> &element) {
    stringstream txtfilename;
    txtfilename << "result_" << filename << ".txt";
    ofstream fpcsv("result.csv");
    ofstream fpout(txtfilename.str());

    for (int i = 0; i < sum; i++) {
        fpcsv << top->fig_csv[i] << endl;
    }

    for (int i = 0; i < sum; i++) {
        fpout << top->msg[i] << endl;
    }

    fpcsv.close();
    fpout.close();
}

void synchro(vector<ele_unit> &element, int ele_num, double syn_value){
    if(element[ele_num].SYN != 0){
        double value = element[ele_num].value;
        for(int i = 0; i < element.size(); i++){
            if(element[i].SYN == element[ele_num].SYN){
                element[i].value = syn_value;
            }
        }
    }

}

void synchro_opt(vector<ele_unit> &element, int ele_num){
    if(element[ele_num].SYN != 0){
        double value = element[ele_num].value;
        for(int i = 0; i < element.size(); i++){
            if(element[i].SYN == element[ele_num].SYN){
                element[i].value = value;
            }
        }
    }

}
