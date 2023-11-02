#ifndef MARGIN_H
#define MARGIN_H

#define MARGIN_UPPER 2          /* マージン測定の上限が100％ */
#define MARGIN_FIG "#"

#include <vector>
#include <string>

#define PATH "/programs/matsuoka/MarginX/"


struct judge;
struct ele_cou;
struct board;
struct ele_unit;

/*マルチプロセスを用いてマージンを測定し、結果は board型の構造体に格納*/
void Margin(std::vector<ele_unit> &element, std::vector<int> &elej, std::vector<judge> &jud ,std::vector<std::string>& data_cir, ele_cou *cou, std::vector<std::string>& arg_arr, int menu);

int margin_ele(int n, int ele_num, std::vector<ele_unit> &element,std::vector<int> &elej, std::vector<judge> &jud, board *top, std::vector<std::string> &data_cir, ele_cou *cou);

//クリティカルな素子を探索して、それが格納されているelement内の要素番号返す
int find_critical(std::vector<ele_unit> &element);

//バイアス電圧・電流内のクリティカルな素子を探索して、それが格納されているelement内の要素番号返す
int find_critical_bias(std::vector<ele_unit> &element);

/*Lのマージンを測定(make_cir_Lを含む)*/
int margin_L(int n, ele_unit *p,std::vector<int> &elej, std::vector<judge> &jud, board *top, std::vector<std::string>& data_cir, ele_cou *cou);

/*Kのマージンを測定(make_cir_Kを含む)*/
int margin_K(int n, ele_unit *p,std::vector<int> &elej, std::vector<judge> &jud, board *top, std::vector<std::string>& data_cir, ele_cou *cou);

/*Bのマージンを測定(make_cir_Bを含む)*/
int margin_B(int n, ele_unit *p,std::vector<int> &elej, std::vector<judge> &jud, board *top, std::vector<std::string>& data_cir, ele_cou *cou);

/*BIのマージンを測定(make_cir_BIを含む)*/
int margin_BI(int n, ele_unit *p,std::vector<int> &elej, std::vector<judge> &jud, board *top, std::vector<std::string>& data_cir, ele_cou *cou);

/*PIのマージンを測定(make_cir_PIを含む)*/
int margin_PI(int n, ele_unit *p, std::vector<int> &elej, std::vector<judge> &jud, board *top, std::vector<std::string>& data_cir ,ele_cou *cou);

/*Rのマージンを測定(make_cir_Rを含む)*/
int margin_R(int n, ele_unit *p, std::vector<int> &elej, std::vector<judge> &jud, board *top, std::vector<std::string>& data_cir, ele_cou *cou);

/*Vのマージンを測定(make_cir_Vを含む)*/
int margin_V(int n, ele_unit *p, std::vector<int> &elej, std::vector<judge> &jud, board *top, std::vector<std::string>& data_cir, ele_cou *cou);

/*Iのマージンを測定(make_cir_Iを含む)*/
int margin_I(int n, ele_unit *p, std::vector<int> &elej, std::vector<judge> &jud, board *top, std::vector<std::string>& data_cir, ele_cou *cou);












#endif
