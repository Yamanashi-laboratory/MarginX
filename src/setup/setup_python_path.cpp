#include <iostream>
#include <vector>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <iomanip> 
#include <string>
#include <filesystem>
#include "function.hpp"

using namespace std;

void setup_python_path(vector<string> arg_arr){
    int cou = 0;
    int lineNumberToReplace = 0;

    // 実行ファイルのパスを取得
    filesystem::path executablePath = filesystem::absolute(filesystem::path(arg_arr[0]));
    // ディレクトリ部分を取得
    filesystem::path directoryPath = executablePath.parent_path();

    // ディレクトリの絶対パスを文字列として取得
    string absoluteDirectoryPath = directoryPath.string();
    absoluteDirectoryPath.pop_back();

    vector<string> lines;
    string line;
    string replacementText = "#define PATH \"";
    replacementText += absoluteDirectoryPath + "python_source/";
    replacementText += "\"";

    string path_to_hpp = absoluteDirectoryPath + "include/function.hpp";   
    ifstream fmargin(path_to_hpp);
    if (!fmargin){
        cout << " ERROR : Can't open file!" << endl;
        exit(1);
    }
    while (getline(fmargin, line)) {
        lines.push_back(line);
        if(line.find("#define PATH") != string::npos){
            lineNumberToReplace = cou;
        }
        cou++;
    }
    fmargin.close();
    cout << " Changed path to margin.py" << endl;
    cout << " Path to margin.py : " << absoluteDirectoryPath + "python_source/" << endl;

    // ファイルの10行目を変換する（0ベースのインデックスなので9行目）
    lines[lineNumberToReplace] = replacementText;

    // ファイルに変更内容を書き込む
    ofstream outputFile(path_to_hpp);
    if (!outputFile){
        cout << " ERROR : Can't open file!" << endl;
        exit(1);
    }
    
    for (const string& updatedLine : lines) {
        outputFile << updatedLine << endl;
    }

    outputFile.close();
}


