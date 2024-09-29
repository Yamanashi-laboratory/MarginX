#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <chrono> 
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <mutex>
#include "function.hpp"
#include <iomanip> 

using namespace std;

int margin_ele_low(int ele_num, vector<ele_unit> &element, vector<vector<judge>> &jud, board *top, vector<string> &data_cir){
    //int check = 0;
    int outline_num = ele_num;
    int index = 0;
    int index_ite = 10; 
    double value_ini,  HIGH, LOW, percent_HIGH, percent_LOW, median;
    //double value_h, value_l;
    string unit;
    stringstream commandname, delete_cir, delete_out, out_det, out_fig, out_csv;
    vector<ele_unit> copy = element;
    string name = element[ele_num].element;

    //elementの中身は初期値で固定→代わりにcopyを生成し、この値を変えてマージン測定を行う
    commandname << JOSIM_COMMAND << " MARGIN" << getpid() << ".cir > /dev/null"; 
    delete_cir << "rm -f MARGIN" << getpid() << ".cir";
    delete_out << "rm -f CIRCUIT" << getpid() << ".CSV";

    switch(element[ele_num].ide_num){
        case 0 :  
            unit = "H  "; 
            break;
        case 1 :  
            unit = "    ";
            break;
        case 2 :  
            unit = "    ";
            break;
        case 3 :  
            unit = "    ";
            break;
        case 4 :  
            unit = "    ";
            break;
        case 5 :  
            unit = "ohm ";
            break;
        case 6 :  
            unit = "V  ";
            break;
        case 7 :  
            unit = "A  ";
            break;
        default :  break;
    }

 /*素子の値が正の場合*/
    //if(element[ele_num].value > 0){   
        index = 0;     
        value_ini = element[ele_num].value;           
         //マージン上限探索(負の場合下限)
        HIGH = element[ele_num].value;
        while(index < index_ite){
            make_cir(HIGH, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 1){
                HIGH += value_ini / pow(2, index + 1);
            }
            else{
                HIGH -= value_ini / pow(2, index + 1);
            }
            index++;
        }
        make_cir(HIGH, ele_num, copy, data_cir);
        if(system((commandname.str()).c_str()) == -1){
            cout << "error:1" << endl;
        }
        if(judge_operation( jud, 0) == 0){
            HIGH -= value_ini / pow(2, index);
        }         

            //cout << endl << name << endl;
            
        //マージン下限探索(正の場合上限)
        index = 0;     
        value_ini = element[ele_num].value;           
        LOW = element[ele_num].value;
        while(index < index_ite){
            make_cir(LOW, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 1){
                LOW -= value_ini / pow(2, index + 1);
            }
            else{
                LOW += value_ini / pow(2, index + 1);
            }
            index++;
        }
        make_cir(LOW, ele_num, copy, data_cir);
        if(system((commandname.str()).c_str()) == -1){
            cout << "error:1" << endl;
        }
        if(judge_operation( jud, 0) == 0){
            LOW += value_ini / pow(2, index);
        }        

    if(value_ini < 0){
        swap(HIGH, LOW);
        percent_HIGH = ( -(HIGH - element[ele_num].value) / element[ele_num].value) * 100;
        percent_LOW = -( -(element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }
    else{
        percent_HIGH = ( (HIGH - element[ele_num].value) / element[ele_num].value) * 100;
        percent_LOW = -( (element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }
    median = (HIGH + LOW)/2;



    if(element[ele_num].ide_num == 2 ||element[ele_num].ide_num == 3){
        name.replace(0,1,"J");
    }


    top->margin[ele_num][0] = percent_LOW;
    top->margin[ele_num][1] = percent_HIGH;

    
    //cout << "  " << top->cri_m << "    " <<  top->line_num_cri <<  endl;
    //cout << "  " << top->bia_m[0] << ", "<< top->bia_m[1] << endl;

    //中間ファイル削除
    system((delete_cir.str()).c_str());
    system((delete_out.str()).c_str());
    
    return 0;


}

