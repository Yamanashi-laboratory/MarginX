#include "function.hpp"

using namespace std;

void delete_file(){
    system("rm -rf MARGIN*");
    system("rm -rf OPTIMIZE*");
    system("rm -rf CIRCUIT*");
}