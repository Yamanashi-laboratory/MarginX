#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fstream>
#include <iomanip> 
#include <string>
#include <sstream>
#include <filesystem>
#include "function.hpp"

using namespace std;

int menu(){
    int menu;

    cout << " Please select an operation mode." << endl << endl;

    cout << " 0.  Remove Intermediary File" << endl;
    cout << " 1.  Judge operation" << endl;
    cout << " 2.  Calculate Margin (acurate)" << endl;
    cout << " 8.  Calculate Margin (binary search)" << endl;
    cout << " 9.  Calculate Margin (binary search with synchro)" << endl;
    cout << " 3.  Optimization with Critical Margin Method" << endl;
    //cout << " 4.  Optimization with Monte Carlo Method (MCM)" << endl;
    //cout << " 5.  Optimization with MCM and CMM" << endl;
    //cout << " 6.  Endless Optimization with MCM" << endl;
    cout << " 7.  Optimization with Center of Gravity Method (CGM)" << endl;

    cout << " 10. Sequential Optimization with CGM" << endl;

    cout << " 13. JSIM modes" << endl;

    //cout << " 13. Search parameters" << endl << endl;
    cout << "  Selected: ";

    cin >> menu;
    cout << endl;

    return menu;
}