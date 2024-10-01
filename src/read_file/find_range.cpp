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
#define PYTHON_EXE_PATH u8"Path to python" 
using namespace std;

range find_range(range range, string line){
            string ignore;
            stringstream liness; 
            liness.clear();
            liness.str("");

            if(line.find("*LMIN") == 0){
                double LMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  LMIN;
                range.LMIN = LMIN;
            }
            else if(line.find("*LMAX") == 0){
                double LMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  LMAX;
                range.LMAX = LMAX;
            }
            else if(line.find("*KMIN") == 0){
                double KMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  KMIN;
                range.KMIN = KMIN;
            }
            else if(line.find("*KMAX") == 0){
                double KMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  KMAX;
                range.KMAX = KMAX;
            }            
            else if(line.find("*BMIN") == 0){
                double BMIN = 0.1;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BMIN;
                range.BMIN = BMIN;
            }
            else if(line.find("*BMAX") == 0){
                double BMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BMAX;
                range.BMAX = BMAX;
            }
            else if(line.find("*BIMIN") == 0){
                double BIMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BIMIN;
                range.BIMIN = BIMIN;
            }        
            else if(line.find("*BIMAX") == 0){
                double BIMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BIMAX;
                range.BIMAX = BIMAX;
            }            
            else if(line.find("*CMIN") == 0){
                double CMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  CMIN;
                range.CMIN = CMIN;
            }        
            else if(line.find("*CMAX") == 0){
                double CMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  CMAX;
                range.CMAX = CMAX;
            }            
            else if(line.find("*RMIN") == 0){
                double RMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  RMIN;
                range.RMIN = RMIN;
            }
            else if(line.find("*RMAX") == 0){
                double RMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  RMAX;
                range.RMAX = RMAX;
            }
            else if(line.find("*VMIN") == 0){
                double VMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  VMIN;
                range.VMIN = VMIN;
            }
            else if(line.find("*VMAX") == 0){
                double VMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  VMAX;
                range.VMAX = VMAX;
            }
            else if(line.find("*IMIN") == 0){
                double IMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  IMIN;
                range.IMIN = IMIN;
            }
            else if(line.find("*IMAX") == 0){
                double IMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  IMAX;
                range.IMAX = IMAX;
            }

    return range;
}
