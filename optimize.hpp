#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include <vector>
#include <string>


#define MULTI_NUM 100
#define MONTE_CARLO 500
#define CRITICAL_NUM 10

struct judge;
struct ele_cou;
struct board;
struct ele_unit;
struct opt_num;
struct gauss;


void optimize(std::vector<ele_unit> &element, std::vector<std::string> &data_cir, ele_cou *cou,std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, std::vector<std::string>& arg_arr);
void optimize_monte(std::vector<ele_unit> &element, std::vector<std::string> &data_cir, ele_cou *cou,std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, std::vector<std::string>& arg_arr);
void optimize_monte_ul(std::vector<ele_unit> &element, std::vector<std::string> &data_cir, ele_cou *cou,std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, std::vector<std::string>& arg_arr);
void opt_ele(std::vector<ele_unit> &element, std::vector<std::string> &data_cir, ele_cou *cou,std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud,opt_num *opt, int m, int i, double global);
void opt_ele2(std::vector<ele_unit> &element, std::vector<std::string> &data_cir, ele_cou *cou,std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, opt_num *opt, gauss *gau);
void critical_margin_method(std::vector<ele_unit> &element, std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud ,std::vector<std::string>& data_cir, ele_cou *cou, std::vector<std::string>& arg_arr);
std::string GetOrdinalNumber(int num);
double rand_global(gauss *gau, std::vector<ele_unit> &element);
void optimize_yield(std::vector<ele_unit> &element, std::vector<std::string> &data_cir, ele_cou *cou,std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, std::vector<std::string>& arg_arr);


#endif