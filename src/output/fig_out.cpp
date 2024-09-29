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


void fig_out(vector<ele_unit> &element){
    int num_cri = find_critical(element);
    int num_bias = find_critical_bias(element);
    vector<string> hush(2);
    vector<double> hush_num{0, 0};
    stringstream out_fig;
    string name;
    cout << "\n" << "                  -50%                                                 50%\n";
    cout << "                    +---------------------------------------------------+\n";
    for(  int i = 0 ; i < element.size() ; i++ ){
        hush_num = {abs(element[i].margin_L), element[i].margin_H};
        hush = {"",""};
        for( int j = 0 ; j < 2 ; j++ ){
            if(hush_num[j] > 50){
                    hush_num[j] = 50; 
            }
            for ( int k = 0 ; k < (int)ceil(hush_num[j] / 2) ; k++){
                hush[j] += MARGIN_FIG;
                }
        }
        
        name = element[i].element;
        if(element[i].ide_num == 2 ||element[i].ide_num == 3){
            name.replace(0,1,"J");
        }
        out_fig.clear();
        out_fig.str("");
        out_fig << setw(6) << element[i].element << "  :  " << fixed  << right << setprecision(2) << setw(6) << -hush_num[0] << " % ["
                << setw(25) << hush[0] << "|" << left << setw(25) << hush[1] << "]" << right << setprecision(2) << setw(6) << hush_num[1] << " %";
        cout << out_fig.str() << endl;
    }
    cout << "                    +---------------------------------------------------+\n";
    cout << "                  -50%                                                 50%\n";
    cout << "    Critical Margin : " << fixed << setprecision(2) << min({-element[num_cri].margin_L, element[num_cri].margin_H}) << " %" << " ('" << element[num_cri].element << "') " << endl;
    cout << "    Bias Margin     : " << fixed << setprecision(2) << min({-element[num_bias].margin_L, element[num_bias].margin_H}) << " %" << " ('" << element[num_bias].element << "') " << endl;
    cout << endl;
}