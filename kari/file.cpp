#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <cstdio>
#include <regex>
#include <algorithm>
#include <filesystem>
#include <cmath>
#include <math.h>
#include <iomanip> 
#include "function.hpp"
#define PYTHON_EXE_PATH u8"Path to python" 
using namespace std;

/* 判定基準ファイルの読み込み */
int readJudgementFile(vector<vector<judge>> &jud, string &judgefilename){
    //FILE *fp = NULL;
    string line, ignore;
    
    vector<judge> elej_unit;

    stringstream dvwords;
    judgefilename += ".txt";
    int x = 0;
    int y = 0;
    int num_jud = 0; 
    int btime = 0, etime = 0, anti = 0;
    double phase = 0;

    /* ファイルのオープン */
    ifstream file(judgefilename);
    if (!file.is_open()) {
        cerr << "ERROR : Can't read JudgementFile" << endl;
        exit(1);
    }
            while ( getline(file,line) ) {
                    dvwords.clear();
                    dvwords.str("");
                    btime = 0;
                    etime = 0;
                    phase = 0;
                    anti = 0;
                    dvwords << line;
                    dvwords >> btime >> etime >> phase >> anti;
                    if(etime != 0){
                        jud[x - 1].push_back({btime, etime, phase, anti});
                        num_jud++;
                    }
                    if(line.find("B") != string::npos || line.find("b") != string::npos || line.find("e") != string::npos || line.find("E") != string::npos){
                        x++;
                        jud.push_back(elej_unit);     //vector ni youso wo tuika               
                    }
                }
    
    cout << " Number of Elements : " << x << endl;
    cout << " Total of Judgement : " << num_jud << endl;
    file.close();
    return 1;
}


double triple_digits(double num){
    num = num * 1000;
    num = lround(num);
    num = num / 1000;
    return num;
}



/* 素子判定 */
int judge_element(string &line){
    string ele,ele_bi;
    ele = line.substr(0,1);
    ele_bi = line.substr(0,2);
    switch(*ele.c_str()){
        case 'l' :  return 0;
        case 'k' :  return 1;
        case 'b' :  
        if(ele_bi == "bi" || ele_bi == "bI"){
                    return 3;  //JJインダクタンス
            }
            else{
                    return 2;  //通常のJJ
            }
        case 'c' :  return 4;
        case 'r' :  return 5;
        case 'v' :  return 6;
        case 'i' :  return 7;
        default  :  return -1;


    }
}

//circuitファイルの読み込み */
void make_data_cir(vector<string> &data_cir, string &filename, vector<ele_unit> &element){
    
    int flg_shunt_B = 0;
    int flg_shunt_P = 0;
    int line_num = 0;
    int i = 0;
    int i1 = 0, i2 = 0, i3 = 0;
    int file_check = -1;
    range range;

    string cirfilename = filename + ".cir";
    string inpfilename = filename + ".inp";

    filename = cirfilename;

    if(!filesystem::is_regular_file(cirfilename)){ //if there is [input].cir, change the name of filename to .inp
        cirfilename = inpfilename;
        filename = inpfilename;
    }

    ifstream file(cirfilename);
    string line, name, node1, node2, ignore, jjmod, shunt, main, sub, unit;
    double value;
    stringstream liness, mainss;

    if (!file.is_open()) {
        cerr << "Error opening file2: " << cirfilename << endl;
    }
    while( getline(file, line)) {
            liness.clear();
            liness.str("");
            mainss.clear();
            mainss.str("");
            sub = "";
            unit = "";
            data_cir.emplace_back(line); // insert line to data_cir

            if (flg_shunt_B == 1){
                if(line.find("RS") == 0 || line.find("Rs") == 0){
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    stringstream liness(line);
                    liness >> ignore >> ignore >> ignore >> ignore >> shunt >> value;

                    element[element.size() - 1].shunt_cal = value;  //shant_cal(shunt抵抗計算用)に値を格納

                    if(line.find("*SHUNT") != string::npos || line.find("*shunt") != string::npos){
                        element[element.size() - 1].shunt_det = 0;   //shunt_detの値を0(IcRsでシャント抵抗計算)に変更
                    }
                    else if(line.find("*Bc") != string::npos || line.find("*BC") != string::npos){
                        element[element.size() - 1].shunt_det = 1; //shunt_detの値を1(Bcでシャント抵抗計算)に変更
                    }
                    flg_shunt_B = 0;
                }
            }

            if(line.find("*MIN") == 0){
                double MIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  MIN;
                (element.back()).MIN = MIN;
            }
            else if(line.find("*MAX") == 0){
                double MAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  MAX;
                (element.back()).MAX = MAX;
            }
            else if(line.find("*FIX") == 0){
                (element.back()).FIX = 1;
            }
            else if(line.find("*SYN") == 0){
                double SYN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  SYN;
                (element.back()).SYN = SYN;
            }


            range = find_range(range, line);
            flg_shunt_B = 0;


            switch (judge_element(line)) {

                case 0:
                    liness << line;    // 文字列lineをstringstream liness に格納
                    liness >> name >> node1 >> node2 >> main; //linessをname(素子名), node1, node2, main(それ以降)に分割

                    i1 = main.find("h");    //単位の切り離し(H)
                    i2 = main.find("H");
                    if ( i1 != string::npos || i2 != string::npos){
                        i = max({i1,i2});
                        main.insert(i," ");     
                    }
                    
                    sub = sub_unit(main);
                    unit = sub + "H";
                    mainss << main;
                    mainss >> value;

                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({0, line_num, name, node1, node2, triple_digits(value), unit,0, 0, range.LMIN, range.LMAX, 0, 0, 0, 0, ""});
                    break;
                case 1:
                    liness << line;
                    liness >> name >> node1 >> node2 >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({1, line_num, name, node1, node2, triple_digits(value), "",0, 0, range.KMIN, range.KMAX, 0, 0, 0, 0, ""});
                    break;
                case 2:     // B
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    liness << line;                    //jjmod    area=
                    liness >> name >> node1 >> node2 >> jjmod >> ignore >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({2, line_num, name, node1, node2, triple_digits(value), "",0, 0, range.BMIN, range.BMAX, 0, 0, -1, 0, jjmod});
                    flg_shunt_B = 1;
                    break;
                case 3:
                    i1 = line.rfind("=");
                    if (i1 != string::npos){
                        line.insert(i1 + 1, " ");
                    }
                    liness << line;
                    liness >> name >> node1 >> node2 >> jjmod >> ignore >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({3, line_num, name, node1, node2, triple_digits(value), sub,0, 0, range.BIMIN, range.BIMAX, 0, 0, 0, 0, jjmod});
                    break;
                case 4:  //C
                    liness << line;
                    liness >> name >> node1 >> node2 >> main;

                    i1 = main.find("f");
                    i2 = main.find("F");
                    if ( i1 != string::npos || i2 != string::npos){
                        i = max({i1,i2});
                        main.insert(i," ");     
                    }

                    sub = sub_unit(main);
                    unit = sub + "F";
                    mainss << main;
                    mainss >> value;

                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({5, line_num, name, node1, node2, triple_digits(value), unit,0, 0, range.CMIN, range.CMAX, 0, 0, 0, 0, ""});
                    break;
                case 5:
                    i1 = line.rfind("ohm");    //Rの場合は、"ohm" がsub_unitのミリ(m)に引っかかるので、先に"ohm"を排除
                    i2 = line.rfind("OHM");
                    if ( i1 != string::npos || i2 != string::npos){
                        i = max({i1,i2});
                        line.insert(i," ");     
                    }

                    liness << line;
                    liness >> name >> node1 >> node2 >> main;

                    sub = sub_unit(main);
                    unit = sub + "ohm";
                    mainss << main;
                    mainss >> value;

                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({5, line_num, name, node1, node2, triple_digits(value), unit,0, 0, range.RMIN, range.RMAX, 0, 0, 0, 0, ""});
                    break;
                case 6:
                    liness << line;
                    liness >> name >> node1 >> node2 >> ignore >> ignore  >> ignore >> main;
                    i1 = main.rfind("v");
                    i2 = main.rfind("V");
                    if ( i1 != string::npos || i2 != string::npos){
                        i = max({i1,i2});
                        main.insert(i," ");     
                    }

                    sub = sub_unit(main);
                    unit = sub + "V";
                    mainss << main;
                    mainss >> value;

                    liness << line;
                    liness >> name >> node1 >> node2 >> ignore >> ignore  >> ignore >> value;
                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({6, line_num, name, node1, node2, triple_digits(value), unit,0, 0, range.IMIN, range.IMAX, 0, 0, 0, 0, ""});
                    break;
                case 7:
                    liness << line;
                    liness >> name >> node1 >> node2 >> ignore >> ignore  >> ignore >> main;

                    i1 = main.rfind("a");
                    i2 = main.rfind("A");
                    if ( i1 != string::npos || i2 != string::npos){
                        i = max({i1,i2});
                        main.insert(i," ");     
                    }

                    sub = sub_unit(main);
                    unit = sub + "A";
                    mainss << main;
                    mainss >> value;

                    transform(name.begin(), name.end(), name.begin(), ::toupper); 
                    element.push_back({7, line_num, name, node1, node2, triple_digits(value), unit,0, 0, range.VMIN, range.VMAX, 0, 0, 0, 0, ""});
                    break;
                    
                default:
                    break;

            }

        if(line.find(".FILE") == 0 || line.find(".file") == 0 || line.find(".File") == 0){// .FILE の行があったら、file_check == 1とする
            file_check = 1;
        }
        line_num++;
    }
    
    if(element.size() == 0){
        cout << " Error : No target element." << endl;
        return;
    }
    if(file_check == -1){ 
        cerr << "Please insert \".FILE\" line into your circuit file.";
        return;
    }

    file.close();

    int sort = 0;
    vector<ele_unit> element_order;  //整列されたelement配列
    vector<int> line_num_order;
    for(int ide_num = 0; ide_num <= 7; ide_num++){   //elementを整列
        for (int i = sort; i < element.size(); i++){
            if(element[i].ide_num == ide_num){
                swap(element[sort], element[i]);
                sort++;
            }
        }
    }

    /*
    vector<ele_unit> element_order;  //整列されたelement配列
    vector<int> line_num_order;
    for(int ide_num = 0; ide_num <= 7; ide_num++){   //elementを整列
        for (int i = 0; i < element.size(); i++){
            if(element[i].ide_num == ide_num){
                element_order.emplace_back(element[i]);
                line_num_order.emplace_back(element[i].line_num);
            }
        }
    }
    for(int i = 0; i < element.size(); i++){
            cout << element[i].element << " " << element[i].ide_num << " " << cou->line_num_arr[i] << endl ;
    }    
    element = element_order;
    cou->line_num_arr = line_num_order;

    for(int i = 0; i < element.size(); i++){
            cout << element[i].element << " " << element[i].ide_num << " " << cou->line_num_arr[i] << endl ;
        }    */
    }



void detail_out(vector<ele_unit> &element){

    stringstream out_det;
    string name;
    for(size_t i = 0; i < element.size(); i++){
        name = element[i].element;
        if(element[i].ide_num == 2 ||element[i].ide_num == 3){
            name.replace(0,1,"J");
        }

        out_det.clear();
        out_det.str("");
        out_det << setw(6) << right << name << " : " << fixed  << right << setw(6) << setprecision(3) << element[i].value  << " " << setw(5) << left <<  element[i].unit << " Lower: "  << setprecision(3) << setw(6) << element[i].value * (1 + element[i].margin_L / 100)  << "   Upper: "
                << setw(6) << setprecision(3)  << element[i].value * (1 + element[i].margin_H / 100) << "   Median: " << setw(6) << setprecision(3)  << element[i].value * (1 + (element[i].margin_L / 100 + element[i].margin_H / 100) / 2 ) << "  "  << setprecision(2) << setw(7) << element[i].margin_L << " % ~ "  << setprecision(2) << setw(6) << element[i].margin_H << " %";
        cout << out_det.str() << endl;
    }

}

void fig_out(vector<ele_unit> &element){
    int num_cri = find_critical(element);
    int num_bias = find_critical_bias(element);
    vector<string> hush(2);
    vector<double> hush_num{0, 0};
    stringstream out_fig;
    string name;
    cout << "\n" << "                  -50%                                                 50%\n";
    cout << "                    +---------------------------------------------------+\n";
    for(  int i = 0 ; i < element.size() ; i++ ){
        hush_num = {abs(element[i].margin_L), element[i].margin_H};
        hush = {"",""};
        for( int j = 0 ; j < 2 ; j++ ){
            if(hush_num[j] > 50){
                    hush_num[j] = 50; 
            }
            for ( int k = 0 ; k < (int)ceil(hush_num[j] / 2) ; k++){
                hush[j] += MARGIN_FIG;
                }
        }
        
        name = element[i].element;
        if(element[i].ide_num == 2 ||element[i].ide_num == 3){
            name.replace(0,1,"J");
        }
        out_fig.clear();
        out_fig.str("");
        out_fig << setw(6) << element[i].element << "  :  " << fixed  << right << setprecision(2) << setw(6) << -hush_num[0] << " % ["
                << setw(25) << hush[0] << "|" << left << setw(25) << hush[1] << "]" << right << setprecision(2) << setw(6) << hush_num[1] << " %";
        cout << out_fig.str() << endl;
    }
    cout << "                    +---------------------------------------------------+\n";
    cout << "                  -50%                                                 50%\n";
    cout << "    Critical Margin : " << fixed << setprecision(2) << min({-element[num_cri].margin_L, element[num_cri].margin_H}) << " %" << " ('" << element[num_cri].element << "') " << endl;
    cout << "    Bias Margin     : " << fixed << setprecision(2) << min({-element[num_bias].margin_L, element[num_bias].margin_H}) << " %" << " ('" << element[num_bias].element << "') " << endl;
    cout << endl;
}

void file_out(string &filename, vector<ele_unit> &element) {
    stringstream txtfilename, out_csv, out_det;
    txtfilename << "result_" << filename << ".txt";
    ofstream fpcsv("result.csv");
    ofstream fpout(txtfilename.str());
    string name;

    for (int i = 0; i < element.size(); i++) {
        name = element[i].element;
        if(element[i].ide_num == 2 ||element[i].ide_num == 3){
            name.replace(0,1,"J");
        }
        out_csv.clear();
        out_csv.str("");
        out_csv << setw(5) << name << "," << setw(6) << setprecision(3) << element[i].margin_L << "," << setw(6) << setprecision(3) << element[i].margin_H;
        fpcsv << out_csv.str() << endl;
    }

    for (int i = 0; i < element.size(); i++) {
        name = element[i].element;
        if(element[i].ide_num == 2 ||element[i].ide_num == 3){
            name.replace(0,1,"J");
        }
        out_det.clear();
        out_det.str("");
        out_det << setw(6) << right << name << " : " << fixed  << right << setw(6) << setprecision(3) << element[i].value  << " " << setw(5) << left <<  element[i].unit << " Lower: "  << setprecision(3) << setw(6) << element[i].value * (1 + element[i].margin_L / 100)  << "   Upper: "
                << setw(6) << setprecision(3)  << element[i].value * (1 + element[i].margin_H / 100) << "   Median: " << setw(6) << setprecision(3)  << element[i].value * (1 + (element[i].margin_L / 100 + element[i].margin_H / 100) / 2 ) << "  "  << setprecision(2) << setw(7) << element[i].margin_L << " % ~ "  << setprecision(2) << setw(6) << element[i].margin_H << " %";
        fpout << out_det.str() << endl;
    }

    fpcsv.close();
    fpout.close();
}

void yield_out(board* top, int sum, string &filename, vector<ele_unit> &element){

    
}

void synchro(vector<ele_unit> &element, int ele_num, double syn_value){
    if(element[ele_num].SYN != 0){
        double value = element[ele_num].value;
        for(size_t i = 0; i < element.size(); i++){
            if(element[i].SYN == element[ele_num].SYN){
                element[i].value = syn_value;
            }
        }
    }

}

void synchro_opt(vector<ele_unit> &element, int ele_num){
    if(element[ele_num].SYN != 0){
        double value = element[ele_num].value;
        for(size_t i = 0; i < element.size(); i++){
            if(element[i].SYN == element[ele_num].SYN){
                element[i].value = value;
            }
        }
    }

}


range find_range(range range, string line){
            string ignore;
            stringstream liness; 
            liness.clear();
            liness.str("");

            if(line.find("*LMIN") == 0){
                double LMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  LMIN;
                range.LMIN = LMIN;
            }
            else if(line.find("*LMAX") == 0){
                double LMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  LMAX;
                range.LMAX = LMAX;
            }
            else if(line.find("*KMIN") == 0){
                double KMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  KMIN;
                range.KMIN = KMIN;
            }
            else if(line.find("*KMAX") == 0){
                double KMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  KMAX;
                range.KMAX = KMAX;
            }            
            else if(line.find("*BMIN") == 0){
                double BMIN = 0.1;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BMIN;
                range.BMIN = BMIN;
            }
            else if(line.find("*BMAX") == 0){
                double BMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BMAX;
                range.BMAX = BMAX;
            }
            else if(line.find("*BIMIN") == 0){
                double BIMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BIMIN;
                range.BIMIN = BIMIN;
            }        
            else if(line.find("*BIMAX") == 0){
                double BIMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  BIMAX;
                range.BIMAX = BIMAX;
            }            
            else if(line.find("*CMIN") == 0){
                double CMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  CMIN;
                range.CMIN = CMIN;
            }        
            else if(line.find("*CMAX") == 0){
                double CMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  CMAX;
                range.CMAX = CMAX;
            }            
            else if(line.find("*RMIN") == 0){
                double RMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  RMIN;
                range.RMIN = RMIN;
            }
            else if(line.find("*RMAX") == 0){
                double RMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  RMAX;
                range.RMAX = RMAX;
            }
            else if(line.find("*VMIN") == 0){
                double VMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  VMIN;
                range.VMIN = VMIN;
            }
            else if(line.find("*VMAX") == 0){
                double VMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  VMAX;
                range.VMAX = VMAX;
            }
            else if(line.find("*IMIN") == 0){
                double IMIN;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  IMIN;
                range.IMIN = IMIN;
            }
            else if(line.find("*IMAX") == 0){
                double IMAX;
                line.insert(line.find("=") ,"  ");
                line.insert(line.find("=") + 1 ,"  ");
                liness << line;
                liness >> ignore >> ignore >>  IMAX;
                range.IMAX = IMAX;
            }

    return range;
}



string sub_unit(string& main){
    int find_sub1 = 0, find_sub2 = 0;
    int i = 0;
    find_sub1 = main.find("f");
    find_sub2 = main.find("F");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({find_sub1,find_sub2});
        main.insert(i," ");     
        return "f";
    }

    find_sub1 = main.find("p");
    find_sub2 = main.find("P");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({find_sub1,find_sub2});
        main.insert(i," ");     
        return "p";
    }

    find_sub1 = main.find("n");
    find_sub2 = main.find("N");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({find_sub1,find_sub2});
        main.insert(i," ");     
        return "n";
    }

    find_sub1 = main.find("u");
    find_sub2 = main.find("U");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({find_sub1,find_sub2});
        main.insert(i," ");     
        return "u";
    }

    find_sub1 = main.find("m");
    find_sub2 = main.find("M");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({find_sub1,find_sub2});
        main.insert(i," ");     
        return "m";
    }

    find_sub1 = main.find("k");
    find_sub2 = main.find("K");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({find_sub1,find_sub2});
        main.insert(i," ");     
        return "k";
    }

    find_sub1 = main.find("meg");
    find_sub2 = main.find("MEG");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({find_sub1,find_sub2});
        main.insert(i," ");     
        return "meg";
    }

    find_sub1 = main.find("x");
    find_sub2 = main.find("X");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({find_sub1,find_sub2});
        main.insert(i," ");     
        return "x";
    }

    find_sub1 = main.find("g");
    find_sub2 = main.find("G");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({find_sub1,find_sub2});
        main.insert(i," ");     
        return "g";
    }

    find_sub1 = main.find("t");
    find_sub2 = main.find("T");
    if(find_sub1 != string::npos || find_sub2 != string::npos){
        i = max({find_sub1,find_sub2});
        main.insert(i," ");     
        return "t";
    }

    return "";
}