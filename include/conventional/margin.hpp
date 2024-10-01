#ifndef MARGIN_H
#define MARGIN_H

#define MARGIN_UPPER 2          /* マージン測定の上限が100％ */
#define MARGIN_FIG "#"

#include <vector>
#include <string>

#define PATH "/Users/matsuoka/MarginX"


struct judge;
struct ele_cou;
struct board;
struct board_bp;
struct ele_unit;

/*マルチプロセスを用いてマージンを測定し、結果は board型の構造体に格納*/
void Margin(std::vector<ele_unit> &element, std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud ,std::vector<std::string>& data_cir, std::vector<std::string>& arg_arr, int menu);

void Margin_low(std::vector<ele_unit> &element, std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud ,std::vector<std::string>& data_cir, std::vector<std::string>& arg_arr, int menu);

int margin_ele(int ele_num, std::vector<ele_unit> &element,std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, board *top, std::vector<std::string> &data_cir);

int margin_ele_low(int ele_num, std::vector<ele_unit> &element,std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, board *top, std::vector<std::string> &data_cir);

//クリティカルな素子を探索して、それが格納されているelement内の要素番号返す
int find_critical(std::vector<ele_unit> &element);

//バイアス電圧・電流内のクリティカルな素子を探索して、それが格納されているelement内の要素番号返す
int find_critical_bias(std::vector<ele_unit> &element);

//void Margin_low_new(std::vector<std::vector<ele_unit>> &elements, std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud ,std::vector<std::string>& data_cir, ele_cou *cou, std::vector<std::string>& arg_arr, int menu);

//nt margin_ele_low_new(int n, int ele_num, std::vector<std::vector<ele_unit>> &elements,std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, board *top, std::vector<std::string> &data_cir, ele_cou *cou);

int get_digit(double num);  //桁数を取得する関数 (□ × 10 ^ ○) の ○ の部分を取得

//void Margin_bp(std::vector<ele_unit> &element, std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud ,std::vector<std::string>& data_cir, std::vector<std::string>& arg_arr, int menu);

//int margin_ele_bp(int ele_num, std::vector<ele_unit> &element,std::vector<std::string> &elej, std::vector<std::vector<judge>> &jud, board_bp *top, std::vector<std::string> &data_cir);
#endif
