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

string sub_unit(string& main){
    size_t find_sub1 = 0, find_sub2 = 0;
    int i = 0;
    find_sub1 = main.find("f");
    find_sub2 = main.find("F");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({(int)find_sub1,(int)find_sub2});
        main.insert(i," ");     
        return "f";
    }

    find_sub1 = main.find("p");
    find_sub2 = main.find("P");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({(int)find_sub1,(int)find_sub2});
        main.insert(i," ");     
        return "p";
    }

    find_sub1 = main.find("n");
    find_sub2 = main.find("N");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({(int)find_sub1,(int)find_sub2});
        main.insert(i," ");     
        return "n";
    }

    find_sub1 = main.find("u");
    find_sub2 = main.find("U");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({(int)find_sub1,(int)find_sub2});
        main.insert(i," ");     
        return "u";
    }

    find_sub1 = main.find("m");
    find_sub2 = main.find("M");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({(int)find_sub1,(int)find_sub2});
        main.insert(i," ");     
        return "m";
    }

    find_sub1 = main.find("k");
    find_sub2 = main.find("K");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({(int)find_sub1,(int)find_sub2});
        main.insert(i," ");     
        return "k";
    }

    find_sub1 = main.find("meg");
    find_sub2 = main.find("MEG");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({(int)find_sub1,(int)find_sub2});
        main.insert(i," ");     
        return "meg";
    }

    find_sub1 = main.find("x");
    find_sub2 = main.find("X");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({(int)find_sub1,(int)find_sub2});
        main.insert(i," ");     
        return "x";
    }

    find_sub1 = main.find("g");
    find_sub2 = main.find("G");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({(int)find_sub1,(int)find_sub2});
        main.insert(i," ");     
        return "g";
    }

    find_sub1 = main.find("t");
    find_sub2 = main.find("T");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({(int)find_sub1,(int)find_sub2});
        main.insert(i," ");     
        return "t";
    }

    return "";
}