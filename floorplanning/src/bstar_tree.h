#include<vector>
#include<list>
#include<queue>
#include<unordered_map>
#include<algorithm>
#include<random>
#include<cmath>

using namespace std;

#ifndef BSTAR_TREE_H
#define BSTAR_TREE_H
    class Block
    {
        public:
            string name;
            int x;
            int y;
            int w;
            int h;
            bool terminal;
            bool fixed;
            Block* parent;
            Block* left_child;
            Block* right_child;
            Block():x(-1),y(-1),w(0),h(0),terminal(0),fixed(0),parent(nullptr),left_child(nullptr),right_child(nullptr){}
    };
    class BS_tree
    {
        public:
            int x_max;
            int y_max;
            int n;
            Block* root;
            BS_tree():x_max(0),y_max(0),n(0),root(nullptr){}
    };
    class Contour
    {
        public:
            int x;
            int x_right;
            int y;
            Contour(int a,int b,int c):x(a),x_right(b),y(c){}
    };
    class Net
    {
        public:
            int degree;
            vector<string> block_names;
            Net(int n):degree(n){}
    };
    void build_tree(BS_tree* &bst,vector<Block*> &block_vec);
    void ini(BS_tree* &bst,list<Contour> &contour_list);
    int update_contour(BS_tree* &bst,Block* current,list<Contour> &contour_list);
    void update_coor(Block* current,BS_tree* &bst,list<Contour> &contour_list);
    void block_rotate(Block* current);
    bool block_move(Block* current,Block* target,bool move_left,int child_pos);
    void block_swap(Block* current,Block* target,unordered_map<string,Block*> &block_map);
    double area_cost(int out_w,int out_h,double alpha,int HPWL,BS_tree* &bst);
    double cost_calculation(double alpha,int &HPWL,BS_tree* &bst,vector<Net*> &net_vec,unordered_map<string,Block*> &block_map,unordered_map<string,Block*> &terminal_map);
    double accept_prob(double current_cost,double new_cost,int temperature);
    void simulate_annealing(int out_w,int out_h,int block_num,double alpha,int &HPWL,BS_tree* &bst,vector<Block*> &block_vec,vector<Block*> &terminal_vec,vector<Net*> &net_vec,list<Contour> &contour_list,unordered_map<string,Block*> &block_map,unordered_map<string,Block*> &terminal_map);
#endif