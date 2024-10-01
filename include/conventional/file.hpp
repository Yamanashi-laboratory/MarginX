#ifndef FILE_H
#define FILE_H

#define SMALL_SUM 100           /* L_MAX + K_MAX + B_MAX + BI_MAX + PI_MAX + R_MAX + V_MAX + I_MAX */

#include <string>
#include <vector>

struct ele_unit{
    int ide_num;  // L = 0, K = 1, B = 2, BI = 3, C = 4, R = 5, V = 6, I = 7
    int line_num;
    std::string element;
    std::string node1;
    std::string node2;
    double value;  
    std::string unit;
    double margin_L;
    double margin_H;
    double MIN;
    double MAX;
    int FIX;  //1なら値をばらつかせない
    int SYN;
    int shunt_det= -1;   //Bc => 1,  IcRs => 0  unshunt => -1  //shunt_det
    double shunt_cal;   //shunt no keisanyou IcRs and Bc    shunt_cal
    std::string jjmod; //jjmod
};



struct range{
    double LMIN = 0;
    double LMAX = 1000;
    double KMIN = -1;
    double KMAX = 1;
    double BMIN = 0.1;
    double BMAX = 3;
    double BIMIN = 0;
    double BIMAX = 3;
    double CMIN = 0;
    double CMAX = 3;
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
    double sum_value_f[SMALL_SUM] = {};
    //double param[50000][3] = {};
};

struct gauss{
    double global_L = 1;
    double global_K = 1;
    double global_B = 1;
    double global_BI = 1;
    double global_PI = 1;
    double global_R = 1;
    double global_V = 1;
    double global_I = 1;
};


struct judge{
    int btime;
    int etime;
    double phase;
    int anti;
};

struct board{
    double margin[SMALL_SUM][2];
};

struct board_bp {
    std::vector<std::string> msg;
    std::vector<std::string> fig;
    std::vector<std::string> fig_csv;
    std::vector<std::vector<double>> margin;
    double cri_m = 100;
    std::string cri_name;

    board_bp(size_t small_sum) {
        msg.resize(small_sum, "");
        fig.resize(small_sum, "");
        fig_csv.resize(small_sum, "");
        margin.resize(small_sum, std::vector<double>(2));
    }
};

int readJudgementFile(std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, std::string &judgefilename);
double triple_digits(double num);
void make_data_cir(std::vector<std::string>& data_cir, std::string &filename, std::vector<ele_unit> &element);
int find_parameter(char *data);
int judge_element(std::string &line);
void detail_out(std::vector<ele_unit> &element);
void fig_out(std::vector<ele_unit> &element);
void file_out(std::string &filename, std::vector<ele_unit> &element);
void synchro(std::vector<ele_unit> &element, int ele_num, double syn_value);
void synchro_opt(std::vector<ele_unit> &element, int ele_num); 
range find_range(range range, std::string line); //各パラメータの上限・下限をチェック
//void make_data_cir_new(std::vector<std::string>& data_cir, std::string &filename, std::vector<std::vector<ele_unit>> &elements, ele_cou *cou);
std::string sub_unit(std::string &main);


#endif