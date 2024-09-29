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



void detail_out(vector<ele_unit> &element){
    stringstream out_det;
    string name;
    for(size_t i = 0; i < element.size(); i++){
        name = element[i].element;
        if(element[i].ide_num == 2 ||element[i].ide_num == 3){
            name.replace(0,1,"J");
        }

        out_det.clear();
        out_det.str("");
        out_det << setw(6) << right << name << " : " << fixed  << right << setw(6) << setprecision(3) << element[i].value  << " " << setw(5) << left <<  element[i].unit << " Lower: "  << setprecision(3) << setw(6) << element[i].value * (1 + element[i].margin_L / 100)  << "   Upper: "
                << setw(6) << setprecision(3)  << element[i].value * (1 + element[i].margin_H / 100) << "   Median: " << setw(6) << setprecision(3)  << element[i].value * (1 + (element[i].margin_L / 100 + element[i].margin_H / 100) / 2 ) << "  "  << setprecision(2) << setw(7) << element[i].margin_L << " % ~ "  << setprecision(2) << setw(6) << element[i].margin_H << " %";
        cout << out_det.str() << endl;
    }

}
