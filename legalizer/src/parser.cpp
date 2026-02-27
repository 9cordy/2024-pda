#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<cfloat>
#include"parser.h"
#include"legalizer.h"

using namespace std;


void read_file(ifstream& fin,double &alpha,double &beta,double &row_max_y,DieSize* &die,unordered_map<string,Cell*> &cells,unordered_map<double,Placement_row*> &placement_rows)
{
    string temp;
    stringstream buffer;
    buffer<<fin.rdbuf();
    fin.close();
    buffer>>temp>>alpha>>temp>>beta;
    row_max_y=0;
    buffer>>temp>>die->lower_left_x>>die->lower_left_y>>die->upper_right_x>>die->upper_right_y;
    vector<Cell*> temp_cells;
    vector<Placement_row*> temp_rows;
    temp_cells.reserve(150000);
    temp_rows.reserve(1000);
    while(buffer>>temp)
    {
        if(temp!="PlacementRows")
        {
            Cell* cell=new Cell;
            cell->name=move(temp);
            buffer>>cell->x>>cell->y>>cell->w>>cell->h;
            cell->initial_x=cell->x;
            cell->initial_y=cell->y;
            string temp2;
            buffer>>temp2;
            cell->fixed=(temp2=="FIX");
            temp_cells.push_back(cell);
        }
        else 
        {
            Placement_row* row=new Placement_row;
            buffer>>row->x>>row->y>>row->w>>row->h>>row->sites;
            // cout<<row->x<<" "<<row->y<<" "<<row->w<<" "<<row->h<<" "<<row->sites<<endl;
            row_max_y=max(row->y,row_max_y);
            double site=row->sites;
            row->occupied_sites.reserve(site);
            for(double i=0;i<=site;i++)
                row->occupied_sites.push_back({0,""});
            temp_rows.push_back(row);
        }
    }
    for (auto& cell:temp_cells)
        cells[cell->name]=move(cell);//use move instead of assign to preserve time
    for (auto& row:temp_rows)
        placement_rows[row->y]=move(row);
}

void write_file(ofstream& fout,string name,vector<Cell*> &moved_cells,unordered_map<string,Cell*> &cells)
{
    fout<<cells[name]->x<<" "<<cells[name]->y<<endl;
    fout<<moved_cells.size()<<endl;
    for(Cell* cell:moved_cells)
        fout<<cell->name<<" "<<cell->x<<" "<<cell->y<<endl;
}