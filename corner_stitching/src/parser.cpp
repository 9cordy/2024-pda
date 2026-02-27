#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<algorithm>
#include"parser.h"
#include"corner_stitching.h"

using namespace std;

void read_file(ifstream& fin,int *out_w,int *out_h,vector<Tile*> &tiles,vector<pair<int,int>> &point_in_tile)
{
    fin>>(*out_w)>>(*out_h);
    int x,y,w,h,block_num;
    Tile* ini=new Tile(0,0,(*out_w),(*out_h),0);//initial blank block
    tiles.push_back(ini);
    string s;
    stringstream ss;
    while(fin>>s)
    {
        if(s=="P")
        {
            fin>>x>>y;
            Tile* pos=point_finding(x,y,(*out_w),(*out_h),tiles);
            point_in_tile.push_back(make_pair(pos->x,pos->y));
        }
        else
        {
            ss.clear();
            ss<<s;
            ss>>block_num;
            fin>>x>>y>>w>>h;
            Tile* block=new Tile(x,y,w,h,1,block_num); 
            vector<Tile*> check_merge_tiles;
            block_creating((*out_w),(*out_h),block,tiles,check_merge_tiles);
            tiles.push_back(block);
            check_all_pointers((*out_w),(*out_h),block,check_merge_tiles,tiles);                
        }
    }
}



void write_file(ofstream& fout,vector<Tile*> &tiles,map<int,int> &block_neighbor,map<int,int> &blank_neighbor,vector<pair<int,int> > &point_in_tile)
{
    fout<<tiles.size()<<endl;
    map<int,int>::const_iterator iter;
    for(iter=block_neighbor.begin();iter!=block_neighbor.end();iter++)
    {
        int block_num=iter->first;
        fout<<block_num<<" "<<block_neighbor[block_num]<<" "<<blank_neighbor[block_num]<<endl;
    }    
    for(int i=0;i<point_in_tile.size();i++)
        fout<<point_in_tile[i].first<<" "<<point_in_tile[i].second<<endl;
    fout.close();
}