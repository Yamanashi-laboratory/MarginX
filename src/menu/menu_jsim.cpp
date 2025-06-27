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

int menu_jsim(){
    int menu;
    cout << " This is JSIM mode." << endl;
    cout << " Please select an operation mode." << endl << endl;

    cout << " 1. Judge operation" << endl;
    cout << " 2. Calculate Margin (acurate)" << endl;
    cout << " 3. Calculate Margin (binary search)" << endl;
    cout << " 4. Calculate Margin (binary search with synchro)" << endl;
    cout << " 5. Optimization with Critical Margin Method" << endl;
    cout << " 6. Optimization with Center of Gravity Method (CGM)" << endl;
    cout << " 7. Sequential Optimization with CGM" << endl;
    cout << "  Selected: ";

    cin >> menu;
    cout << endl;

    return menu;
}