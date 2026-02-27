#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include"parser.h"
#include"corner_stitching.h"

using namespace std;

int main(int argc,char *argv[])
{
    ifstream fin;
    ofstream fout;
    fin.open(argv[1]);
    int out_w,out_h;
    vector<Tile*> tiles;
    vector<pair<int,int> > point_in_tile;
    read_file(fin,&out_w,&out_h,tiles,point_in_tile);    
    final_merge(tiles);
    map<int,int> block_neighbor;
    map<int,int> blank_neighbor;
    neighbor_finding(out_w,out_h,tiles,block_neighbor,blank_neighbor);
    fout.open(argv[2]);
    write_file(fout,tiles,block_neighbor,blank_neighbor,point_in_tile);
    return 0;
}

