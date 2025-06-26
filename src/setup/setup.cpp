#include "function.hpp"
#include <iostream>
using namespace std;

void setup(vector<string> &arg_arr){
    

    int menu;

    cout << " Please select a setting you want to setup." << endl << endl;
    cout << " 1. setting JoSIM command (default command is \"josim\")" << endl;
    cout << " 2. setting JSIM command (default command is \"jsim\")" << endl;
    cout << "  Selected Mode : ";
    cin >> menu;
    cout << endl;

    switch(menu){
        case 1:
            setup_josim_command(arg_arr);
            break;
        case 2:
            setup_jsim_command(arg_arr);
            break;
        default:
            cerr << " ERROR : Please input a correct number." << endl;
            break;
    }


    exit(0);

}