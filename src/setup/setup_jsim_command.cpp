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

void setup_jsim_command(vector<string> arg_arr){
    int cou = 0;
    int lineNumberToReplace = 0;
    string jsim_commnad = "";

    // 実行ファイルのパスを取得
    filesystem::path executablePath = filesystem::absolute(filesystem::path(arg_arr[0]));
    // ディレクトリ部分を取得
    filesystem::path directoryPath = executablePath.parent_path();

    // ディレクトリの絶対パスを文字列として取得
    string absoluteDirectoryPath = directoryPath.string();

    if(absoluteDirectoryPath.rfind(".") != string::npos){
        absoluteDirectoryPath.pop_back();
    }

    //absoluteDirectoryPath.pop_back();
    string absoluteDirectoryPath_cmake = absoluteDirectoryPath + "../include/function.hpp";
    absoluteDirectoryPath += "/include/function.hpp";
    //cout << absoluteDirectoryPath << endl;

    if(!filesystem::is_regular_file(absoluteDirectoryPath)){ //if there is [input].cir, change the name of filename to .inp
        absoluteDirectoryPath = absoluteDirectoryPath_cmake;
    }
            cout << absoluteDirectoryPath << endl;

    ifstream fmargin(absoluteDirectoryPath);
    if (!fmargin){

            cout << "can't open file!" << endl;
            exit(1);
        
    }

    vector<string> lines;
    string line;
    string replacementText = "#define JSIM_COMMAND \"";


    while (getline(fmargin, line)) {
        lines.push_back(line);
        if(line.find("#define JSIM_COMMAND") != string::npos){
            lineNumberToReplace = cou;
        }
        cou++;
    }
    fmargin.close();

    cout << " PLease input command to execute JSIM in your environemnt : " ;
    cin >> jsim_commnad; 
    cout << endl;

    replacementText = replacementText + jsim_commnad + "\"";

    // ファイルの10行目を変換する（0ベースのインデックスなので9行目）
    lines[lineNumberToReplace] = replacementText;

    // ファイルに変更内容を書き込む
    ofstream outputFile(absoluteDirectoryPath);
    for (const string& updatedLine : lines) {
        outputFile << updatedLine << endl;
    }

    outputFile.close();
}


