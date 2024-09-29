#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip> 
#include <cstdio>
#include <regex>
#include <algorithm>
#include <cmath>
#include <mutex>
#include "function.hpp"

using namespace std;


int make_cir_opt(vector<ele_unit> &element, vector<string> &data_cir){
    stringstream ciroutname;
    ciroutname << "OPTIMIZE" << getpid() << ".cir";
    
    ofstream fpin(ciroutname.str());
    string shunt;
    int y = 0;
    double Rs = 1;
    double resistor = 1;
    string shunt_order;
    vector<string> data_cir_copy = data_cir;
    string line;
    stringstream liness;
    if(!fpin.is_open()){
        cerr << "file not error3!"  << endl;
        return 0;
    }        
    for (int x = 0; x < element.size(); x++) {
        liness.clear();
        liness.str("");
        y = element[x].line_num;
        transform(element[x].element.begin(), element[x].element.end(), element[x].element.begin(), ::tolower); 
        switch(element[x].ide_num){ // マージンを測定している素子なら新しいデータに書き換える
                case 0:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << setw(20) <<  setprecision(3) << element[x].value << element[x].unit << "  fcheck";
                    data_cir_copy[y] = liness.str();
                    break;
                case 1:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << setw(20) <<  setprecision(3) << element[x].value;                     
                    data_cir_copy[y] = liness.str();
                    break;
                case 2:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << "  " << element[x].jjmod<< " area=" << left << setw(10) <<  setprecision(3) << element[x].value;
                    data_cir_copy[y] = liness.str();
                    if(element[x].shunt_det == 0){  // shunt_det == 0 のときは、IcRsの値でshunt抵抗の値を決定
                        Rs = element[x].shunt_cal / element[x].value;
                        shunt_order = "*SHUNT=";
                    }
                    else if(element[x].shunt_det == -1){ // shunt_det == -1 のときは、unshuntとしてシャント抵抗は記述しない
                        break;
                    }
                    else{                           //それ以外( shunt_det == 1)のときは、Bcの値でshunt抵抗の値を決定          
                        resistor = sqrt(element[x].shunt_cal * 1.055 / (2 * 1.602 * element[x].value / 10 * 0.218 * element[x].value) );
                        Rs = resistor * 200 / (200 - resistor);
                        shunt_order = "*Bc=";
                    }
                    shunt = element[x].element;
                    shunt.replace(0,1,"RS");
                    liness.clear();
                    liness.str("");
                    liness <<  shunt << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << setw(20) <<  setprecision(3) << Rs  << "ohm" << "  " << shunt_order << element[x].shunt_cal;
                    data_cir_copy[y + 1] = liness.str();
                    break;
                case 3:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << "  " << element[x].jjmod << " area=" << left << setw(10) <<  setprecision(3) << element[x].value;
                    data_cir_copy[y] = liness.str();
                    break; 
                case 4:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << setw(20) <<  setprecision(3) << element[x].value << element[x].unit;
                    data_cir_copy[y] = liness.str();
                    break;
                case 5:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << setw(20) <<  setprecision(3) << element[x].value << element[x].unit;
                    data_cir_copy[y] = liness.str();
                    break;
                case 6:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(20) <<  setprecision(3)<< element[x].value << element[x].unit <<  ")";
                    data_cir_copy[y] = liness.str();
                    break;
                case 7:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(20) <<  setprecision(3)<< element[x].value << element[x].unit << ")";
                    data_cir_copy[y] = liness.str();
                    break;
        }
        transform(element[x].element.begin(), element[x].element.end(), element[x].element.begin(), ::toupper); 
    }
    for (int x = 0; x < data_cir_copy.size(); x++) {     //内容を書き換えたdata_cir_copy(バッファ)を全て書く
        if(data_cir_copy[x].find(".file") != string::npos || data_cir_copy[x].find(".FILE") != string::npos || data_cir_copy[x].find(".File") != string::npos){
            fpin << ".FILE CIRCUIT" <<  getpid() << ".CSV" << endl;
        }
        else{
            fpin << data_cir_copy[x] << endl;  
        } 

    }
    fpin.close();
    return 0;
}