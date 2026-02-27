#include<vector>
#include<map>
#include"corner_stitching.h"

using namespace std;

#ifndef PARSER_H
#define PARSER_H
    void read_file(ifstream& fin,int *out_w,int *out_h,vector<Tile*> &tiles,vector<pair<int,int>> &point_in_tile);
    void write_file(ofstream& fout,vector<Tile*> &tiles,map<int,int> &block_neighbor,map<int,int> &blank_neighbor,vector<pair<int,int> > &point_in_tile);
#endif