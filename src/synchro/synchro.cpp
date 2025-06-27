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


void synchro(vector<ele_unit> &element, int ele_num, double syn_value){
    if(element[ele_num].SYN != 0){
        for(size_t i = 0; i < element.size(); i++){
            if(element[i].SYN == element[ele_num].SYN){
                element[i].value = syn_value;
            }
        }
    }

}