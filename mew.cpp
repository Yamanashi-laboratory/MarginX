#include <iostream>
#include <vector>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <iomanip> 
#include <string>
#include <filesystem>

using namespace std;
int main(int argc, const char *argv[]){
    ifstream fmarginx("MarginX.cpp");
    if (!fmarginx){
        cout << "can't open file!" << endl;
        return 0;
    }

    vector<string> lines;
    string line;
    while (getline(fmarginx, line)) {
        lines.push_back(line);   
    }
    fmarginx.close();

    // ファイルの10行目を変換する（0ベースのインデックスなので9行目）
    string replacementText = "            commandname << \"josim_v265 OPTIMIZE\" << getpid() << \".cir > /dev/null\"; ";   
    int lineNumberToReplace;
    for (int i = 0; i < lines.size(); i++){
        if( line.find("josim OPTIMIZE") != string::npos){
            lineNumberToReplace = i;    
        }
    }
    lines[lineNumberToReplace] = replacementText;

    // ファイルに変更内容を書き込む
    ofstream outputFile("MarginX.cpp");
    for (const string& updatedLine : lines) {
        outputFile << updatedLine << endl;
    }
    outputFile.close();



    ifstream fopt("optimize.cpp");
    if (!fopt){
        cout << "can't open file!" << endl;
        return 0;
    }
    vector<string> lines2;
    string line2;
    while (getline(fopt, line2)) {
        lines2.push_back(line2);   
    }
    fopt.close();
    // ファイルの10行目を変換する（0ベースのインデックスなので9行目）
    vector<int> linenums;
    for (int i = 0; i < lines2.size(); i++){
        if( line2.find("josim OPTIMIZE") != string::npos){
            linenums.emplace_back(i);  
        }
    }
    for(int num : linenums){
        lines2[num] = replacementText;
    }
    // ファイルに変更内容を書き込む
    ofstream outputFile2("optimize.cpp");
    for (const string& updatedLine2 : lines2) {
        outputFile2 << updatedLine2 << endl;
    }
    outputFile2.close();



    ifstream fmargin("margin.cpp");
    if (!fmargin){
        cout << "can't open file!" << endl;
        return 0;
    }

    vector<string> lines3;
    string line3;
    while (getline(fmargin, line3)) {
        lines3.push_back(line3);   
    }
    fmargin.close();

    // ファイルの10行目を変換する（0ベースのインデックスなので9行目）
    string replacementText2 = "            commandname << \"josim_v265 MARGIN\" << getpid() << \".cir > /dev/null\"; ";   
    int lineNumberToReplace2;
    for (int i = 0; i < lines3.size(); i++){
        if( line3.find("josim MARGIN") != string::npos){
            lineNumberToReplace2 = i;    
        }
    }
    lines3[lineNumberToReplace2] = replacementText2;

    // ファイルに変更内容を書き込む
    ofstream outputFile3("margin.cpp");
    for (const string& updatedLine3 : lines3) {
        outputFile3 << updatedLine3 << endl;
    }
    outputFile3.close();



    return 0;
}


