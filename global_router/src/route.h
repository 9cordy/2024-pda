#include<vector>
#include<list>
#include<queue>
#include<unordered_set>
#include<algorithm>
#include<random>
#include<cmath>

using namespace std;
typedef tuple<double,int,int> Node;

#ifndef ROUTE_H
#define ROUTE_H
    class Bump
    {
        public:
            int lower_left_x;
            int lower_left_y;
            Bump(int chip_x,int chip_y,int x,int y):lower_left_x(chip_x+x),lower_left_y(chip_y+y){}
    };
    class Chip
    {
        public:
            int lower_left_x;
            int lower_left_y;
            int width;
            int height;
            vector<Bump*> bumps;
            Chip(int ra_x,int ra_y,int x,int y,int w,int h):lower_left_x(ra_x+x),lower_left_y(ra_y+y),width(w),height(h){}
    };
    class G_cell
    {
        public:
            int lower_left_x;
            int lower_left_y;
            int left_capacity;
            int bottom_capacity;
            int current_left_capacity;
            int current_bottom_capacity;
            double metal1_cost;
            double metal2_cost;
            double cost;
            bool available_vertical;
            bool available_horizontal;
            G_cell* parent;
            G_cell(int x,int y,int lc,int bc):lower_left_x(x),lower_left_y(y),left_capacity(lc),bottom_capacity(bc),current_left_capacity(0),current_bottom_capacity(0),cost(100000000),available_vertical(1),available_horizontal(1),parent(nullptr){}
    };
    struct pair_hash
    {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };
    void route_bumps(ofstream& fout,int grid_w,int grid_h,double alpha,double gamma,double delta,double via_cost,Chip* &ra,Chip* &chip1,Chip* &chip2,vector<vector<G_cell*>> &cells);
    void get_path(pair<int,int> &bump1,pair<int,int> &bump2,list<G_cell*> &path,vector<vector<G_cell*>> &cells);
    void print_path(list<G_cell*> &path);
    void A_star_search(int grid_w,int grid_h,double alpha,double gamma,double delta,double via_cost,pair<int,int> &bump1,pair<int,int> &bump2,vector<vector<G_cell*>> &cells);
    bool current_routing_direction(pair<int,int> &current_pos,vector<vector<G_cell*>> &cells);
    void update_capacity(list<G_cell*> &path,vector<vector<G_cell*>> &cells);
    void initialize(priority_queue<Node,vector<Node>,greater<Node>> &open_list,unordered_set<pair<int,int>,pair_hash> &closed_list,vector<vector<G_cell*>> &cells);
#endif