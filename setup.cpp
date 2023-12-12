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
    ifstream fmargin("margin.hpp");
    if (!fmargin){
        cout << "can't open file!" << endl;
        return 0;
    }
    // 実行ファイルのパスを取得
    filesystem::path executablePath = filesystem::absolute(filesystem::path(argv[0]));

    // ディレクトリ部分を取得
    filesystem::path directoryPath = executablePath.parent_path();

    // ディレクトリの絶対パスを文字列として取得
    string absoluteDirectoryPath = directoryPath.string();
    absoluteDirectoryPath.pop_back();

    vector<string> lines;
    string line;
    string replacementText = "#define PATH \"";
    replacementText += absoluteDirectoryPath;
    replacementText += "\"";
    while (getline(fmargin, line)) {
        lines.push_back(line);
    }
    fmargin.close();

    // ファイルの10行目を変換する（0ベースのインデックスなので9行目）
    int lineNumberToReplace = 9;
    lines[lineNumberToReplace] = replacementText;

    // ファイルに変更内容を書き込む
    ofstream outputFile("margin.hpp");
    for (const string& updatedLine : lines) {
        outputFile << updatedLine << endl;
    }

    outputFile.close();
    fmargin.close();
    return 0;
}


