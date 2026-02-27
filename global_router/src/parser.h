#include<fstream>
#include<vector>
#include<queue>
#include<unordered_map>
#include<algorithm>
#include<random>
#include<cmath>
#include"route.h"

using namespace std;

#ifndef PARSER_H
#define PARSER_H
    void read_gmp(ifstream& fin,int &grid_w,int &grid_h,Chip* &ra,Chip* &chip1,Chip* &chip2);
    void read_gcl(ifstream& fin,int w,int cell_w,int cell_h,Chip* &ra,vector<vector<G_cell*>> &cells);
    void read_cst(ifstream& fin,double &alpha,double &beta,double &gamma,double &delta,double &via_cost,vector<vector<G_cell*>> &cells);
    void write_file(ofstream& fout,int net_num,list<G_cell*> path);
    void print_gmp(int grid_w,int grid_h,Chip* ra,Chip* chip1,Chip* chip2);
    void print_gcl(vector<vector<G_cell*>> &cells);
#endif