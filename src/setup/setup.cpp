#include "function.hpp"
#include <iostream>
using namespace std;

void setup(vector<string> &arg_arr){
    

    int menu;

    cout << " Please select a setting you want to setup." << endl << endl;
    cout << " 1. setting for using function of outputting margin figure (only to execute)" << endl;
    cout << " 2. setting JoSIM command (default command is \"josim\")" << endl;
    cout << "  Selected Mode : ";
    cin >> menu;
    cout << endl;

    switch(menu){
        case 1:
            setup_python_path(arg_arr);
            break;
        case 2:
            setup_josim_command();
            break;
        default:
            cerr << " ERROR : Please input a correct number." << endl;
            break;
    }


    exit(0);

}