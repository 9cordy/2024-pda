#include<vector>
#include<list>
#include<queue>
#include<set>
#include<map>
#include<unordered_map>
#include<algorithm>
#include<random>
#include<cmath>

using namespace std;

#ifndef LEGALIZATION_H
#define LEGALIZATION_H
    class DieSize
    {
        public:
            double lower_left_x;
            double lower_left_y;
            double upper_right_x;
            double upper_right_y;
    };
    class Cell
    {
        public:
            string name;
            double initial_x;
            double initial_y;
            double x;//lower left
            double y;//lower left
            double w;
            double h;
            int occupied_rows;
            bool fixed;
            bool placed;
            Cell():placed(0){}
    };
    class Placement_row
    {
        public:
            double x;
            double y;
            double w;
            double h;
            double sites;
            vector<pair<bool,string>> occupied_sites;
            set<pair<double,double>> empty_intervals;//{1, 0, 0, 0, 1}=>{2,4}            
    };
    void initial_placement(double row_height,unordered_map<string,Cell*> &cells,unordered_map<double,Placement_row*> &placement_rows);
    void legalize_all(ifstream& fin,ofstream& fout,double alpha,double beta,double &row_max_y,double row_height,unordered_map<string,Cell*> &cells,unordered_map<double,Placement_row*> &placement_rows);
    void insert_cell(Cell* &cell,double &row_height,unordered_map<double,Placement_row*> &placement_rows,set<pair<double,double>> &new_intervals);
    void remove_cell(Cell* &cell,double &row_height,unordered_map<double,Placement_row*> &placement_rows);
    void find_insertion_point(double &row_max_y,double &row_height,Cell* &target_cell,vector<Cell*> &moved_cells,unordered_map<string,Cell*> &cells,unordered_map<double,Placement_row*> &placement_rows,set<pair<double,double>> &new_intervals);
    bool search_row(double &factor,double &row_height,Cell* &target_cell,unordered_map<double,Placement_row*> &placement_rows,deque<pair<double,double>> &distance_with_row,set<pair<double,double>> &new_intervals);
    void get_all_row_distance(double &row_height,Cell* &target_cell,unordered_map<double,Placement_row*> &placement_rows,deque<pair<double,double>> &distance_with_row);
    bool current_row_available(double &left_most_x,double current_row_y,double start_x,double end_x,unordered_map<double,Placement_row*> &placement_rows);
    bool other_rows_available(double &left_most_x,double row_height,double current_x,double current_row_y,double start_x,double end_x,double end_y,Cell* &target_cell,unordered_map<double,Placement_row*> &placement_rows);
    void compact(double &row_height,vector<Cell*> &moved_cells,unordered_map<string,Cell*> &cells,unordered_map<double,Placement_row*> &placement_rows,set<pair<double,double>> &new_intervals);

    void print_empty_intervals(unordered_map<double,Placement_row*> &placement_rows);
    void print_all_row_distance(deque<pair<double,double>> &distance_with_row);
#endif