#include<iostream>
#include<fstream>
#include<vector>
#include<unordered_map>
#include"parser.h"
#include"legalizer.h"

using namespace std;

int main(int argc, char **argv)
{
    ifstream fin;
    ofstream fout;
    fin.open(argv[1]);
    double alpha,beta,row_max_y;
    DieSize* die=new DieSize;
    // vector<Cell*> cells;
    // vector<Placement_row*> placement_rows;
    unordered_map<string,Cell*> cells;
    unordered_map<double,Placement_row*> placement_rows;
    read_file(fin,alpha,beta,row_max_y,die,cells,placement_rows);
    double row_height=placement_rows.begin()->second->h;
    row_max_y+=row_height;
    // fout.open(argv[3]);
    // for(auto it=cells.begin();it!=cells.end();it++)
    // {
    //     Cell* cell=it->second;
    //     fout<<cell->name<<" "<<cell->x<<" "<<cell->y<<" "<<cell->w<<" "<<cell->h<<" "<<cell->fix<<endl;

    // }
    // for(auto it=placement_rows.begin();it!=placement_rows.end();it++)
    // {
    //     Placement_row* row=it->second;
    //     fout<<row->x<<" "<<row->y<<" "<<row->w<<" "<<row->h<<endl;
    // }
    initial_placement(row_height,cells,placement_rows);    
    fin.open(argv[2]);
    fout.open(argv[3]);
    legalize_all(fin,fout,alpha,beta,row_max_y,row_height,cells,placement_rows);
    return 0;
}