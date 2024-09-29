#include <iostream>
#include <vector>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <iomanip> 
#include <string>
#include <sstream>
#include <filesystem>
#include "function.hpp"

using namespace std;

void setup_josim_command(){
    int cou = 0;
    int lineNumberToReplace = 0;
    string josim_commnad = "";

    ifstream fmargin("function.hpp");
    if (!fmargin){
        cout << "can't open file!" << endl;
        exit(1);
    }

    vector<string> lines;
    string line;
    string replacementText = "#define JOSIM_COMMAND ";


    while (getline(fmargin, line)) {
        lines.push_back(line);
        if(line.find("#define JOSIM_COMMAND") != string::npos){
            lineNumberToReplace = cou;
            break;
        }
        cou++;
    }
    fmargin.close();

    cout << " PLease input command to execute JoSIM in your environemnt : " ;
    cin >> josim_commnad; 
    cout << endl;

    replacementText += josim_commnad;

    // ファイルの10行目を変換する（0ベースのインデックスなので9行目）
    lines[lineNumberToReplace] = replacementText;

    // ファイルに変更内容を書き込む
    ofstream outputFile("function.hpp");
    for (const string& updatedLine : lines) {
        outputFile << updatedLine << endl;
    }

    outputFile.close();
    fmargin.close();
}


