#include<iostream>
#include<fstream>
#include<vector>
#include"route.h"
#include"parser.h"
using namespace std;

int main(int argc,char **argv)
{
    ifstream fin;
    ofstream fout;
    int grid_w,grid_h;
    double alpha,beta,gamma,delta,via_cost;
    Chip *ra,*chip1,*chip2;
    vector<vector<G_cell*>> cells;
    fin.open(argv[1]);
    read_gmp(fin,grid_w,grid_h,ra,chip1,chip2);
    fin.open(argv[2]);
    read_gcl(fin,(ra->width/grid_w),grid_w,grid_h,ra,cells);
    fin.open(argv[3]);
    read_cst(fin,alpha,beta,gamma,delta,via_cost,cells);
    // print_gmp(grid_w,grid_h,ra,chip1,chip2);
    // print_gcl(cells);
    // char c;cin>>c;
    fout.open(argv[4]);
    route_bumps(fout,grid_w,grid_h,alpha,gamma,delta,via_cost,ra,chip1,chip2,cells);
    return 0;
}