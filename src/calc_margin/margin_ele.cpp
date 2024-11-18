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

/* 全素子のマージン測定 */  
int margin_ele(int ele_num, vector<ele_unit> &element, vector<vector<judge>> &jud, board *top, vector<string> &data_cir){
    int check = 0;
    int order_num = get_digit(element[ele_num].value);
    int outline_num = ele_num;
    double value_h, value_l, HIGH, LOW, percent_HIGH, percent_LOW, median;
    string unit;
    stringstream commandname, delete_cir, delete_out, out_det, out_fig, out_csv;
    vector<ele_unit> copy = element;

    //elementの中身は初期値で固定→代わりにcopyを生成し、この値を変えてマージン測定を行う
    commandname << JOSIM_COMMAND << " MARGIN" << getpid() << ".cir > /dev/null"; 
    delete_cir << "rm -f MARGIN" << getpid() << ".cir";
    delete_out << "rm -f CIRCUIT" << getpid() << ".CSV";

 /*素子の値が正の場合*/
    if(element[ele_num].value > 0){                      
        value_h = element[ele_num].value * MARGIN_UPPER;
         //マージン上限探索
        HIGH = element[ele_num].value;
        while(check < 4){
            HIGH = HIGH + pow(10 , order_num - check);
            if(element[ele_num].ide_num == 1 && HIGH > 1){   // 結合係数 k ならば
                HIGH = HIGH - pow(10 , order_num - check);
                ////synchro(copy, ele_num, HIGH);
                check += 1;
            }
            //synchro(copy, ele_num, HIGH);
            make_cir(HIGH, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            else if(judge_operation( jud, 0) == 0){
                HIGH = HIGH - pow(10 , order_num - check);
                ////synchro(copy, ele_num, HIGH);
                check += 1;
            }
            else if(HIGH > value_h){
                    HIGH = value_h;
                    check = 4;
                    break;
                }
            
        }
        
        //マージン下限探索
        check = 0;
        copy = element;
        LOW = element[ele_num].value;

        while(check < 4){
            LOW = LOW - pow(10 , order_num - check);
            ////synchro(copy, ele_num, LOW);
            if(LOW < 0.001){
                LOW = LOW + pow(10 , order_num - check);
                ////synchro(copy, ele_num, LOW);
                check += 1;
            }
            //synchro(copy, ele_num, LOW);
            make_cir(LOW, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 0){
                LOW = LOW + pow(10 , order_num - check);
                ////synchro(copy, ele_num, LOW);
                check += 1;
            }
        }
    percent_HIGH = ( (HIGH - element[ele_num].value) / element[ele_num].value) * 100;
    percent_LOW = -( (element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }
 /*素子の値が負の場合*/
    else{
         //マージン上限探索
        HIGH = element[ele_num].value;
        while(check < 4){
            HIGH = HIGH + pow(10 , order_num - check);
            ////synchro(copy, ele_num, HIGH);
            if(HIGH > 0){
                HIGH = HIGH - pow(10 , order_num - check);
                ////synchro(copy, ele_num, HIGH);
                check += 1;
            }
            //synchro(copy, ele_num, HIGH);
            make_cir(HIGH, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 0){
                HIGH = HIGH - pow(10 , order_num - check);
                ////synchro(copy, ele_num, HIGH);
                check += 1;
            }
        }
        //マージン下限探索
        check = 0;
        copy = element;
        LOW = element[ele_num].value;
        value_l = element[ele_num].value * MARGIN_UPPER;
        while(check < 4){
            LOW = LOW - pow(10 , order_num - check);
            if(element[ele_num].ide_num == 1 && LOW < -1){
                LOW = LOW + pow(10 , order_num - check);
                ////synchro(copy, ele_num, LOW);
                check += 1;
            }
            //synchro(copy, ele_num, LOW);
            make_cir(LOW, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            else if(judge_operation( jud, 0) == 0){
                LOW = LOW + pow(10 , order_num - check);
                ////synchro(copy, ele_num, LOW);
                check += 1;
            }
            else if(LOW < value_l){
                LOW = value_l;
                check = 4;
                
            }

        }

    percent_HIGH = ( -(HIGH - element[ele_num].value) / element[ele_num].value) * 100;
    percent_LOW = -( -(element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }

    string name = element[ele_num].element;
    if(element[ele_num].ide_num == 2 ||element[ele_num].ide_num == 3){
        name.replace(0,1,"J");
    }

    //top->msg[outline_num] = out_det.str();
    /*マージン図を作成*/
    top->margin[ele_num][0] = percent_LOW;
    top->margin[ele_num][1] = percent_HIGH;

    
    //cout << "  " << top->cri_m << "    " <<  top->line_num_cri <<  endl;
    //cout << "  " << top->bia_m[0] << ", "<< top->bia_m[1] << endl;

    //中間ファイル削除
    system((delete_cir.str()).c_str());
    system((delete_out.str()).c_str());
    
    return 0;
}