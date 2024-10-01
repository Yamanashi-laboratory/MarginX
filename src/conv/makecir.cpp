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



/*

int make_cir_thread(double new_value, int ele_num, vector<ele_unit> &element, vector<string> &data_cir){
    stringstream ciroutname;
    std::thread::id threadid = std::this_thread::get_id();
    ciroutname << "MARGIN" << threadid << ".cir";

    std::lock_guard<std::mutex> lock(mtx);
    ofstream fpin(ciroutname.str());
    string shunt;
    int y = 0;
    double Rs = 1;
    double resistor;
    double value = 0;
    string shunt_order;
    vector<string> data_cir_copy = data_cir;
    string line;
    stringstream liness;

    if(!fpin.is_open()){
        cerr << "file not error3!"  << endl;
        return 0;
    }        

                cout << "OK" << endl;                 

    for (int x = 0; x < element.size(); x++) {
        liness.clear();
        liness.str("");
        double value = (x == ele_num) ? new_value : element[x].value;

        y = element[x].line_num;
        switch(element[x].ide_num){ // マージンを測定している素子なら新しいデータに書き換える
                case 0:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << setw(20) <<  setprecision(3) << value << element[x].unit << "H";
                    data_cir_copy[y] = liness.str();
                    break;
                case 1:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << setw(20) <<  setprecision(3) << value;                     
                    data_cir_copy[y] = liness.str();
                    break;
                case 2:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << "  " << element[x].jjmod<< " area=" << left << setw(10) <<  setprecision(3) << value;
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
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << "  " << element[x].jjmod << " area=" << left << setw(10) <<  setprecision(3) << value;
                    data_cir_copy[y] = liness.str();
                    break; 
                case 4:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << setw(20) <<  setprecision(3) << value << element[x].unit << "F";
                    data_cir_copy[y] = liness.str();
                    break;
                case 5:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << setw(20) <<  setprecision(3) << value << element[x].unit << "ohm";
                    data_cir_copy[y] = liness.str();
                    break;
                case 6:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(20) <<  setprecision(3)<< value << element[x].unit <<  "V)";
                    data_cir_copy[y] = liness.str();
                    break;
                case 7:
                    liness <<  element[x].element << fixed  << right << setw(6) << element[x].node1 << setw(6) << element[x].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(20) <<  setprecision(3)<< value << element[x].unit << "A)";
                    data_cir_copy[y] = liness.str();
                    break;
        }
    }
    for (int x = 0; x < data_cir_copy.size(); x++) {     //内容を書き換えたdata_cir_copy(バッファ)を全て書く
        if(data_cir_copy[x].find(".file") != string::npos || data_cir_copy[x].find(".FILE") != string::npos || data_cir_copy[x].find(".File") != string::npos){
            fpin << ".FILE CIRCUIT" <<  threadid << ".CSV" << endl;
        }
        else{
            fpin << data_cir_copy[x] << endl;  
        } 

    }

    fpin.close();
    return 0;
}



/*
int make_cir(double new_value, int ele_num, vector<ele_unit> &element, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "MARGIN" << getpid() << ".cir";
    ofstream fpin(ciroutname.str());
    string shunt;
    int y = 0;
    double Rs = 1;
    double resistor;
    double value = 0;
    string shunt_order;
    if(!fpin.is_open()){
        cerr << "file not error3!"  << endl;
        return 0;
    }        
    
    for (int x = 0; x < data_cir.size(); x++) {
            if(x == cou->line_num_arr[y]){
                value = new_value;
            }
            else{
                value = element[y].value;
            }
        if(x == cou->line_num_arr[y] ){
            if(y == ele_num ){
            switch(element[y].ide_num){ // マージンを測定している素子なら新しいデータに書き換える
                case 0:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << new_value << element[y].unit << "H" << endl;
                    break;
                case 1:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << new_value << endl;                     
                    break;
                case 2:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "  " << element[y].jjmod<< " area=" << left << setw(10) <<  setprecision(3) << new_value  << endl;
                    if(element[y].shunt_det == 0){  // shunt_det == 0 のときは、IcRsの値でshunt抵抗の値を決定
                        Rs = element[y].shunt_cal / element[y].value;
                        shunt_order = "*SHUNT=";
                    }
                    else if(element[y].shunt_det == -1){ // shunt_det == -1 のときは、unshuntとしてシャント抵抗は記述しない
                        break;
                    }
                    else{                           //それ以外( shunt_det == 1)のときは、Bcの値でshunt抵抗の値を決定          
                        resistor = sqrt(element[y].shunt_cal * 1.055 / (2 * 1.602 * element[y].value / 10 * 0.218 * element[y].value) );
                        Rs = resistor * 200 / (200 - resistor);
                        shunt_order = "*Bc=";
                    }
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << Rs  << "ohm" << "  " << shunt_order << element[y].shunt_cal << endl;
                    x++;
                    break;
                case 3:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "  " << element[y].jjmod << " area=" << left << setw(10) <<  setprecision(3) << new_value  << endl;
                    break;
                case 4:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << new_value << element[y].unit << "F" << endl;
                    break;
                case 5:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << new_value << element[y].unit << "ohm" << endl;
                    break;
                case 6:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(20) <<  setprecision(3)<< new_value << element[y].unit <<  "V)" << endl;
                break;
                case 7:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(20) <<  setprecision(3)<< new_value << element[y].unit << "A)" << endl;
                    break;
            }
            y++;
            }
            else{
            switch(element[y].ide_num){ // マージンを測定していない素子なら初期値を書き込む
                case 0:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << element[y].unit << "H" << endl;
                    break;
                case 1:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value  << endl;                     
                    break;
                case 2:
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << element[y].jjmod << " area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    if(element[y].shunt_det == 0){  // shunt_det == 0 のときは、IcRsの値でshunt抵抗の値を決定
                        Rs = element[y].shunt_cal / element[y].value;
                        shunt_order = "*SHUNT=";
                    }
                    else if(element[y].shunt_det == -1){ // shunt_det == -1 のときは、unshuntとしてシャント抵抗は記述しない
                        break;
                    }
                    else{                           //それ以外( shunt_det == 1)のときは、Bcの値でshunt抵抗の値を決定          
                        resistor = sqrt(element[y].shunt_cal * 1.055 / (2 * 1.602 * element[y].value / 10 * 0.218 * element[y].value) );
                        Rs = resistor * 200 / (200 - resistor);
                        shunt_order = "*Bc=";
                    }
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << Rs  << "ohm" << "  " << shunt_order << element[y].shunt_cal << endl;
                    x++;
                    break;
                case 3:
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << element[y].jjmod << " area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    break;
                case 4:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << element[y].unit << "F" << endl;
                    break;
                case 5:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << element[y].unit << "ohm" << endl;
                    break;
                case 6:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "50ps   " << setw(20) <<  setprecision(3)<< element[y].value << element[y].unit << "V)" << endl;
                break;
                case 7:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3)<< element[y].value << element[y].unit << "A)" << endl;
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
*/






























/*
int make_cir_test(vector<ele_unit> &element, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "OPTIMIZE" << getpid() << ".cir";
    
    ofstream fpin(ciroutname.str());
    string shunt;
    int y = 0;
    double Rs = 1;
    double resistor = 1;
    string shunt_order;
    if(!fpin.is_open()){
        cerr << "file not error3!"  << endl;
        return 0;
    }        
    for (int x = 0; x < data_cir.size(); x++) {
        if(x == cou->line_num_arr[y] ){
            transform((element[y].element).begin(), (element[y].element).end(), (element[y].element).begin(), ::tolower); 
            switch(element[y].ide_num){
                case 0:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << element[y].unit << "H" << endl;
                    break;
                case 1:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value  << endl;                     
                    break;
                case 2:
                    if(element[y].shunt_det == 1){
                        resistor = sqrt(element[y].shunt_cal * 1.055 / (2 * 1.602 * element[y].value / 10 * 0.218 * element[y].value) );
                        Rs = resistor * 200 / (200 - resistor);
                        shunt_order = "*Bc=";
                    }
                    else{
                        Rs = element[y].shunt_cal / element[y].value;
                        shunt_order = "*SHUNT=";
                    }
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << element[y].jjmod << " area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << Rs  << "ohm" << "  " << shunt_order << element[y].shunt_cal << endl;
                    x++;
                    break;
                case 3:
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << element[y].jjmod << " area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    break;
                case 4:
                    shunt = element[y].element;
                    shunt.replace(0,1,"RS");
                    fpin <<  element[y].element << fixed  << right << setw(7) << element[y].node1 << setw(6) << element[y].node2 << "  " << "jjmod area=" << left << setw(10) <<  setprecision(3) << element[y].value  << endl;
                    fpin <<  shunt << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << cou->IcRs / element[y].value  << "ohm" << endl;
                    x++;
                    break;
                case 5:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << setw(20) <<  setprecision(3) << element[y].value << element[y].unit << "ohm" << endl;
                    break;
                case 6:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3) << element[y].value << element[y].unit << "V)" << endl;
                break;
                case 7:
                    fpin <<  element[y].element << fixed  << right << setw(6) << element[y].node1 << setw(6) << element[y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3) << element[y].value << element[y].unit << "A)" << endl;
                    break;
            }
            transform((element[y].element).begin(), (element[y].element).end(), (element[y].element).begin(), ::toupper); 
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


int make_cir_test2(vector<vector<ele_unit>> &elements, vector<string> &data_cir, ele_cou *cou){
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

        for(int x = 0; x < elements.size(); x++){
            for(int y = 0; y < elements[x].size(); y++){
            liness.clear();
            liness.str("");
            transform((elements[x][y].element).begin(), (elements[x][y].element).end(), (elements[x][y].element).begin(), ::tolower); 
            switch(elements[x][y].ide_num){
                case 0:
                    liness <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << setw(20) <<  setprecision(3) << elements[x][y].value << elements[x][y].unit << "H" << endl;
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
                case 1:
                    liness <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << setw(20) <<  setprecision(3) << elements[x][y].value  << endl;                     
                    break;
                case 2:
                    if(elements[x][y].shunt_det == 1){
                        resistor = sqrt(elements[x][y].shunt_cal * 1.055 / (2 * 1.602 * elements[x][y].value / 10 * 0.218 * elements[x][y].value) );
                        Rs = resistor * 200 / (200 - resistor);
                        shunt_order = "*Bc=";
                    }
                    else{
                        Rs = elements[x][y].shunt_cal / elements[x][y].value;
                        shunt_order = "*SHUNT=";
                    }
                    shunt = elements[x][y].element;
                    shunt.replace(0,1,"RS");

                    liness <<  elements[x][y].element << fixed  << right << setw(7) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << "  " << elements[x][y].jjmod << " area=" << left << setw(10) <<  setprecision(3) << elements[x][y].value;
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    liness.clear();
                    liness.str("");

                    liness <<  shunt << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << setw(20) <<  setprecision(3) << Rs  << "ohm" << "  " << shunt_order << elements[x][y].shunt_cal;
                    data_cir_copy[elements[x][y].line_num + 1] = liness.str();
                    break;
                case 3:
                    liness <<  elements[x][y].element << fixed  << right << setw(7) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << "  " << elements[x][y].jjmod << " area=" << left << setw(10) <<  setprecision(3) << elements[x][y].value;
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
                case 4:
                    fpin <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << setw(20) <<  setprecision(3) << elements[x][y].value << elements[x][y].unit << "F";
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
                case 5:
                    liness <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << setw(20) <<  setprecision(3) << elements[x][y].value << elements[x][y].unit << "ohm";
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
                case 6:
                    liness <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3) << elements[x][y].value << elements[x][y].unit << "V)";
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
                case 7:
                    liness <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3) << elements[x][y].value << elements[x][y].unit << "A)";
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
            }
            transform((elements[x][y].element).begin(), (elements[x][y].element).end(), (elements[x][y].element).begin(), ::toupper); 
            }
        }

    for (int x = 0; x < data_cir_copy.size(); x++) {     //内容を書き換えたdata_cir_copy(バッファ)を全て書く
        if(x == cou->fileoutline){
            fpin << ".FILE CIRCUIT" <<  getpid() << ".CSV" << endl;
        }
        else{
            fpin << data_cir_copy[x] << endl;
        }

    }

    fpin.close();
    return 0;
}

int make_cir_new(double new_value, int ele_row, int ele_col, vector<vector<ele_unit>> &elements, vector<string> &data_cir, ele_cou *cou){
    stringstream ciroutname;
    ciroutname << "MARGIN" << getpid() << ".cir";
    ofstream fpin(ciroutname.str());
    string shunt;
    int y = 0;
    double Rs = 1;
    double resistor;
    double value = 0;
    string shunt_order;
    vector<string> data_cir_copy = data_cir;
    stringstream liness;
    if(!fpin.is_open()){
        cerr << "file not error3!"  << endl;
        return 0;
    }        

        for(int x = 0; x < elements.size(); x++){
            for(int y = 0; y < elements[x].size(); y++){
            liness.clear();
            liness.str("");
            if(x == ele_row && y == ele_col){
                value = new_value;
            }
            else{
                value = elements[x][y].value;
            }

            transform((elements[x][y].element).begin(), (elements[x][y].element).end(), (elements[x][y].element).begin(), ::tolower); 
            switch(elements[x][y].ide_num){
                case 0:
                    liness <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << setw(20) <<  setprecision(3) << value << elements[x][y].unit << "H" << endl;
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
                case 1:
                    liness <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << setw(20) <<  setprecision(3) << value  << endl;                     
                    break;
                case 2:
                    if(elements[x][y].shunt_det == 1){
                        resistor = sqrt(elements[x][y].shunt_cal * 1.055 / (2 * 1.602 * value / 10 * 0.218 * value) );
                        Rs = resistor * 200 / (200 - resistor);
                        shunt_order = "*Bc=";
                    }
                    else{
                        Rs = elements[x][y].shunt_cal / elements[x][y].value;
                        shunt_order = "*SHUNT=";
                    }
                    shunt = elements[x][y].element;
                    shunt.replace(0,1,"RS");

                    liness <<  elements[x][y].element << fixed  << right << setw(7) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << "  " << elements[x][y].jjmod << " area=" << left << setw(10) <<  setprecision(3) << value;
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    liness.clear();
                    liness.str("");

                    liness <<  shunt << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << setw(20) <<  setprecision(3) << Rs  << "ohm" << "  " << shunt_order << elements[x][y].shunt_cal;
                    data_cir_copy[elements[x][y].line_num + 1] = liness.str();
                    break;
                case 3:
                    liness <<  elements[x][y].element << fixed  << right << setw(7) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << "  " << elements[x][y].jjmod << " area=" << left << setw(10) <<  setprecision(3) << value;
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
                case 4:
                    fpin <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << setw(20) <<  setprecision(3) << value << elements[x][y].unit << "F";
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
                case 5:
                    liness <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << setw(20) <<  setprecision(3) << value << elements[x][y].unit << "ohm";
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
                case 6:
                    liness <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3) << value << elements[x][y].unit << "V)";
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
                case 7:
                    liness <<  elements[x][y].element << fixed  << right << setw(6) << elements[x][y].node1 << setw(6) << elements[x][y].node2 << "   "
                        << "PWL(0ps 0mV " <<setw(10) <<  setprecision(3) << "20ps   " << setw(20) <<  setprecision(3) << value << elements[x][y].unit << "A)";
                    data_cir_copy[elements[x][y].line_num] = liness.str();
                    break;
            }
            transform((elements[x][y].element).begin(), (elements[x][y].element).end(), (elements[x][y].element).begin(), ::toupper); 
            }
        }
        for (int x = 0; x < data_cir_copy.size(); x++) {     //内容を書き換えたdata_cir_copy(バッファ)を全て書く
            if(x == cou->fileoutline){
                fpin << ".FILE CIRCUIT" <<  getpid() << ".CSV" << endl;
            }
            else{
                fpin << data_cir_copy[x] << endl;
            }

        }


    fpin.close();
    return 0;


    
}


*/