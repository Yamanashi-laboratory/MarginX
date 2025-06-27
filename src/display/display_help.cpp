#include "function.hpp"
#include <iostream>
using namespace std;

void display_help (){
    
        cout << " MarginX : Margin Calculator and Circuit Optimizer from Yamanashi Lab, YNU" << endl;
        cout << "           Created by Sho Matsuoka (in 2023) and Nakaishi Sotaro (in 2017)" << endl;
        cout << endl << endl;
        cout << " MarginX help" << endl;
        cout << "--------------------------------------------------------------------------" << endl << endl;
        cout << " You must prepare 2 Files: Circuit File which is suitable for JoSIM" << endl;
        cout << " and Judgement File which defines correct operations " << endl;
        cout << " In Circuit File, you must output phase of Josephson Junction (JJ)" << endl;
        cout << " Example :)  .print phase B1.XI10 " << endl << endl;
        cout << " In Judgement File, you must write switchign time of JJs like below." << endl;
        cout << " \"begining tim\"   \"end time\"  \"phase \" " << endl;
        cout << " If the phase of targeted JJ is passed between begining time and end time," << endl;
        cout << " it is regarded as correct operation. " << endl;
        cout << " Phases of JJ are standardized with π" << endl;
        cout << " Example :)  " << endl;
        cout << "             B1 " << endl;
        cout << "             100   200   1" << endl;
        cout << "             300   400   3" << endl;
        cout << "             400   500   5" << endl << endl;
        cout << "             B3  " << endl;
        cout << "             200   300   -1" << endl;
        cout << "             300   400   -3" << endl;
        cout << "             900   1000  -5" << endl;
        cout << "                    ・ " << endl;
        cout << "                    ・ " << endl;
        cout << "                    ・ " << endl << endl;
        cout << " Note that you must make Judgement file name the same as Circuit file name  " << endl;
        cout << " without extension." << endl << endl;;
        cout << "--------------------------------------------------------------------------" << endl << endl;
        cout << "There are some commands below." << endl << endl;
        cout << " -d            |   You can see the details of results. you can also see them   " << endl;
        cout << " (details)     |   in outputted text file.    " << endl;
        cout << "               |         " << endl;
        cout << "               |         " << endl;
        cout << " -f            |   the output of a figure of Margins. You can also make   " << endl;
        cout << " (figure)      |   figures of Margins from CSV file normally outputted " << endl;
        cout << "               |         " << endl;
        cout << "               |         " << endl;
        cout << " -o            |   change outputted file name. You must write new outputted   " << endl;
        cout << " (output)      |   file name without extension to the right of \"-o\"" << endl;
        cout << "               |         " << endl;
        cout << "               |         " << endl;
        cout << " -j            |   Specify optional name of Judgement File. You must write     " << endl;
        cout << " (jud file)    |   optional judgement file name without extensionto the "<< endl;
        cout << "               |   right of \"-j\"" << endl;
        cout << "               |         " << endl;
        cout << "               |         " << endl << endl;
        cout << " Example :)  ./Marginx ex02 -j ex01 -o hoge -f" << endl << endl << endl;
        exit(0);

}