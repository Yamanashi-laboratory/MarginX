#ifndef MAKECIR_H
#define MAKECIR_H

#include <vector>
#include <string>

struct board;
struct ele_unit;
struct ele_cou;

int make_cir(double new_value, int ele_num, std::vector<ele_unit> &element, std::vector<std::string>& data_cir, ele_cou *cou);
int make_cir_opt(std::vector<ele_unit> &element, std::vector<std::string>& data_cir, ele_cou *cou);
int make_cir_last(std::vector<ele_unit> &element, std::vector<std::string>& data_cir, ele_cou *cou, std::vector<std::string> &arg_arr);

/*
int make_cir_L(int n, double new_value, ele_unit *p, std::vector<std::string>& data_cir, ele_cou *cou);
int make_cir_K(int n, double new_value, ele_unit *p, std::vector<std::string>& data_cir, ele_cou *cou);
int make_cir_B(int n, double new_value, ele_unit *p, std::vector<std::string>& data_cir, ele_cou *cou);
int make_cir_BI(int n, double new_value, ele_unit *p, std::vector<std::string>& data_cir, ele_cou *cou);
int make_cir_PI(int n, double new_value, ele_unit *p, std::vector<std::string>& data_cir, ele_cou *cou);
int make_cir_R(int n, double new_value, ele_unit *p, std::vector<std::string>& data_cir, ele_cou *cou);
int make_cir_V(int n, double new_value, ele_unit *p, std::vector<std::string>& data_cir, ele_cou *cou);
int make_cir_I(int n, double new_value, ele_unit *p, std::vector<std::string>& data_cir, ele_cou *cou);
*/

#endif