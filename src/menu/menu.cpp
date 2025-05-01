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
    cout << " 0. Remove Intermediary File" << endl;
    cout << " 1. Judge" << endl; 
    cout << " 2. Calculate Margin" << endl;
    cout << " 3. Optimization with Critical Margin Method (CMM)" << endl;
    cout << " 4. Optimization with Monte Carlo Method (MCM)" << endl;
    cout << " 5. Optimization with MCM and CMM" << endl;
    cout << " 6. Endless Optimization with MCM" << endl;
    cout << " 7. New Optimization with MCM (yield)" << endl;
    cout << " 8. Calculate Margin (low quality)" << endl;
    cout << " 9. Calculate Margin (synchro)" << endl;
    cout << " 10. Sequential Optimization with MCM (yield)" << endl;
    cout << " 11. Optimization with MCM (yield) using jsim" << endl;
    cout << " 12. Sequential Optimization with MCM (yield) using jsim" << endl;
    cout << " 13. Search parameters" << endl << endl;
    cout << "  Selected Mode : ";
    cin >> menu;
    cout << endl;

    return menu;
}