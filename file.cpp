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
int readJudgementFile(vector<string> &elej, vector<vector<judge>> &jud, string &judgefilename){
    //FILE *fp = NULL;
    string line, ignore;
    
    vector<judge> elej_unit;

    stringstream dvwords;
    judgefilename += ".txt";
    int x = 0;
    int y = 0;
    int num_jud = 0;
    int btime = 0, etime = 0, anti = 0;
    double phase = 0;

    /* ファイルのオープン */
    ifstream file(judgefilename);
    if (!file.is_open()) {
        cerr << " Please select judgement file."  << endl;
        return 0;
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
                    if(line.find("B") != string::npos || line.find("b") != string::npos ){
                        x++;
                        elej.emplace_back(line);      //elej ni sosi no namae wo tuika
                        jud.push_back(elej_unit);     //vector ni youso wo tuika               
                    }
                }
    
    cout << " Number of Elements : " << x << endl;
    cout << " Total of Judgement : " << num_jud << endl;
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
    int i = 0, i1 = 0, i2 = 0, i3 = 0;
    range range;

    string cirfilename = filename + ".cir";
    string inpfilename = filename + ".inp";

    filename = cirfilename;

    if(!filesystem::is_regular_file(cirfilename)){
        cirfilename = inpfilename;
        filename = inpfilename;
    }

    ifstream file(cirfilename);
    string line, name, node1, node2, ignore, jjmod, shunt;
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
                if(line.find("RS") == 0 || line.find("Rs") == 0){
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    stringstream liness(line);
                    liness >> ignore >> ignore >> ignore >> ignore >> shunt >> value;
                    element[element.size() - 1].other_num2 = value;
                    cou->IcRs = value;
                    if(line.find("*Bc") != string::npos || line.find("*BC") != string::npos){
                        element[element.size() - 1].other_num1 = 1;
                    }
                    flg_shunt_B = 0;
                }
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

            if(line.find("*MIN") == 0){
                double MIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  MIN;
                (element.back()).MIN = MIN;
            }
            else if(line.find("*MAX") == 0){
                double MAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  MAX;
                (element.back()).MAX = MAX;
            }
            else if(line.find("*FIX") == 0){
                (element.back()).FIX = 1;
            }
            else if(line.find("*SYN") == 0){
                double SYN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  SYN;
                (element.back()).SYN = SYN;
            }

            if(line.find("*LMIN") == 0){
                double LMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  LMIN;
                range.LMIN = LMIN;
            }
            else if(line.find("*LMAX") == 0){
                double LMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  LMAX;
                range.LMAX = LMAX;
            }
            else if(line.find("*KMIN") == 0){
                double KMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  KMIN;
                range.KMIN = KMIN;
            }
            else if(line.find("*KMAX") == 0){
                double KMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  KMAX;
                range.KMAX = KMAX;
            }            
            else if(line.find("*BMIN") == 0){
                double BMIN = 0.1;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BMIN;
                range.BMIN = BMIN;
            }
            else if(line.find("*BMAX") == 0){
                double BMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BMAX;
                range.BMAX = BMAX;
            }
            else if(line.find("*BIMIN") == 0){
                double BIMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BIMIN;
                range.BIMIN = BIMIN;
            }        
            else if(line.find("*BIMAX") == 0){
                double BIMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BIMAX;
                range.BIMAX = BIMAX;
            }            
            else if(line.find("*PIMIN") == 0){
                double PIMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  PIMIN;
                range.PIMIN = PIMIN;
            }        
            else if(line.find("*PIMAX") == 0){
                double PIMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  PIMAX;
                range.PIMAX = PIMAX;
            }            
            else if(line.find("*RMIN") == 0){
                double RMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  RMIN;
                range.RMIN = RMIN;
            }
            else if(line.find("*RMAX") == 0){
                double RMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  RMAX;
                range.RMAX = RMAX;
            }
            else if(line.find("*VMIN") == 0){
                double VMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  VMIN;
                range.VMIN = VMIN;
            }
            else if(line.find("*VMAX") == 0){
                double VMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  VMAX;
                range.VMAX = VMAX;
            }
            else if(line.find("*IMIN") == 0){
                double IMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  IMIN;
                range.IMIN = IMIN;
            }
            else if(line.find("*IMAX") == 0){
                double IMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  IMAX;
                range.IMAX = IMAX;
            }


            switch (judge_element(line)) {

                case 1:
                    i1 = line.find("p");
                    i2 = line.find("P");

                    if ( i1 != string::npos || i2 != string::npos){
                        i = max({i1,i2,i3});
                        line.insert(i," ");     
                    }

                    liness << line;
                    liness >> name >> node1 >> node2 >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({0, line_num, name, node1, node2, triple_digits(value), 0, 0, range.LMIN, range.LMAX, 0, 0, 0, 0, ""});
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_L++;
                                                        
                    break;
                case 2:
                    liness << line;
                    liness >> name >> node1 >> node2 >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({1, line_num, name, node1, node2, triple_digits(value), 0, 0, range.KMIN, range.KMAX, 0, 0, 0, 0, ""});
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_K++;
                    break;
                case 3:
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    liness << line;                    //jjmod    area=
                    liness >> name >> node1 >> node2 >> jjmod >> ignore >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({2, line_num, name, node1, node2, triple_digits(value), 0, 0, range.BMIN, range.BMAX, 0, 0, 0, 0, jjmod});
                    flg_shunt_B = 1;
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_B++;
                    break;
                case 4:
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    liness << line;
                    liness >> name >> node1 >> node2 >> jjmod >> ignore >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({3, line_num, name, node1, node2, triple_digits(value), 0, 0, range.BIMIN, range.BIMAX, 0, 0, 0, 0, jjmod});
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
                    element.push_back({4, line_num, name, node1, node2, triple_digits(value), 0, 0, range.PIMIN, range.PIMAX, 0, 0, 0, 0, ""});
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
                    element.push_back({5, line_num, name, node1, node2, triple_digits(value), 0, 0, range.RMIN, range.RMAX, 0, 0, 0, 0, ""});
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
                    element.push_back({6, line_num, name, node1, node2, triple_digits(value), 0, 0, range.IMIN, range.IMAX, 0, 0, 0, 0, ""});
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
                    element.push_back({7, line_num, name, node1, node2, triple_digits(value), 0, 0, range.VMIN, range.VMAX, 0, 0, 0, 0, ""});
                    cou->line_num_arr.emplace_back(line_num);
                    cou->count_I++;
                    break;
                    
                default:
                    break;

            }

        if(line.find(".FILE") == 0 || line.find(".file") == 0 || line.find(".File") == 0){// .if there is ".FILE" line, store the line number.
            cou->fileoutline = line_num;
        }

        else if(line.find(".tran") == 0 || line.find(".TRAN") == 0){// .if there is ".tran" or ".TRAN" line, store the line number and insert an empty line.
            cou->tranline = line_num;
            data_cir.emplace_back(" ");    
            line_num++;
        }

        line_num++;
    }
    if(element.size() == 0){
        cout << " Error : No target element." << endl;
        return;
    }
    if(cou->fileoutline == -1){ // .FILE の行がなかったら、.tranの次の行(tranline + 1) に ".FILE hogehoge.csv" を挿入
        cou->fileoutline = cou->tranline + 1;
        data_cir[cou->tranline + 1] = ".FILE hogehoge.csv";    
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

void yield_out(board* top, int sum, string &filename, vector<ele_unit> &element){

    
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
