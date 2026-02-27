#include<fstream>
#include<vector>
#include<list>
#include<queue>
#include<unordered_map>
#include<algorithm>
#include<random>
#include<cmath>
#include"bstar_tree.h"

using namespace std;

#ifndef PARSER_H
#define PARSER_H
    void read_block(ifstream& fin,int &out_w,int &out_h,int &block_num,int &terminal_num,vector<Block*> &block_vec,vector<Block*> &terminal_vec,unordered_map<string,Block*> &block_map,unordered_map<string,Block*> &terminal_map);
    void read_net(ifstream& fin,int &net_num,vector<Net*> &net_vec);
    void write_file(ofstream& fout,int HPWL,double time,int cost,BS_tree* bst,vector<Block*> &block_vec);
#endif