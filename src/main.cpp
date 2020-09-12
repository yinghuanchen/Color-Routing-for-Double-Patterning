#include <cstring>
#include <iostream>
#include <fstream>
#include "Routing.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
using namespace std;

void help_message() {
    cout << "usage: ./color_route <pin_file> <net_file> <block_file> <output_file>" << '\n';
}

int main(int argc, char* argv[]){
    if(argc != 5) {
       help_message();
       return 0;
    }

    fstream fin1(argv[1]); //pin
    fstream fin2(argv[2]); //net
    fstream fin3(argv[3]); //block
    fstream fout;
    fout.open(argv[4],ios::out);

    int tmp,tmplayer;
    double tmpx, tmpy;

    vector<point> pin_record;
    point zero;
    pin_record.push_back(zero);
    while(fin1 >> tmp >> tmplayer >> tmpx >> tmpy){
        int tmpintx = int(tmpx*2);
        int tmpinty = int(tmpy*2);
        point tmppair(tmpintx,tmpinty,tmplayer);
        pin_record.push_back(tmppair);
    }


    vector<vector<int> > net_record;
    vector<bool> net_critical;
    vector<int> forzero;
    string tmps;
    while(getline(fin2,tmps)){
        if ((int)tmps[tmps.size()-1] == 13){
            net_critical.push_back(tmps[tmps.size()-2]=='Y');
            tmps.erase(tmps.end()-3,tmps.end());
        }
        else{
            net_critical.push_back(tmps[tmps.size()-1]=='Y');
            tmps.erase(tmps.end()-2,tmps.end());
        }
        stringstream ss(tmps);
        vector<int> tmpvec;
        while(ss >> tmp) tmpvec.push_back(tmp);
        net_record.push_back(tmpvec);
    }

    vector<vector<int> > block_record;
    while(fin3 >> tmpx >> tmpy){
        vector<int> tmpvec;
        int tmpintx = int(tmpx*2);
        int tmpinty = int(tmpy*2);
        tmpvec.push_back(tmpintx);
        tmpvec.push_back(tmpinty);
        fin3 >> tmpx >> tmpy;
        tmpintx = int(tmpx*2);
        tmpinty = int(tmpy*2);
        tmpvec.push_back(tmpintx);
        tmpvec.push_back(tmpinty);
        block_record.push_back(tmpvec);
    }

    fin1.close();
    fin2.close();
    fin3.close();

    vector<vector<int> > first_order_of_net = netordering(pin_record,net_record,net_critical);
    prepocessing(pin_record,net_record,block_record);
    vector<vector<wire> > routingresult = routing(pin_record,first_order_of_net);

    for(int i=0;i<routingresult.size();i++){
        fout << "Net " << i+1 << '\n';
        for(int j=0;j<routingresult[i].size();j++){
            wire tmp = routingresult[i][j];
            fout << "+ ";
            if (tmp.x1%2) fout << (int)tmp.x1/2 << ".5 ";
            else fout << (int)tmp.x1/2 << ".0 ";
            if (tmp.y1%2) fout << (int)tmp.y1/2 << ".5 ";
            else fout << (int)tmp.y1/2 << ".0 ";
            if (tmp.x2%2) fout << (int)tmp.x2/2 << ".5 ";
            else fout << (int)tmp.x2/2 << ".0 ";
            if (tmp.y2%2) fout << (int)tmp.y2/2 << ".5 ";
            else fout << (int)tmp.y2/2 << ".0 ";
            fout << tmp.layer << ' ' << tmp.color << '\n';
        }
    }

    fout.close();

    return 0;
}

