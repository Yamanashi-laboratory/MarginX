#ifndef JUDGE_H
#define JUDGE_H

#include <vector>

struct judge;

std::vector<std::vector<double>> readJOSIMData();
int judge_operation(std::vector<int> &elej, std::vector<judge> &jud);



#endif