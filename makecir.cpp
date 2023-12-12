#include "makecir.hpp"
#include <stdio.h>
#include <unistd.h>
//#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#include <fstream>
#include <iomanip> 
#include <cstdio>
#include <regex>
#include <algorithm>
#include "file.hpp"

using namespace std;

int make_cir(double new_value, int ele_num, vector<ele_unit> &element, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "MARGIN" << getpid() << ".cir";
    ofstream fpin(ciroutname.str());
    string shunt;
    int y = 0;
    if(!fpin.is_open()){
        cerr << "file not error3!"  << endl;
        return 0;
    }        
    for (int x = 0; x < data_cir.size(); x++) {
        if(x == cou->line_num_arr[y] ){
            if(y == ele_num ){
            switch(element[y].ide_num){
                case 0:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << new_value << "pH" << endl;
                    break;
                case 1:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << new_value  << endl;                     
                    break;
                case 2:
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << new_value  << endl;
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << cou->IcRs / new_value  << "ohm" << endl;
                    x++;
                    break;
                case 3:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << new_value  << endl;
                    break;
                case 4:
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << new_value  << endl;
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << cou->IcRs / new_value  << "ohm" << endl;
                    x++;
                    break;
                case 5:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << new_value << "ohm" << endl;
                    break;
                case 6:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(20) <<  setprecision(3)<< new_value << "mV)" << endl;
                break;
                case 7:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(20) <<  setprecision(3)<< new_value << "mA)" << endl;
                    break;
            }
            y++;
            }
            else{
            switch(element[y].ide_num){
                case 0:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << "pH" << endl;
                    break;
                case 1:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value  << endl;                     
                    break;
                case 2:
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << cou->IcRs / element[y].value  << "ohm" << " *SHUNT=" << cou->IcRs << endl;
                    x++;
                    break;
                case 3:
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    break;
                case 4:
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << cou->IcRs / element[y].value  << "ohm" << endl;
                    x++;
                    break;
                case 5:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << "ohm" << endl;
                    break;
                case 6:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(20) <<  setprecision(3)<< element[y].value << "mV)" << endl;
                break;
                case 7:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3)<< element[y].value << "mA)" << endl;
                    break;
            }
        
            y++;
            }
        }
        else if(x == cou->fileoutline){
            fpin << ".FILE CIRCUIT" <<  getpid() << ".CSV" << endl;
        }
        else{
            fpin << data_cir[x] << endl;
        }

    }

    fpin.close();
    return 0;
}

int make_cir_opt(vector<ele_unit> &element, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "OPTIMIZE" << getpid() << ".cir";
    
    ofstream fpin(ciroutname.str());
    string shunt;
    int y = 0;
    if(!fpin.is_open()){
        cerr << "file not error3!"  << endl;
        return 0;
    }        
    for (int x = 0; x < data_cir.size(); x++) {
        if(x == cou->line_num_arr[y] ){
            switch(element[y].ide_num){
                case 0:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << "pH" << endl;
                    break;
                case 1:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value  << endl;                     
                    break;
                case 2:
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << cou->IcRs / element[y].value  << "ohm" << " *SHUNT=" << cou->IcRs << endl;
                    x++;
                    break;
                case 3:
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    break;
                case 4:
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << cou->IcRs / element[y].value  << "ohm" << endl;
                    x++;
                    break;
                case 5:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << "ohm" << endl;
                    break;
                case 6:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3)<< element[y].value << "mV)" << endl;
                break;
                case 7:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3)<< element[y].value << "mA)" << endl;
                    break;
            }
        
            y++;
        }
        else if(x == cou->fileoutline){
            fpin << ".FILE CIRCUIT" <<  getpid() << ".CSV" << endl;
        }
        else{
            fpin << data_cir[x] << endl;
        }

    }

    fpin.close();
    return 0;
}

int make_cir_last(vector<ele_unit> &element, vector<string> &data_cir, ele_cou *cou, vector<string> &arg_arr){
    stringstream ciroutname;
    ciroutname << arg_arr[1] << "_out" << ".cir";
    for(int i = 0; i < arg_arr.size(); i++){
        if(arg_arr[i] == "-o"){
            ciroutname.clear();
            ciroutname.str("");
            ciroutname << arg_arr[i + 1] << ".cir";
        }
    }
    cout << " Output File : " << ciroutname.str() << endl << endl;
    ofstream fpin(ciroutname.str());
    string shunt;
    int y = 0;
    if(!fpin.is_open()){
        cerr << "file not error3!"  << endl;
        return 0;
    }        
    for (int x = 0; x < data_cir.size(); x++) {
        if(x == cou->line_num_arr[y] ){
            transform((element[y].element).begin(), (element[y].element).end(), (element[y].element).begin(), ::tolower); 
            switch(element[y].ide_num){
                case 0:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << "pH" << endl;
                    break;
                case 1:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value  << endl;                     
                    break;
                case 2:
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << cou->IcRs / element[y].value  << "ohm" << " *SHUNT=" << cou->IcRs << endl;
                    x++;
                    break;
                case 3:
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    break;
                case 4:
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << cou->IcRs / element[y].value  << "ohm" << endl;
                    x++;
                    break;
                case 5:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << "ohm" << endl;
                    break;
                case 6:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3)<< element[y].value << "mV)" << endl;
                break;
                case 7:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3)<< element[y].value << "mA)" << endl;
                    break;
            }
            transform((element[y].element).begin(), (element[y].element).end(), (element[y].element).begin(), ::toupper); 
            y++;
        }
        else if(x == cou->fileoutline){
            fpin << ".FILE " <<  arg_arr[1] << "_out" << ".CSV" << endl;
        }
        else{
            fpin << data_cir[x] << endl;
        }

    }

    fpin.close();
    return 0;
}




/*
// インダクタンス cirファイル作成 
int make_cir_L(int n, double new_value, ele_unit *p, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "margin_L" << getpid() << ".cir";
    ofstream fpin_L(ciroutname.str());
    if(!fpin_L.is_open()){
        cerr << "Please select judgement file."  << endl;
        return 0;
    }    
    for (int x = 0; x < cou->line; x++) {
        if(x == p->line_num ){
        fpin_L <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(10) <<  setprecision(3) << new_value << "pH" << endl;
        } 
        else if(x == cou->fileoutline){
        fpin_L << ".FILE MARGIN" <<  getpid() << ".CSV" << endl;
        }
        else{
        fpin_L << data_cir[x] << endl;
        }
    }
    fpin_L.close();
    return 0;
}

// 結合係数 cirファイル作成 
int make_cir_K(int n, double new_value, ele_unit *p, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "margin_K" << getpid() << ".cir";
    ofstream fpin_K(ciroutname.str());
    if(!fpin_K.is_open()){
        cerr << "Please select judgement file."  << endl;
        return 0;
    }    

    for (int x = 0; x < cou->line ; x++) {
        if(x == p->line_num ){
        fpin_K <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(10) <<  setprecision(3) << new_value  << endl;
        }
        else if(x == cou->fileoutline){
        fpin_K << ".FILE MARGIN" <<  getpid() << ".CSV" << endl;
        }
        else{
        fpin_K << data_cir[x] << endl;
        }
    }
    fpin_K.close();
    return 0;
}

// ジョセフソン接合 cirファイル作成 
int make_cir_B(int n, double new_value, ele_unit *p, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    string shunt;
    ciroutname << "margin_B" << getpid() << ".cir";
    ofstream fpin_B(ciroutname.str());
    if(!fpin_B.is_open()){
        cerr << "Please select judgement file."  << endl;
        return 0;
    }    
    for (int x = 0; x < cou->line; x++) {
        if(x == p->line_num ){
        fpin_B <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << new_value  << endl;
        shunt = element[y].element;
        shunt.replace(0,1,"RS");
        fpin_B <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(10) <<  setprecision(3) << cou->IcRs / new_value  << "ohm" << endl;
        x++;
        }
        else if(x == cou->fileoutline){
        fpin_B << ".FILE MARGIN" <<  getpid() << ".CSV" << endl;
        }
        else{
        fpin_B << data_cir[x] << endl;
        }
    }
    fpin_B.close();
    return 0;
}

// JJインダクタンス cirファイル作成 
int make_cir_BI(int n, double new_value, ele_unit *p, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "margin_BI" << getpid() << ".cir";
    ofstream fpin_BI(ciroutname.str());
    if(!fpin_BI.is_open()){
        cerr << "Please select judgement file."  << endl;
        return 0;
    }    
    for (int x = 0; x < cou->line; x++) {
        if(x == p->line_num ){
        fpin_BI <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << new_value  << endl;
        }
        else if(x == cou->fileoutline){
        fpin_BI << ".FILE MARGIN" <<  getpid() << ".CSV" << endl;
        }
        else{
        fpin_BI << data_cir[x] << endl;
        }
    }
    fpin_BI.close();
    return 0;
}

// π接合cirファイル作成 
int make_cir_PI(int n, double new_value, ele_unit *p, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    string shunt;
    ciroutname << "margin_PI" << getpid() << ".cir";
    ofstream fpin_PI(ciroutname.str());
    if(!fpin_PI.is_open()){
        cerr << "Please select judgement file."  << endl;
        return 0;
    }    
    for (int x = 0; x < cou->line; x++) {
        if(x == p->line_num ){
        fpin_PI <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << new_value  << endl;
        shunt = element[y].element;
        shunt.replace(0,1,"RS");
        fpin_PI <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(10) <<  setprecision(3) << cou->IcRs / new_value  << "ohm" << endl;
        x++;
        }
        else if(x == cou->fileoutline){
        fpin_PI << ".FILE MARGIN" <<  getpid() << ".CSV" << endl;
        }
        else{
        fpin_PI << data_cir[x] << endl;
        }
    }
    fpin_PI.close();
    return 0;
}

// 抵抗 cirファイル作成 
int make_cir_R(int n, double new_value, ele_unit *p, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "margin_R" << getpid() << ".cir";
    ofstream fpin_R(ciroutname.str());
    if(!fpin_R.is_open()){
        cerr << "Please select judgement file."  << endl;
        return 0;
    }    
    for (int x = 0; x < cou->line; x++) {
        if(x == p->line_num ){
        fpin_R <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(10) <<  setprecision(3) << new_value << "ohm" << endl;
        }
        else if(x == cou->fileoutline){
        fpin_R << ".FILE MARGIN" <<  getpid() << ".CSV" << endl;
        }
        else{
        fpin_R << data_cir[x] << endl;
        }
    }
    fpin_R.close();
    return 0;
}

// バイアス電圧 cirファイル作成 
int make_cir_V(int n, double new_value, ele_unit *p, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "margin_V" << getpid() << ".cir";
    ofstream fpin_V(ciroutname.str());
    if(!fpin_V.is_open()){
        cerr << "Please select judgement file."  << endl;
        return 0;
    }        
    for (int x = 0; x < cou->line; x++) {
        if(x == p->line_num ){
        fpin_V <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                 << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(10) <<  setprecision(3)<< new_value << "mV)" << endl;
        }
        else if(x == cou->fileoutline){
        fpin_V << ".FILE MARGIN" <<  getpid() << ".CSV" << endl;
        }
        else{
        fpin_V << data_cir[x] << endl;
        }
    }
    fpin_V.close();
    return 0;
}

// バイアス電流 cirファイル作成 
int make_cir_I(int n, double new_value, ele_unit *p, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "margin_I" << getpid() << ".cir";
    ofstream fpin_I(ciroutname.str());
    if(!fpin_I.is_open()){
        cerr << "Please select judgement file."  << endl;
        return 0;
    }        
    for (int x = 0; x < cou->line; x++) {
        if(x == p->line_num ){
        fpin_I <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                 << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(10) <<  setprecision(3)<< new_value << "mA)" << endl;
        }
        else if(x == cou->fileoutline){
        fpin_I << ".FILE MARGIN" <<  getpid() << ".CSV" << endl;
        }
        else{
        fpin_I << data_cir[x] << endl;
        }
    }
    fpin_I.close();
    return 0;
}
*/
