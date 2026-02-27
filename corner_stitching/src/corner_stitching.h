#include<vector>
#include<map>

using namespace std;

#ifndef CORNER_STITCHING_H
#define CORNER_STITCHING_H
    class Tile
    {
        public:
            int x;
            int y;
            int w;
            int h;
            bool type;//0: blank, 1: block
            int block_num;
            Tile* bl;
            Tile* lb;
            Tile* tr;
            Tile* rt;//b:bottom l:left t:top r:right
            Tile():bl(nullptr),lb(nullptr),tr(nullptr),rt(nullptr){}
            Tile(int x,int y,int w,int h,bool type):x(x),y(y),w(w),h(h),type(type),bl(nullptr),lb(nullptr),tr(nullptr),rt(nullptr){}
            Tile(int x,int y,int w,int h,bool type,int block_num):x(x),y(y),w(w),h(h),type(type),block_num(block_num),bl(nullptr),lb(nullptr),tr(nullptr),rt(nullptr){}
    };
    void check_all_pointers(int out_w,int out_h,Tile* block,vector<Tile*> &check_merge_tiles,vector<Tile*> &tiles);
    Tile* point_finding(int point_x,int point_y,int out_w,int out_h,vector<Tile*> &tiles);
    void block_creating(int out_w,int out_h,Tile* block,vector<Tile*> &tiles,vector<Tile*> &check_merge_tiles);
    void split_tile(int out_w,int out_h,Tile* tile,Tile* block,vector<Tile*> &tiles,vector<Tile*> &check_merge_tiles);
    void update_tile_pointers(Tile* new_tile,Tile* old_tile,Tile* block,vector<Tile*> &tiles);
    bool check_top_merge(Tile* current);
    void top_merge(Tile* current,vector<Tile*> &tiles);
    bool check_bottom_merge(Tile* current);
    void bottom_merge(Tile* current,vector<Tile*> &tiles);
    void final_merge(vector<Tile*> &tiles);
    void neighbor_finding(int out_w,int out_h,vector<Tile*> &tiles,map<int,int> &block_neighbor,map<int,int> &blank_neighbor);
    void count_neighbor(Tile* current,vector<Tile*> &count_block_neighbor,vector<Tile*> &count_blank_neighbor);
#endif