#ifndef JUDGE_H
#define JUDGE_H

#include <vector>
#include <string>

struct judge;

std::vector<std::vector<double>> readJOSIMData();
int judge_operation(std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, int mode);


#endif