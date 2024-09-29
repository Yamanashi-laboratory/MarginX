#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fstream>
#include <iomanip> 
#include <string>
#include <sstream>
#include <filesystem>
#include "function.hpp"


using namespace std;


int main(int argc, const char *argv[]) {
    //int shmid;                        // 共有メモリのIDを格納
    vector<vector<judge>> jud;                //judgementファイルの各行(開始時間　終了時間　位相) を格納
    vector<string> elej;                 //各判定素子の名前
    vector<string> data_cir;          //サーキットファイルを格納
    time_t start, end;                //開始時間と終了時間
    struct tm *tps, *tpe;             //開始時間と終了時間
    char date[100];                   //日付・日時を格納する文字列配列
    int sum_target = 0;               // elements の際の総対象素子数
    vector<ele_unit> element;         //測定素子の情報を格納する構造体を格納する配列
    stringstream commandname;
    vector<string> arg_arr(argv, argv+argc);  //コマンドライン引数が格納されている動的配列  コマンドライン引数が格納されている静的配列(argv)の要素を動的配列(arg_arr)に格納


    display_title(arg_arr);   //タイトルを表示
    display_help(arg_arr);    // コマンドラインに "-h" があった場合、ヘルプを表示

    string filename = arg_arr[1];        //サーキットファイル名　
    string judgefilename = arg_arr[1];   //judgementファイル名
int judge = search_command(arg_arr, "-j");
    if(judge != -1){
        if(judge + 1 < arg_arr.size()){
            judgefilename = arg_arr[judge + 1];
        }
    }






    /*開始時間を表示*/
    time(&start);
    tps = localtime(&start);
    strftime(date, sizeof(date), "%Y/%m/%d %H:%M:%S", tps);
    cout << " START : " << date << endl;

    /*judgementファイルの読み取り*/
    cout << " Checking Judgement File..." << endl;
    readJudgementFile( jud, judgefilename);
    cout << " Target File Name   : " << judgefilename  << endl;
    cout << " ------PASS------" << endl << endl;

    /*サーキットファイルの読み取り*/
    cout << " Checking CircuitFile..." << endl;
    make_data_cir(data_cir, filename, element);

    cout << " Sum of Target      : " << element.size()  << endl;
    cout << " Target File Name   : " << filename  << endl;
    cout << " ------PASS------" << endl << endl;



    //モードを選択し、分岐
    int menu_num = menu();
    time(&start);    //時間を取得(コマンドメニュー入力後)
    switch(menu_num){
        case 0:
            delete_file();  //中間ファイルをすべて削除
            break;
        case 1:
            commandname << "josim OPTIMIZE" << getpid() << ".cir > /dev/null"; 
            make_cir_opt(element, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "ERROR : can't execute JoSIM." << endl;
            }
            judge_operation( jud, 1);
            break;
        case 2:
            Margin(element,  jud, data_cir, arg_arr, 0);
            break;
        case 3:
            critical_margin_method(element,  jud, data_cir, arg_arr);
            make_cir_last(element, data_cir, arg_arr);
            break;
        case 4:
            optimize_monte(element,data_cir,jud, arg_arr);
            break;
        case 5:
            optimize(element,data_cir,jud, arg_arr);
            break;
        case 6:
            optimize_monte_ul(element,data_cir,jud, arg_arr);
            break;
        case 7:
            optimize_yield(element,data_cir,jud, arg_arr);
            break;
        case 8:
            Margin_low(element,  jud, data_cir, arg_arr, 0);
            break;
        default: //適切な値が入力されていなければエラーを吐き終了
            cout << " ERROR : Please input a correct number." << endl;
            return 0;
    }

    end_time(start,end);  //終了時間を表示

    delete_file();  //中間ファイルをすべて削除

    return 0;
}
