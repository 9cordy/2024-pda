#include<fstream>
#include<vector>
#include<queue>
#include<unordered_map>
#include<algorithm>
#include<random>
#include<cmath>
#include"legalizer.h"

using namespace std;

#ifndef PARSER_H
#define PARSER_H
    void read_file(ifstream& fin,double &alpha,double &beta,double &row_max_y,DieSize* &die,unordered_map<string,Cell*> &cells,unordered_map<double,Placement_row*> &placement_rows);
    void write_file(ofstream& fout,string name,vector<Cell*> &moved_cells,unordered_map<string,Cell*> &cells);
#endif