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


vector<double> select_score(){

    double CM_power = 0, BM_power = 0, CM_upper = 0, CM_lower = 0;
    int mode = 0;
    vector<double> power;

    cout << " Select the Kind of Score" << endl;
    cout << " 1: Only Critical Margin " << endl;
    cout << " 2: Only Bias Margin" << endl;
    cout << " 3: Only upper critical Margin" << endl;
    cout << " 4: Only lower critical Margin" << endl;
    cout << " 5: Sum of Critical Margin and Bias Margin" << endl;
    cout << " 6: Sum of Critical Margin and Bias Margin * 2 " << endl;
    cout << " 7: Others ( input yourself )" << endl << endl;
    cout << "  Selected Score : ";
    cin >> mode;


    switch(mode){
        case 1:
            CM_power = 1;
            break;
        case 2:
            BM_power = 1;
            break;
        case 3:
            CM_upper = 1;
            break;
        case 4:
            CM_lower = 1;
            break;
        case 5:
            CM_power = 1;
            BM_power = 1;
            break;
        case 6:
            CM_power = 1;
            BM_power = 2;
            break;
        case 7:
            cout << " Select Critical Margin Power" << endl;
            cout << " Critical Margin Power : ";
            cin >> CM_power;
            cout << " Select Bias Margin Power" << endl;
            cout << " Bias Margin Power : ";
            cin >> BM_power;
            cout << " Select Uppper Critical Margin Power" << endl;
            cout << " Critical Margin Power : ";
            cin >> CM_upper;
            cout << " Select Lower Critical Margin Power" << endl;
            cout << " Bias Margin Power : ";
            cin >> CM_lower;
            break;
        default:
            cout << " Please Select a Correct Number" << endl;
            exit(1);
            break;
    }

    power.push_back(CM_power);
    power.push_back(BM_power);
    power.push_back(CM_upper);
    power.push_back(CM_lower);
    return power;
}