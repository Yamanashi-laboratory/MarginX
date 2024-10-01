#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <chrono> 
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <mutex>
#include "function.hpp"
#include <iomanip> 

//#include <boost/interprocess/managed_shared_memory.hpp>
//#include <boost/interprocess/allocators/allocator.hpp>
//#include <boost/interprocess/containers/vector.hpp>

std::mutex mtx;
using namespace std;
//namespace bip = boost::interprocess;

void Margin(vector<ele_unit> &element, vector<vector<judge>> &jud, vector<string> &data_cir, vector<string> &arg_arr, int menu){
    if(menu == 0 || menu == 2){
        cout << " Calculating Margins...                                                                                                             " << endl << endl;
    }

    if(menu == 1){
        cout << " Checking Critical Margin                                                                                                             " << endl << endl;
    }
    int sum = element.size();
    int shmid;
    board *top;
    vector<int> pid;
    stringstream out_det, out_fig, out_csv;

    /*共有メモリを確保し、IDをshmidに格納*/    //共有メモリはプロセス間で通信を行う（board構造体を用いた結果の挿入）ため必要である
    if ((shmid = shmget(IPC_PRIVATE, sizeof(board), 0666)) == -1) {
        cout << "can't make shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    
    /*結果を格納する top に共有メモリをアタッチ*/
    top = (board *)shmat(shmid, NULL, 0);
    //top->cri_m = 100;

    for(int i = 0; i < element.size(); i++){
            board  *shmaddr;
                //マルチプロセスL
                    pid.emplace_back(fork());
                    if(pid[i] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                        margin_ele(i,element,  jud, shmaddr, data_cir);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        exit(0);
                    }
                    else if(pid[i] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }
    }

    /*親プロセス待機 及び インジケータの実装*/
    cout << " [                                                  ]    0 % ";

    for (int i = 0; i < sum; i++) {
        string str = "";
        for (int j = 0; j < static_cast<int>((i / static_cast<double>(sum) * 100) / 2); j++) {
            str += "#";
        }
        cout << "\r [" << setw(50) << left << str << "]  " << setw(3) << right << static_cast<int>(i / static_cast<double>(sum) * 100) << " % ";
        cout.flush();
        this_thread::sleep_for(chrono::microseconds(50));
        wait(NULL);

    }

    cout << "\r" << " [##################################################]  100 % ";

    //マージンの値をelementのmargin_Lとmargin_Hに格納
    for(size_t i = 0; i < element.size(); i++){
        element[i].margin_L = top->margin[i][0];
        element[i].margin_H = top->margin[i][1];
    }

    switch(menu){
        case 0:    //
            fig_out(element);   
            file_out(arg_arr[1], element);  
            break;
        case 1:    // CCM の最初でCMをチェックする用
            goto free; 
            break;
        case 2:    // 最適化の途中でマージンをチェックする用
            fig_out(element);
            for (string cmd : arg_arr) {
                if (cmd == "-d") {    // -d があった場合、マージンの詳細情報を表示
                    detail_out(element);      
                }
            }
            goto free;
            break;
        default:
            break;
    }
    /*
    //マージンの図を表示
    fig_out(top, sum, element);   
    //マージンが格納されたファイル(result.txt, result.csv)を出力
    file_out(top, sum, arg_arr[1], element);  
    */
    //コマンドライン引数を検索し、種類に応じて処理を実行
    for (string cmd : arg_arr) {
        if (cmd == "-d") {    // -d があった場合、マージンの詳細情報を表示
            detail_out(element);      
        }
        if (cmd == "-f"){     // -f があった場合、 matplotlib を用いたグラフを出力
            cout << " Please wait for outputting the graph..." << endl;
            string path = PATH;              //23行目の #define PATH "展開したフォルダの絶対パス" を参照してgnuplot.pyまでの絶対パス文字列を作成
            string margin = "python " + path + "margin.py";     
            int result_py = system(margin.c_str());
            if (result_py != 0) {
                cerr << "Error executing Python script." << endl;
            }
        }
    }

    free:
    /*確保していた共有メモリを解放*/
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        cout << "can't delete shared memory";
        exit(EXIT_FAILURE);
    }

}


/* 全素子のマージン測定 */  
int margin_ele(int ele_num, vector<ele_unit> &element, vector<vector<judge>> &jud, board *top, vector<string> &data_cir){
    int check = 0;
    int order_num = get_digit(element[ele_num].value);
    int outline_num = ele_num;
    double value_h, value_l, HIGH, LOW, percent_HIGH, percent_LOW, median;
    string unit;
    stringstream commandname, delete_cir, delete_out, out_det, out_fig, out_csv;
    vector<ele_unit> copy = element;

    //elementの中身は初期値で固定→代わりにcopyを生成し、この値を変えてマージン測定を行う
    commandname << "josim MARGIN" << getpid() << ".cir > /dev/null"; 
    delete_cir << "rm -f MARGIN" << getpid() << ".cir";
    delete_out << "rm -f CIRCUIT" << getpid() << ".CSV";

 /*素子の値が正の場合*/
    if(element[ele_num].value > 0){                      
        value_h = element[ele_num].value * MARGIN_UPPER;
         //マージン上限探索
        HIGH = element[ele_num].value;
        while(check < 4){
            HIGH = HIGH + pow(10 , order_num - check);
            if(element[ele_num].ide_num == 1 && HIGH > 1){   // 結合係数 k ならば
                HIGH = HIGH - pow(10 , order_num - check);
                //synchro(copy, ele_num, HIGH);
                check += 1;
            }
            //synchro(copy, ele_num, HIGH);
            make_cir(HIGH, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            else if(judge_operation( jud, 0) == 0){
                HIGH = HIGH - pow(10 , order_num - check);
                //synchro(copy, ele_num, HIGH);
                check += 1;
            }
            else if(HIGH > value_h){
                    HIGH = value_h;
                    check = 4;
                    break;
                }
            
        }
        
        //マージン下限探索
        check = 0;
        copy = element;
        LOW = element[ele_num].value;

        while(check < 4){
            LOW = LOW - pow(10 , order_num - check);
            //synchro(copy, ele_num, LOW);
            if(LOW < 0.001){
                LOW = LOW + pow(10 , order_num - check);
                //synchro(copy, ele_num, LOW);
                check += 1;
            }
            make_cir(LOW, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 0){
                LOW = LOW + pow(10 , order_num - check);
                //synchro(copy, ele_num, LOW);
                check += 1;
            }
        }
    percent_HIGH = ( (HIGH - element[ele_num].value) / element[ele_num].value) * 100;
    percent_LOW = -( (element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }
 /*素子の値が負の場合*/
    else{
         //マージン上限探索
        HIGH = element[ele_num].value;
        while(check < 4){
            HIGH = HIGH + pow(10 , order_num - check);
            //synchro(copy, ele_num, HIGH);
            if(HIGH > 0){
                HIGH = HIGH - pow(10 , order_num - check);
                //synchro(copy, ele_num, HIGH);
                check += 1;
            }

            make_cir(HIGH, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 0){
                HIGH = HIGH - pow(10 , order_num - check);
                //synchro(copy, ele_num, HIGH);
                check += 1;
            }
        }
        //マージン下限探索
        check = 0;
        copy = element;
        LOW = element[ele_num].value;
        value_l = element[ele_num].value * MARGIN_UPPER;
        while(check < 4){
            LOW = LOW - pow(10 , order_num - check);
            if(element[ele_num].ide_num == 1 && LOW < -1){
                LOW = LOW + pow(10 , order_num - check);
                //synchro(copy, ele_num, LOW);
                check += 1;
            }
            //synchro(copy, ele_num, LOW);
            make_cir(LOW, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            else if(judge_operation( jud, 0) == 0){
                LOW = LOW + pow(10 , order_num - check);
                //synchro(copy, ele_num, LOW);
                check += 1;
            }
            else if(LOW < value_l){
                LOW = value_l;
                check = 4;
                
            }

        }

    percent_HIGH = ( -(HIGH - element[ele_num].value) / element[ele_num].value) * 100;
    percent_LOW = -( -(element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }

    string name = element[ele_num].element;
    if(element[ele_num].ide_num == 2 ||element[ele_num].ide_num == 3){
        name.replace(0,1,"J");
    }

    //top->msg[outline_num] = out_det.str();
    /*マージン図を作成*/
    top->margin[ele_num][0] = percent_LOW;
    top->margin[ele_num][1] = percent_HIGH;

    
    //cout << "  " << top->cri_m << "    " <<  top->line_num_cri <<  endl;
    //cout << "  " << top->bia_m[0] << ", "<< top->bia_m[1] << endl;

    //中間ファイル削除
    system((delete_cir.str()).c_str());
    system((delete_out.str()).c_str());
    
    return 0;
}

void Margin_low(vector<ele_unit> &element, vector<vector<judge>> &jud, vector<string> &data_cir, vector<string> &arg_arr, int menu){
    if(menu == 0 || menu == 2){
        cout << " Calculating Margins...                                                                                                             " << endl << endl;
    }

    if(menu == 1){
        cout << " Checking Critical Margin                                                                                                           " << endl << endl;
    }
    int shmid;
    int sum = element.size();
    board *top;
    vector<int> pid;
    
    /*共有メモリを確保し、IDをshmidに格納*/    //共有メモリはプロセス間で通信を行う（board構造体を用いた結果の挿入）ため必要である
    if ((shmid = shmget(IPC_PRIVATE, sizeof(board), 0666)) == -1) {
        cout << "can't make shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    
    /*結果を格納する top に共有メモリをアタッチ*/
    top = (board *)shmat(shmid, NULL, 0);
    //top->cri_m = 100;

    for(int i = 0; i < element.size(); i++){
            board  *shmaddr;
                //マルチプロセスL
                    pid.emplace_back(fork());
                    if(pid[i] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                        margin_ele_low(i,element,  jud, shmaddr, data_cir);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        exit(0);
                    }
                    else if(pid[i] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }
    
    }

    /*親プロセス待機 及び インジケータの実装*/
    cout << " [                                                  ]    0 % ";

    for (int i = 0; i < sum; i++) {
        string str = "";
        for (int j = 0; j < static_cast<int>((i / static_cast<double>(sum) * 100) / 2); j++) {
            str += "#";
        }
        cout << "\r [" << setw(50) << left << str << "]  " << setw(3) << right << static_cast<int>(i / static_cast<double>(sum) * 100) << " % ";
        cout.flush();
        this_thread::sleep_for(chrono::microseconds(50));
        wait(NULL);

    }

    cout << "\r" << " [##################################################]  100 % ";

    //マージンの値をelementのmargin_Lとmargin_Hに格納
    for(size_t i = 0; i < element.size(); i++){
        element[i].margin_L = top->margin[i][0];
        element[i].margin_H = top->margin[i][1];
    }
    switch(menu){
        case 0:
            fig_out(element);   
            file_out(arg_arr[1], element);  
            break;
        case 1:
            goto free; 
            break;
        case 2:
            fig_out(element);
            for (string cmd : arg_arr) {
                if (cmd == "-d") {    // -d があった場合、マージンの詳細情報を表示
                    detail_out(element);      
                }
            }
            goto free;
            break;
        default:
            break;
    }
    /*
    //マージンの図を表示
    fig_out(top, sum, element);   
    //マージンが格納されたファイル(result.txt, result.csv)を出力
    file_out(top, sum, arg_arr[1], element);  
    */
    //コマンドライン引数を検索し、種類に応じて処理を実行
    for (string cmd : arg_arr) {
        if (cmd == "-d") {    // -d があった場合、マージンの詳細情報を表示
            detail_out(element);      
        }
        if (cmd == "-f"){     // -f があった場合、 matplotlib を用いたグラフを出力
            cout << " Please wait for outputting the graph..." << endl;
            string path = PATH;              //23行目の #define PATH "展開したフォルダの絶対パス" を参照してgnuplot.pyまでの絶対パス文字列を作成
            string margin = "python " + path + "margin.py";     
            int result_py = system(margin.c_str());
            if (result_py != 0) {
                cerr << "Error executing Python script." << endl;
            }
        }
    }

    free:
    /*確保していた共有メモリを解放*/
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        cout << "can't delete shared memory";
        exit(EXIT_FAILURE);
    }

}




int margin_ele_low(int ele_num, vector<ele_unit> &element, vector<vector<judge>> &jud, board *top, vector<string> &data_cir){
    //int check = 0;
    int outline_num = ele_num;
    int index = 0;
    int index_ite = 10; 
    double value_ini,  HIGH, LOW, percent_HIGH, percent_LOW, median;
    //double value_h, value_l;
    string unit;
    stringstream commandname, delete_cir, delete_out, out_det, out_fig, out_csv;
    vector<ele_unit> copy = element;
    string name = element[ele_num].element;

    //elementの中身は初期値で固定→代わりにcopyを生成し、この値を変えてマージン測定を行う
    commandname << "josim MARGIN" << getpid() << ".cir > /dev/null"; 
    delete_cir << "rm -f MARGIN" << getpid() << ".cir";
    delete_out << "rm -f CIRCUIT" << getpid() << ".CSV";

    switch(element[ele_num].ide_num){
        case 0 :  
            unit = "H  "; 
            break;
        case 1 :  
            unit = "    ";
            break;
        case 2 :  
            unit = "    ";
            break;
        case 3 :  
            unit = "    ";
            break;
        case 4 :  
            unit = "    ";
            break;
        case 5 :  
            unit = "ohm ";
            break;
        case 6 :  
            unit = "V  ";
            break;
        case 7 :  
            unit = "A  ";
            break;
        default :  break;
    }

 /*素子の値が正の場合*/
    //if(element[ele_num].value > 0){   
        index = 0;     
        value_ini = element[ele_num].value;           
         //マージン上限探索(負の場合下限)
        HIGH = element[ele_num].value;
        while(index < index_ite){
            make_cir(HIGH, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 1){
                HIGH += value_ini / pow(2, index + 1);
            }
            else{
                HIGH -= value_ini / pow(2, index + 1);
            }
            index++;
        }
        make_cir(HIGH, ele_num, copy, data_cir);
        if(system((commandname.str()).c_str()) == -1){
            cout << "error:1" << endl;
        }
        if(judge_operation( jud, 0) == 0){
            HIGH -= value_ini / pow(2, index);
        }         

            //cout << endl << name << endl;
            
        //マージン下限探索(正の場合上限)
        index = 0;     
        value_ini = element[ele_num].value;           
        LOW = element[ele_num].value;
        while(index < index_ite){
            make_cir(LOW, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 1){
                LOW -= value_ini / pow(2, index + 1);
            }
            else{
                LOW += value_ini / pow(2, index + 1);
            }
            index++;
        }
        make_cir(LOW, ele_num, copy, data_cir);
        if(system((commandname.str()).c_str()) == -1){
            cout << "error:1" << endl;
        }
        if(judge_operation( jud, 0) == 0){
            LOW += value_ini / pow(2, index);
        }        

    if(value_ini < 0){
        swap(HIGH, LOW);
        percent_HIGH = ( -(HIGH - element[ele_num].value) / element[ele_num].value) * 100;
        percent_LOW = -( -(element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }
    else{
        percent_HIGH = ( (HIGH - element[ele_num].value) / element[ele_num].value) * 100;
        percent_LOW = -( (element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }
    median = (HIGH + LOW)/2;



    if(element[ele_num].ide_num == 2 ||element[ele_num].ide_num == 3){
        name.replace(0,1,"J");
    }


    top->margin[ele_num][0] = percent_LOW;
    top->margin[ele_num][1] = percent_HIGH;

    
    //cout << "  " << top->cri_m << "    " <<  top->line_num_cri <<  endl;
    //cout << "  " << top->bia_m[0] << ", "<< top->bia_m[1] << endl;

    //中間ファイル削除
    system((delete_cir.str()).c_str());
    system((delete_out.str()).c_str());
    
    return 0;


}





/*
// マージン計算関数 (子プロセスで呼び出される)
void margin_ele_2(int ele_num, std::vector<ele_unit>& element, std::vector<std::string>&  std::vector<std::vector<judge>>& jud, board_bp* top, std::vector<std::string>& data_cir) {
    margin_ele_bp(ele_num, element,  jud, top, data_cir);  // 元の関数を使用
}

void Margin_bp(std::vector<ele_unit>& element, std::vector<std::string>&  std::vector<std::vector<judge>>& jud, std::vector<std::string>& data_cir, std::vector<std::string>& arg_arr, int menu) {
    if (menu == 0 || menu == 2) {
        std::cout << " Calculating Margins..." << std::endl << std::endl;
    }

    if (menu == 1) {
        std::cout << " Checking Critical Margin..." << std::endl << std::endl;
    }

    int sum = element.size();
    pid_t pid;

    // 共有メモリを Boost.Interprocess で確保
    bip::shared_memory_object::remove("MySharedMemory");
    bip::managed_shared_memory segment(bip::create_only, "MySharedMemory", 65536);

    // 共有メモリ上に board 構造体を作成
    board_bp *top = segment.construct<board_bp>("SharedBoard")(sum);
    //top->cri_m = 100;

    std::vector<pid_t> pids;

    // 各プロセスを fork して margin_ele 関数を並列に実行
    for (int i = 0; i < element.size(); i++) {
        pid = fork();

        if (pid == 0) {  // 子プロセス
            margin_ele_bp(i, element,  jud, top, data_cir);

            // 共有メモリから切断（子プロセス側）
            segment.destroy_ptr(top);
            exit(0);  // 子プロセス終了
        } 
        else if (pid > 0) {  // 親プロセス
            pids.push_back(pid);
        } 
        else {
            std::cerr << "fork failed!" << std::endl;
            exit(1);
        }
    }

    // 親プロセス待機 & インジケータ表示
    std::cout << " [                                                  ]    0 % ";

    for (int i = 0; i < sum; i++) {
        std::string str = "";
        for (int j = 0; j < static_cast<int>((i / static_cast<double>(sum) * 100) / 2); j++) {
            str += "#";
        }
        std::cout << "\r [" << std::setw(50) << std::left << str << "]  " << std::setw(3) << std::right << static_cast<int>(i / static_cast<double>(sum) * 100) << " % ";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::microseconds(50));

        wait(NULL);  // 子プロセスが終了するまで待機
    }

    std::cout << "\r" << " [##################################################]  100 % ";

    // 各プロセスの終了を待機
    for (pid_t child_pid : pids) {
        waitpid(child_pid, NULL, 0);  // 子プロセスの終了待機
    }

    // マージンの値を element に格納
    for (size_t i = 0; i < element.size(); i++) {
        element[i].margin_L = top->margin[i][0];
        element[i].margin_H = top->margin[i][1];
        //cout << top->fig[i] << endl;
        //cout << "LOW = " << element[i].margin_L << ", HIGH = " << element[i].margin_H << endl;
    }


    // メニューの選択による分岐
    // コマンドライン引数処理

    // 共有メモリの解放（親プロセス側）
    segment.destroy<board>("SharedBoard");
    bip::shared_memory_object::remove("MySharedMemory");
}

全素子のマージン測定 
int margin_ele_bp(int ele_num, vector<ele_unit> &element, vector<vector<judge>> &jud, board_bp *top, vector<string> &data_cir){
    int check = 0;
    int order_num = get_digit(element[ele_num].value);
    int outline_num = ele_num;
    double value_h, value_l, HIGH, LOW, percent_HIGH, percent_LOW, median;
    string unit;
    stringstream commandname, delete_cir, delete_out, out_det, out_fig, out_csv;
    vector<ele_unit> copy = element;

    //elementの中身は初期値で固定→代わりにcopyを生成し、この値を変えてマージン測定を行う
    commandname << "josim MARGIN" << getpid() << ".cir > /dev/null"; 
    delete_cir << "rm -f MARGIN" << getpid() << ".cir";
    delete_out << "rm -f CIRCUIT" << getpid() << ".CSV";

    switch(element[ele_num].ide_num){
        case 0 :  
            unit = "H  ";
            break;
        case 1 :  
            unit = "    ";
            break;
        case 2 :  
            unit = "    ";
            break;
        case 3 :  
            unit = "    ";
            break;
        case 4 :  
            unit = "    ";
            break;
        case 5 :  
            unit = "ohm ";
            break;
        case 6 :  
            unit = "V  ";
            break;
        case 7 :  
            unit = "A  ";
            break;
        default :  break;
    }
 素子の値が正の場合
    if(element[ele_num].value > 0){                      
        value_h = element[ele_num].value * MARGIN_UPPER;
         //マージン上限探索
        HIGH = element[ele_num].value;
        while(check < 4){
            HIGH = HIGH + pow(10 , order_num - check);
            if(element[ele_num].ide_num == 1 && HIGH > 1){   // 結合係数 k ならば
                HIGH = HIGH - pow(10 , order_num - check);
                //synchro(copy, ele_num, HIGH);
                check += 1;
            }
            //synchro(copy, ele_num, HIGH);
            make_cir(HIGH, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            else if(judge_operation( jud, 0) == 0){
                HIGH = HIGH - pow(10 , order_num - check);
                //synchro(copy, ele_num, HIGH);
                check += 1;
            }
            else if(HIGH > value_h){
                    HIGH = value_h;
                    check = 4;
                    break;
                }
            
        }
        
        //マージン下限探索
        check = 0;
        copy = element;
        LOW = element[ele_num].value;

        while(check < 4){
            LOW = LOW - pow(10 , order_num - check);
            //synchro(copy, ele_num, LOW);
            if(LOW < 0.001){
                LOW = LOW + pow(10 , order_num - check);
                //synchro(copy, ele_num, LOW);
                check += 1;
            }
            make_cir(LOW, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 0){
                LOW = LOW + pow(10 , order_num - check);
                //synchro(copy, ele_num, LOW);
                check += 1;
            }
        }
    percent_HIGH = ( (HIGH - element[ele_num].value) / element[ele_num].value) * 100;
    percent_LOW = -( (element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }
 素子の値が負の場合
    else{
         //マージン上限探索
        HIGH = element[ele_num].value;
        while(check < 4){
            HIGH = HIGH + pow(10 , order_num - check);
            //synchro(copy, ele_num, HIGH);
            if(HIGH > 0){
                HIGH = HIGH - pow(10 , order_num - check);
                //synchro(copy, ele_num, HIGH);
                check += 1;
            }

            make_cir(HIGH, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 0){
                HIGH = HIGH - pow(10 , order_num - check);
                //synchro(copy, ele_num, HIGH);
                check += 1;
            }
        }
        //マージン下限探索
        check = 0;
        copy = element;
        LOW = element[ele_num].value;
        value_l = element[ele_num].value * MARGIN_UPPER;
        while(check < 4){
            LOW = LOW - pow(10 , order_num - check);
            if(element[ele_num].ide_num == 1 && LOW < -1){
                LOW = LOW + pow(10 , order_num - check);
                //synchro(copy, ele_num, LOW);
                check += 1;
            }
            //synchro(copy, ele_num, LOW);
            make_cir(LOW, ele_num, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            else if(judge_operation( jud, 0) == 0){
                LOW = LOW + pow(10 , order_num - check);
                //synchro(copy, ele_num, LOW);
                check += 1;
            }
            else if(LOW < value_l){
                LOW = value_l;
                check = 4;
                
            }

        }

    percent_HIGH = ( -(HIGH - element[ele_num].value) / element[ele_num].value) * 100;
    percent_LOW = -( -(element[ele_num].value - LOW) / element[ele_num].value) * 100;
    }
    median = (HIGH + LOW)/2;

    string name = element[ele_num].element;
    if(element[ele_num].ide_num == 2 ||element[ele_num].ide_num == 3){
        name.replace(0,1,"J");
    }

    out_det << setw(6) << name << " " << fixed  << right << setw(6) << setprecision(3) << element[ele_num].value  << " " <<  element[ele_num].unit << unit << " Lower: "  << setprecision(3) << setw(6) << LOW << "   Upper: "
                << setw(6) << setprecision(3)  << HIGH << "   Median: " << setw(6) << setprecision(3)  << median << "  "  << setprecision(2) << setw(7) << percent_LOW << " % ~ "  << setprecision(2) << setw(6) << percent_HIGH << " %";
    top->msg[ele_num] = out_det.str();

    //マージン図を作成
    vector<string> hush(2);
    vector<double> hush_num{-percent_LOW, percent_HIGH};
    for( int j = 0 ; j < 2 ; j++ ){
        if(hush_num[j] > 50){
                hush_num[j] = 50; 
            }
        for ( int k = 0 ; k < (int)ceil(hush_num[j] / 2) ; k++){ 
            hush[j] += MARGIN_FIG;
            }
        }
    

    out_fig << setw(6) << name << "  :  " << fixed  << right << setprecision(2) << setw(6) << -hush_num[0] << " % ["
                << setw(25) << hush[0] << "|" << left << setw(25) << hush[1] << "]" << right << setprecision(2) << setw(6) << hush_num[1] << " %";
    top->fig[ele_num] = out_fig.str();
    //cout << out_fig.str() << endl;


    out_csv << setw(5) << name << "," << setw(6) << setprecision(3) << percent_LOW << "," << setw(6) << setprecision(3) << percent_HIGH;
    //strcpy(top->fig_csv[outline_num],  (out_csv.str()).c_str());
    top->fig_csv[ele_num] = out_csv.str();

    top->margin[ele_num][0] = percent_LOW;
    top->margin[ele_num][1] = percent_HIGH;

    
    //cout << "  " << top->cri_m << "    " <<  top->line_num_cri <<  endl;
    //cout << "  " << top->bia_m[0] << ", "<< top->bia_m[1] << endl;

    //中間ファイル削除
    system((delete_cir.str()).c_str());
    system((delete_out.str()).c_str());
    
    return 0;
}

*/

int find_critical(vector<ele_unit> &element){
    double critical = 100;
    int cri_num;
    for(size_t j = 0; j < element.size(); j++){
        if(element[j].ide_num != 6 && element[j].ide_num != 7){
            if(-element[j].margin_L < critical){
                critical = -element[j].margin_L;
                cri_num = j;
            }
            if(element[j].margin_H < critical){
                critical = element[j].margin_H;
                cri_num = j;
            }
        }
    }
    return cri_num;
}

int find_critical_bias(vector<ele_unit> &element){
    double critical = 100;
    int cri_num = 0;
    for(size_t j = 0; j < element.size(); j++){
        if(element[j].ide_num == 6 || element[j].ide_num == 7){
            if(-element[j].margin_L < critical){
                critical = -element[j].margin_L;
                cri_num = j;
            }
            if(element[j].margin_H < critical){
                critical = element[j].margin_H;
                cri_num = j;
            }
        }
    }
    return cri_num;
}


int get_digit(double num){  //桁数を取得する関数 (□ × 10 ^ ○) の ○ の部分を取得
    double digit;  //出力する桁数
    if(num == 0){
        return 0;
    }
    digit = floor(log10(abs(num))); //入力numの絶対値を常用対数にいることで桁数を取得
    return digit;
}



/*
void Margin_low_new(vector<vector<ele_unit>> &elements, vector<vector<judge>> &jud, vector<string> &data_cir, ele_cou *cou, vector<string> &arg_arr, int menu){
    if(menu == 0 || menu == 2){
        cout << " Calculating Margins...                                                                                                             " << endl << endl;
    }

    if(menu == 1){
        cout << " Checking Critical Margin                                                                                                           " << endl << endl;
    }
    int sum = 0;

    for(int i = 0; i < elements.size(); i++){
        sum += elements[i].size();
    }


    int shmid;
    board *top;
    vector<vector<int>> pid;
    
    //共有メモリを確保し、IDをshmidに格納/    //共有メモリはプロセス間で通信を行う（board構造体を用いた結果の挿入）ため必要である
    if ((shmid = shmget(IPC_PRIVATE, sizeof(board), 0666)) == -1) {
        cout << "can't make shared memory" << endl;
        exit(EXIT_FAILURE);
    }

    
    //結果を格納する top に共有メモリをアタッチ/
    top = (board *)shmat(shmid, NULL, 0);
    top->cri_m = 100;

    int L_num = 0, K_num = 0, B_num = 0, BI_num = 0, PI_num = 0, R_num = 0, V_num = 0, I_num = 0;
    for(int i = 0; i < elements.size(); i++){
        for(int j = 0; j < elements[i].size(); j++){
            board  *shmaddr;
                //マルチプロセス
                    pid[i].emplace_back(fork());
                    if(pid[i][j] == 0){
                        if ((shmaddr = (board*)shmat(shmid, NULL, 0)) == (void *)-1) {
                            cerr << "childL can't load memory" << endl;
                            exit(EXIT_FAILURE);
                        }
                        margin_ele_low_new(i, j,elements,  jud, shmaddr, data_cir);
                        if (shmdt(shmaddr) == -1) {
                            exit(EXIT_FAILURE);
                        }
                        exit(0);
                    }
                    else if(pid[i][j] < 0){
                        cerr << "cann't make child process" << endl;
                        exit(0);
                    }
                    L_num++;
                    break;

        }
    }

    //親プロセス待機 及び インジケータの実装/
    cout << " [                                                  ]    0 % ";

    for (int i = 0; i < sum; i++) {
        for(int j = 0; j < elements[i].size(); j++){
        string str = "";
        for (int sh = 0; sh < static_cast<int>((i / static_cast<double>(sum) * 100) / 2); sh++) {
            str += "#";
        }
        cout << "\r [" << setw(50) << left << str << "]  " << setw(3) << right << static_cast<int>(i / static_cast<double>(sum) * 100) << " % ";
        cout.flush();
        this_thread::sleep_for(chrono::microseconds(50));
        wait(NULL);
        }
    }
    

    cout << "\r" << " [##################################################]  100 % ";

    //マージンの値をelementのmargin_Lとmargin_Hに格納
    for(size_t i = 0; i < element.size(); i++){
        element[i].margin_L = top->margin[i][0];
        element[i].margin_H = top->margin[i][1];
    }
    switch(menu){
        case 0:
            fig_out(top, sum, element);   
            file_out(top, sum, arg_arr[1], element);  
            break;
        case 1:
            goto free; 
            break;
        case 2:
            fig_out(top, sum, element);
            for (string cmd : arg_arr) {
                if (cmd == "-d") {    // -d があった場合、マージンの詳細情報を表示
                    detail_out(top, sum);      
                }
            }
            goto free;
            break;
        default:
            break;
    }
    /*
    //マージンの図を表示
    fig_out(top, sum, element);   
    //マージンが格納されたファイル(result.txt, result.csv)を出力
    file_out(top, sum, arg_arr[1], element);  
    /
    //コマンドライン引数を検索し、種類に応じて処理を実行
    for (string cmd : arg_arr) {
        if (cmd == "-d") {    // -d があった場合、マージンの詳細情報を表示
            detail_out(top, sum);      
        }
        if (cmd == "-f"){     // -f があった場合、 matplotlib を用いたグラフを出力
            cout << " Please wait for outputting the graph..." << endl;
            string path = PATH;              //23行目の #define PATH "展開したフォルダの絶対パス" を参照してgnuplot.pyまでの絶対パス文字列を作成
            string margin = "python " + path + "margin.py";     
            int result_py = system(margin.c_str());
            if (result_py != 0) {
                cerr << "Error executing Python script." << endl;
            }
        }
    }

    free:
    //確保していた共有メモリを解放
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        cout << "can't delete shared memory";
        exit(EXIT_FAILURE);
    }

}

*/
/*
int margin_ele_low_new(int ele_row, int ele_col, vector<vector<ele_unit>> &elements, vector<vector<judge>> &jud, board *top, vector<string> &data_cir, ele_cou *cou){
    //int check = 0;
    int outline_num = 0;
    int index = 0;
    int index_ite = 15;
    double value_ini,  HIGH, LOW, percent_HIGH, percent_LOW, median;
    //double value_h, value_l;
    string unit;
    stringstream commandname, delete_cir, delete_out, out_det, out_fig, out_csv;
    vector<vector<ele_unit>> copy = elements;
    string name = elements[ele_row][ele_col].element;

    //elementの中身は初期値で固定→代わりにcopyを生成し、この値を変えてマージン測定を行う
    commandname << "josim MARGIN" << getpid() << ".cir > /dev/null"; 
    delete_cir << "rm -f MARGIN" << getpid() << ".cir";
    delete_out << "rm -f CIRCUIT" << getpid() << ".CSV";

    switch(elements[ele_row][ele_col].ide_num){
        case 0 :  
            unit = " pH ";
            break;
        case 1 :  
            unit = "    ";
            break;
        case 2 :  
            unit = "    ";
            break;
        case 3 :  
            unit = "    ";
            break;
        case 4 :  
            unit = "    ";
            break;
        case 5 :  
            unit = " ohm";
            break;
        case 6 :  
            unit = " mV ";
            break;
        case 7 :  
            unit = " mA ";
            break;
        default :  break;
    }

 //素子の値が正の場合
     //if(element[ele_num].value > 0){   
        index = 0;     
        value_ini = elements[ele_row][ele_col].value;           
         //マージン上限探索(負の場合下限)
        HIGH = value_ini;
        while(index < index_ite){
            make_cir_new(HIGH, ele_row, ele_col, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 1){
                HIGH += value_ini / pow(2, index + 1);
            }
            else{
                HIGH -= value_ini / pow(2, index + 1);
            }
            index++;
        }
        make_cir_new(HIGH, ele_row, ele_col, copy, data_cir);   //最後の値変更後、正常動作するか確認
        if(system((commandname.str()).c_str()) == -1){
            cout << "error:1" << endl;
        }
        if(judge_operation( jud, 0) == 0){
            HIGH -= value_ini / pow(2, index);
        }        

            //cout << endl << name << endl;
            
        //マージン下限探索(正の場合上限)
        index = 0;     
        LOW = value_ini;
        while(index < index_ite){
            make_cir_new(LOW, ele_row, ele_col, copy, data_cir);
            if(system((commandname.str()).c_str()) == -1){
                cout << "error:1" << endl;
            }
            if(judge_operation( jud, 0) == 1){
                LOW -= value_ini / pow(2, index + 1);
            }
            else{
                LOW += value_ini / pow(2, index + 1);
            }
            index++;
        }
        make_cir_new(LOW, ele_row, ele_col, copy, data_cir);
        if(system((commandname.str()).c_str()) == -1){
            cout << "error:1" << endl;
        }
        if(judge_operation( jud, 0) == 0){
            LOW += value_ini / pow(2, index);
        }        



    if(value_ini < 0){
        swap(HIGH, LOW);
        HIGH = HIGH ;
        LOW  = LOW  ;
        percent_HIGH = ( -(HIGH - value_ini) / value_ini) * 100;
        percent_LOW = -( -(value_ini - LOW) / value_ini) * 100;
    }
    else{
        percent_HIGH = ( (HIGH - value_ini) / value_ini) * 100;
        percent_LOW = -( (value_ini - LOW) / value_ini) * 100;
    }
    median = (HIGH + LOW)/2;



    if(elements[ele_row][ele_col].ide_num == 2 ||elements[ele_row][ele_col].ide_num == 3){
        name.replace(0,1,"J");
    }




    out_det << setw(6) << name << " " << fixed  << right << setw(6) << setprecision(3) << element[ele_num].value  << unit << " Lower: "  << setprecision(3) << setw(6) << LOW << "   Upper: "
                << setw(6) << setprecision(3)  << HIGH << "   Median: " << setw(6) << setprecision(3)  << median << "  "  << setprecision(2) << setw(7) << percent_LOW << " % ~ "  << setprecision(2) << setw(6) << percent_HIGH << " %";

    strcpy(top->msg[outline_num], (out_det.str()).c_str());
    //top->msg[outline_num] = out_det.str();
    //マージン図を作成
    vector<string> hush(2);
    vector<double> hush_num{-percent_LOW, percent_HIGH};
    for( int j = 0 ; j < 2 ; j++ ){
        if(hush_num[j] > 50){
                hush_num[j] = 50;
            }
        for ( int k = 0 ; k < (int)ceil(hush_num[j] / 2) ; k++){
            hush[j] += MARGIN_FIG;
            }
        }
    

    out_fig << setw(6) << name << "  :  " << fixed  << right << setprecision(2) << setw(6) << -hush_num[0] << " % ["
                << setw(25) << hush[0] << "|" << left << setw(25) << hush[1] << "]" << right << setprecision(2) << setw(6) << hush_num[1] << " %";
    strcpy(top->fig[outline_num], (out_fig.str()).c_str());

    out_csv << setw(5) << name << "," << setw(6) << setprecision(3) << percent_LOW << "," << setw(6) << setprecision(3) << percent_HIGH;
    strcpy(top->fig_csv[outline_num],  (out_csv.str()).c_str());

    top->margin[ele_num][0] = percent_LOW;
    top->margin[ele_num][1] = percent_HIGH;

    
    //cout << "  " << top->cri_m << "    " <<  top->line_num_cri <<  endl;
    //cout << "  " << top->bia_m[0] << ", "<< top->bia_m[1] << endl;

    //中間ファイル削除
    system((delete_cir.str()).c_str());
    system((delete_out.str()).c_str());
    
    return 0;


}
*/