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
#define PYTHON_EXE_PATH u8"Path to python" 
using namespace std;


/* 素子判定 */
int judge_element(string &line){
    string ele,ele_bi;
    ele = line.substr(0,1);
    ele_bi = line.substr(0,2);
    switch(*ele.c_str()){
        case 'l' :  return 0;
        case 'k' :  return 1;
        case 'b' :  
        if(ele_bi == "bi" || ele_bi == "bI"){
                    return 3;  //JJインダクタンス
            }
            else{
                    return 2;  //通常のJJ
            }
        case 'c' :  return 4;
        case 'r' :  return 5;
        case 'v' :  return 6;
        case 'i' :  return 7;
        default  :  return -1;


    }
}
