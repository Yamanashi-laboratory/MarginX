#include "function.hpp"
#include <iostream>
using namespace std;

void display_title (vector<string> &arg_arr){
    cout << "\n";
    cout << "    MM             MM                                         ii                 'XX         XX'        \n";
    cout << "    MMM           MMM                                                              XXX     XXX         \n";
    cout << "    MMMM         MMMM      ......     ..  ....     .....      ..    .. .....        XXX   XXX         \n";
    cout << "    MM MM       MM MM    .aAAAAAa.    RRrr'''   .gGGGGGgg.    II    NN.nNNNNn.        XXxXX          \n";
    cout << "    MM  MM     MM  MM           AA    RR'      .gg      Gg    II    NN      NN         xXx            \n";
    cout << "    MM   MM   MM   MM     aAAAAAAA    RR       gg.      Gg    II    NN      NN        XXxXX             \n";
    cout << "    MM    MM MM    MM    Aa     aAa   RR       gg.      Gg    II    NN      NN      XXX   XXX           \n";
    cout << "    MM     MMM     MM    Aa     aAa   RR        gg     .Gg    II    NN      NN     XXX     XXX         \n";
    cout << "    MM      m      MM     aAAAAAAa    RR         gGGGGGggg    II    NN      NN   .xX         Xx.         \n";
    cout << "                                                        Gg                                                \n";
    cout << "                                                gg.   .gg                                                  \n";
    cout << "                                                 'gGGGg'                                                   \n";
    cout << "                                                                            Version 2.1               \n";
    cout << " ~~ Parallel Algorithm with Josim ~~ " << endl << endl;


    if(arg_arr.size() == 1){
        cout << " MarginX is Margin Calculator and Circuit Optimizer from Yamanashi Lab, YNU" << endl;
        cout << " Pleae prepare some files to calculate margin or optimize your circuit" << endl;
        cout << " Before executing MarginX, please check settings to use \"-s\" command. " << endl;
        cout << " If you want more infomation, you can use \"-h\" command." << endl << endl;
        exit(0);
    }

}