#ifndef FILE_H
#define FILE_H

#define SMALL_SUM 150           /* L_MAX + K_MAX + B_MAX + BI_MAX + PI_MAX + R_MAX + V_MAX + I_MAX */

#include <string>
#include <vector>

struct ele_unit{
    int ide_num;  // L = 0, K = 1, B = 2, BI = 3, PI = 4, R = 5, V = 6, I = 7
    int line_num;
    std::string element;
    std::string node1;
    std::string node2;
    double value;  
    double margin_L;
    double margin_H;
    double MIN;
    double MAX;
    int FIX;  //1なら値をばらつかせない
    int SYN;
};

struct range{
    double LMIN = 0;
    double LMAX = 1000;
    double KMIN = -1;
    double KMAX = 1;
    double BMIN = 0;
    double BMAX = 3;
    double BIMIN = 0;
    double BIMAX = 3;
    double PIMIN = 0;
    double PIMAX = 3;
    double RMIN = 0;
    double RMAX = 1000;
    double VMIN = -10;
    double VMAX = 10;
    double IMIN = -10;
    double IMAX = 10;
};

struct opt_num{
    int success = 0;
    int suc_max = 0;
    double best_value[SMALL_SUM] = {};
    double cri_bias_best = 0;
    //std::vector<double> sum_value;
    double sum_value[SMALL_SUM] = {};
};

struct gauss{
    double global_L;
    double global_K;
    double global_B;
    double global_BI;
    double global_PI;
    double global_R;
    double global_V;
    double global_I;
};

struct ele_cou{
    double IcRs = 11.3; // nennnotame shokiti
    double IcRs_PI = 11.3; // nennnotame shokiti
    int fileoutline = -1; // 初期値から変わったかの判別用に -1 としている
    int tranline = -1;   // the line number of ".TRAN"
    int count_L = 0;
    int count_K = 0;
    int count_B = 0;
    int count_BI = 0;
    int count_PI = 0;
    int count_R = 0;
    int count_V = 0;
    int count_I = 0;
    int sum_B = 0;
    int sum_BI = 0;
    int sum_PI = 0;
    int sum_R = 0;
    int sum_V = 0;
    int sum_I = 0;
    std::vector<int> line_num_arr;
};

struct judge{
    int btime;
    int etime;
    double phase;
};

struct board{
    char msg[SMALL_SUM][100];
    char fig[SMALL_SUM][100];
    char fig_csv[SMALL_SUM][50];
    double margin[SMALL_SUM][2];
    double cri_m = 100;
    char cri_name[10];
};


int readJudgementFile(std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, std::string &judgefilename);
double triple_digits(double num);
void make_data_cir(std::vector<std::string>& data_cir, std::string &filename, std::vector<ele_unit> &element, ele_cou *cou);
int find_parameter(char *data);
int judge_element(std::string &line);
void detail_out(board *top, int sum);
void fig_out(board *top, int sum, std::vector<ele_unit> &element);
void file_out(board *top, int sum, std::string &filename, std::vector<ele_unit> &element);
void synchro(std::vector<ele_unit> &element, int ele_num, double syn_value);
void synchro_opt(std::vector<ele_unit> &element, int ele_num);

#endif