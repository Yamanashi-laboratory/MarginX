#include <string>
#include <vector>
#include <iostream>
#include "function.hpp"

using namespace std;

int search_command(vector<string> arg_arr, const string command){

    for(int i = 0; i < arg_arr.size(); i++){
        if(arg_arr[i] == command){
            return i;
        }
    }
    return -1;
}