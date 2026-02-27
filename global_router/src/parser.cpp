#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<unordered_map>
#include"route.h"
#include"parser.h"

using namespace std;

void read_gmp(ifstream& fin,int &grid_w,int &grid_h,Chip* &ra,Chip* &chip1,Chip* &chip2)
{
    string temp,temp2;
    stringstream buffer;
    buffer<<fin.rdbuf();
    fin.close();
    int x,y,w,h,num;
    buffer>>temp>>x>>y>>w>>h;
    ra=new Chip(0,0,x,y,w,h);
    buffer>>temp>>grid_w>>grid_h;
    bool second=0;
    bool read_chip=0;
    bool read_bump=0;
    getline(buffer,temp);
    stringstream ss;
    while(getline(buffer,temp))
    {
        if(temp=="")
        {
            second=1;
            read_chip=0;
            read_bump=0;
            continue;
        }
        if(temp==".c")
        {
            read_chip=1;
            continue;
        }
        if(temp==".b")
        {
            read_bump=1;
            continue;
        }
        if(read_bump)
        {
            ss<<temp;
            ss>>num>>x>>y;
            if(!second)
            {
                Bump* bump=new Bump(chip1->lower_left_x,chip1->lower_left_y,x,y);
                chip1->bumps.push_back(bump);
            }                
            else
            {
                Bump* bump=new Bump(chip2->lower_left_x,chip2->lower_left_y,x,y);
                chip2->bumps.push_back(bump);
            }
        }
        else if(read_chip)
        {
            ss<<temp;
            ss>>x>>y>>w>>h;
            if(!second)
                chip1=new Chip(ra->lower_left_x,ra->lower_left_y,x,y,w,h);
            else                
                chip2=new Chip(ra->lower_left_x,ra->lower_left_y,x,y,w,h);    
        }            
        ss.clear();           
    }
}

void read_gcl(ifstream& fin,int w,int cell_w,int cell_h,Chip* &ra,vector<vector<G_cell*>> &cells)
{
    string temp;
    stringstream buffer;
    buffer<<fin.rdbuf();
    fin.close();
    int lc,bc;
    buffer>>temp;
    int count=1;
    vector<G_cell*> row_cells;
    int y=ra->lower_left_y+cell_h*(count/w);
    while(buffer>>lc>>bc)
    {
        int x=ra->lower_left_x+cell_w*((count-1)%w);
        G_cell* cell=new G_cell(x,y,lc,bc);
        row_cells.push_back(cell);
        if(count%w==0)
        {
            cells.push_back(row_cells);
            y+=cell_h;
            row_cells.clear();
        }
        count++;
    }
}

void read_cst(ifstream& fin,double &alpha,double &beta,double &gamma,double &delta,double &via_cost,vector<vector<G_cell*>> &cells)
{
    string temp;
    stringstream buffer;
    buffer<<fin.rdbuf();
    fin.close();
    buffer>>temp>>alpha>>temp>>beta>>temp>>gamma>>temp>>delta;
    buffer>>temp>>via_cost;
    buffer>>temp;
    for(vector<G_cell*> row_cells:cells)
    {
        for(G_cell* cell:row_cells)
            buffer>>cell->metal1_cost;            
    }
    buffer>>temp;
    for(vector<G_cell*> row_cells:cells)
    {
        for(G_cell* cell:row_cells)
            buffer>>cell->metal2_cost;            
    }
}

void write_file(ofstream& fout,int net_num,list<G_cell*> path)
{
    fout<<"n"<<net_num<<endl;
    G_cell* cell=path.front();
    path.pop_front();
    bool prev_dir,dir;//0: bottom 1:left
    if(path.front()->lower_left_x!=cell->lower_left_x)//vertical M2
        prev_dir=0;
    else//horizontal M1
        prev_dir=1;
    if(prev_dir==0)
        fout<<"via"<<endl<<"M2 ";
    else
        fout<<"M1 ";
    fout<<cell->lower_left_x<<" "<<cell->lower_left_y<<" ";   
    while(!path.empty())
    {
        cell=path.front();
        path.pop_front();
        if(path.empty())
        {
            fout<<cell->lower_left_x<<" "<<cell->lower_left_y<<endl;
            if(prev_dir==0)
                fout<<"via"<<endl;
            break;
        }
        if(path.front()->lower_left_x!=cell->lower_left_x)//vertical M2
            dir=0;
        else//horizontal M1
            dir=1;
        if(dir!=prev_dir)
        {
            fout<<cell->lower_left_x<<" "<<cell->lower_left_y<<endl<<"via"<<endl;
            if(dir==0)
                fout<<"M2 ";
            else
                fout<<"M1 ";
            fout<<cell->lower_left_x<<" "<<cell->lower_left_y<<" ";               
        }  
        prev_dir=dir;
    }
    fout<<".end"<<endl;
}

void print_gmp(int grid_w,int grid_h,Chip* ra,Chip* chip1,Chip* chip2)
{
    cout<<ra->lower_left_x<<" "<<ra->lower_left_y<<" "<<ra->width<<" "<<ra->height<<endl;
    cout<<grid_w<<" "<<grid_h<<endl;
    cout<<"chip 1: "<<chip1->lower_left_x<<" "<<chip1->lower_left_y<<" "<<chip1->width<<" "<<chip1->height<<endl;
    for(Bump* bump:chip1->bumps)
        cout<<bump->lower_left_x<<" "<<bump->lower_left_y<<endl;
    cout<<"chip 2: "<<chip2->lower_left_x<<" "<<chip2->lower_left_y<<" "<<chip2->width<<" "<<chip2->height<<endl;
    for(Bump* bump:chip2->bumps)
        cout<<bump->lower_left_x<<" "<<bump->lower_left_y<<endl;
}

void print_gcl(vector<vector<G_cell*>> &cells)
{
    for(vector<G_cell*> row_cells:cells)
    {
        for(G_cell* cell:row_cells)
            cout<<cell->lower_left_x<<" "<<cell->lower_left_y<<endl;
        cout<<endl;
    }
}

void print_cst(vector<vector<G_cell*>> &cells)
{
    for(vector<G_cell*> row_cells:cells)
    {
        for(G_cell* cell:row_cells)
            cout<<cell->metal1_cost<<" ";
        cout<<endl;
    }
    for(vector<G_cell*> row_cells:cells)
    {
        for(G_cell* cell:row_cells)
            cout<<cell->metal2_cost<<" ";
        cout<<endl;
    }
}