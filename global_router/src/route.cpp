#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<unordered_set>
#include<algorithm>
#include<random>
#include<cmath>
#include"route.h"
#include"parser.h"

using namespace std;
typedef tuple<double,int,int> Node;



void route_bumps(ofstream& fout,int grid_w,int grid_h,double alpha,double gamma,double delta,double via_cost,Chip* &ra,Chip* &chip1,Chip* &chip2,vector<vector<G_cell*>> &cells)
{
    int total_bumps=chip1->bumps.size();
    for(int i=0;i<total_bumps;i++)
    {
        //find bump position in G_cell
        pair<int,int> bump1,bump2;
        int x,y;
        x=chip1->bumps[i]->lower_left_x;
        y=chip1->bumps[i]->lower_left_y;
        bump1={(y-ra->lower_left_y)/grid_h,(x-ra->lower_left_x)/grid_w};//cells[bump1.first][bump1.second]
        x=chip2->bumps[i]->lower_left_x;
        y=chip2->bumps[i]->lower_left_y;
        bump2={(y-ra->lower_left_y)/grid_h,(x-ra->lower_left_x)/grid_w};
        A_star_search(grid_w,grid_h,alpha,gamma,delta,via_cost,bump1,bump2,cells);
        list<G_cell*> path;
        // char c;cin>>c;
        get_path(bump1,bump2,path,cells);
        // print_path(path);
        write_file(fout,i+1,path);
        update_capacity(path,cells);
        path.clear();
    }    
}

void get_path(pair<int,int> &bump1,pair<int,int> &bump2,list<G_cell*> &path,vector<vector<G_cell*>> &cells)
{    
    G_cell* cell=cells[bump2.first][bump2.second];
    G_cell* start_cell=cells[bump1.first][bump1.second];
    while((cell->lower_left_x!=start_cell->lower_left_x)||(cell->lower_left_y!=start_cell->lower_left_y))
    {
        path.push_front(cell);
        cell=cell->parent;
    }
    path.push_front(cell);
}

void print_path(list<G_cell*> &path)
{
    for(G_cell* cell:path)
        cout<<cell->lower_left_x<<" "<<cell->lower_left_y<<" "<<cell->cost<<endl;
}

void A_star_search(int grid_w,int grid_h,double alpha,double gamma,double delta,double via_cost,pair<int,int> &bump1,pair<int,int> &bump2,vector<vector<G_cell*>> &cells)//ignore overflow cost
{
    priority_queue<Node,vector<Node>,greater<Node>> open_list;
    unordered_set<pair<int,int>,pair_hash> closed_list;
    // list<pair<int,int>> closed_list;
    cells[bump1.first][bump1.second]->cost=cells[bump1.first][bump1.second]->metal1_cost+abs(cells[bump1.first][bump1.second]->lower_left_x-cells[bump2.first][bump2.second]->lower_left_x)+abs(cells[bump1.first][bump1.second]->lower_left_y-cells[bump2.first][bump2.second]->lower_left_y);
    open_list.emplace(cells[bump1.first][bump1.second]->cost,bump1.first,bump1.second);                                 
    int width_boundary=cells[0].size();
    int height_boundary=cells.size();
    int x,y;
    while(!open_list.empty())
    { 
        auto top=open_list.top();
        open_list.pop();
        x=get<2>(top);
        y=get<1>(top);
        pair<int,int> current_pos={y,x};
        double cost,old_cost_h,new_cost_h;
        double current_cost=get<0>(top);
        old_cost_h=abs(cells[current_pos.first][current_pos.second]->lower_left_x-cells[bump2.first][bump2.second]->lower_left_x)+abs(cells[current_pos.first][current_pos.second]->lower_left_y-cells[bump2.first][bump2.second]->lower_left_y);
        bool layer1=current_routing_direction(current_pos,cells);
        //up
        if(current_pos.first+1<height_boundary)
        {
            x=current_pos.second;
            y=current_pos.first+1;
            if(make_pair(y,x)==bump2)
            {
                cells[y][x]->parent=cells[current_pos.first][current_pos.second];
                break;
            }
            if(cells[y][x]->available_vertical)
            {
                cost=current_cost-alpha*old_cost_h;
                if(!layer1)
                {
                    double temp=gamma*(cells[current_pos.first][current_pos.second]->metal1_cost/2-cells[current_pos.first][current_pos.second]->metal2_cost/2)+delta*via_cost;
                    cost+=temp;
                }
                new_cost_h=abs(cells[y][x]->lower_left_x-cells[bump2.first][bump2.second]->lower_left_x)+abs(cells[y][x]->lower_left_y-cells[bump2.first][bump2.second]->lower_left_y);        
                cost+=(gamma*cells[y][x]->metal1_cost+alpha*grid_h+alpha*new_cost_h);
                if(cost<cells[y][x]->cost)
                {
                    cells[y][x]->cost=cost;
                    cells[y][x]->parent=cells[current_pos.first][current_pos.second];
                    auto it=closed_list.find({y,x});
                    if(it!=closed_list.end())
                        closed_list.erase({y,x});
                    open_list.emplace(cost,y,x);
                }
            }
        }
        //right
        if(current_pos.second+1<width_boundary)
        {
            x=current_pos.second+1;
            y=current_pos.first;
            if(make_pair(y,x)==bump2)
            {
                cells[y][x]->parent=cells[current_pos.first][current_pos.second];
                break;
            }
            if(cells[y][x]->available_horizontal)
            {
                cost=current_cost-alpha*old_cost_h;
                if(layer1)
                {
                    double temp=gamma*(cells[current_pos.first][current_pos.second]->metal2_cost/2-cells[current_pos.first][current_pos.second]->metal1_cost/2)+delta*via_cost;
                    cost+=temp;
                }
                new_cost_h=abs(cells[y][x]->lower_left_x-cells[bump2.first][bump2.second]->lower_left_x)+abs(cells[y][x]->lower_left_y-cells[bump2.first][bump2.second]->lower_left_y);        
                cost+=(gamma*cells[y][x]->metal2_cost+alpha*grid_w+alpha*new_cost_h);
                if(cost<cells[y][x]->cost)
                {
                    cells[y][x]->cost=cost;
                    cells[y][x]->parent=cells[current_pos.first][current_pos.second];
                    auto it=closed_list.find({y,x});
                    if(it!=closed_list.end())
                        closed_list.erase({y,x});
                    open_list.emplace(cost,y,x);
                }
            }
        }
        //bottom
        if(current_pos.first-1>=0)
        {
            x=current_pos.second;
            y=current_pos.first-1;
            if(make_pair(y,x)==bump2)
            {
                cells[y][x]->parent=cells[current_pos.first][current_pos.second];
                break;
            }
            if(cells[y+1][x]->available_vertical)
            {
                cost=current_cost-alpha*old_cost_h;
                if(!layer1)
                {
                    double temp=gamma*(cells[current_pos.first][current_pos.second]->metal1_cost/2-cells[current_pos.first][current_pos.second]->metal2_cost/2)+delta*via_cost;
                    cost+=temp;
                }
                new_cost_h=abs(cells[y][x]->lower_left_x-cells[bump2.first][bump2.second]->lower_left_x)+abs(cells[y][x]->lower_left_y-cells[bump2.first][bump2.second]->lower_left_y);        
                cost+=(gamma*cells[y][x]->metal1_cost+alpha*grid_h+alpha*new_cost_h);
                if(cost<cells[y][x]->cost)
                {
                    cells[y][x]->cost=cost;
                    cells[y][x]->parent=cells[current_pos.first][current_pos.second];
                    auto it=closed_list.find({y,x});
                    if(it!=closed_list.end())
                        closed_list.erase({y,x});
                    open_list.emplace(cost,y,x);
                }
            }
        }
        //left
        if(current_pos.second-1>=0)
        {
            x=current_pos.second-1;
            y=current_pos.first;
            if(make_pair(y,x)==bump2)
            {
                cells[y][x]->parent=cells[current_pos.first][current_pos.second];
                break;
            }
            if(cells[y][x+1]->available_horizontal)
            {
                cost=current_cost-alpha*old_cost_h;
                if(layer1)
                {
                    double temp=gamma*(cells[current_pos.first][current_pos.second]->metal2_cost/2-cells[current_pos.first][current_pos.second]->metal1_cost/2)+delta*via_cost;
                    cost+=temp;
                }
                new_cost_h=abs(cells[y][x]->lower_left_x-cells[bump2.first][bump2.second]->lower_left_x)+abs(cells[y][x]->lower_left_y-cells[bump2.first][bump2.second]->lower_left_y);        
                cost+=(gamma*cells[y][x]->metal2_cost+alpha*grid_w+alpha*new_cost_h);
                if(cost<cells[y][x]->cost)
                {
                    cells[y][x]->cost=cost;
                    cells[y][x]->parent=cells[current_pos.first][current_pos.second];
                    auto it=closed_list.find({y,x});
                    if(it!=closed_list.end())
                        closed_list.erase({y,x});
                    open_list.emplace(cost,y,x);
                }
            }
        }
        closed_list.insert(current_pos);
    }
    initialize(open_list,closed_list,cells);
    closed_list.clear();
}

bool current_routing_direction(pair<int,int> &current_pos,vector<vector<G_cell*>> &cells)
{
    if(cells[current_pos.first][current_pos.second]->parent==nullptr)
        return 1;
    else
    {
        if(cells[current_pos.first][current_pos.second]->parent->lower_left_x==cells[current_pos.first][current_pos.second]->lower_left_x)
            return 1;
        else
            return 0;
    }
}

void update_capacity(list<G_cell*> &path,vector<vector<G_cell*>> &cells)
{
    G_cell* prev_cell=path.front();
    path.pop_front();
    while(!path.empty())
    {
        G_cell* cell=path.front();
        path.pop_front();
        if(prev_cell->lower_left_x>cell->lower_left_x)//left
        {
            cell->current_left_capacity++;
            if(cell->current_left_capacity>=cell->left_capacity)
                cell->available_horizontal=0;
        }
        else if(prev_cell->lower_left_y>cell->lower_left_y)//up
        {
            cell->current_bottom_capacity++;
            if(cell->current_bottom_capacity>=cell->bottom_capacity)
                cell->available_vertical=0;
        }
        else if(prev_cell->lower_left_x<cell->lower_left_x)//right
        {
            prev_cell->current_left_capacity++;
            if(prev_cell->current_left_capacity>=prev_cell->left_capacity)
                prev_cell->available_horizontal=0;
        }
        else if(prev_cell->lower_left_y<cell->lower_left_y)//down
        {
            prev_cell->current_bottom_capacity++;
            if(prev_cell->current_bottom_capacity>=prev_cell->bottom_capacity)
                prev_cell->available_vertical=0;
        }
        prev_cell=cell;
    }
}

void initialize(priority_queue<Node,vector<Node>,greater<Node>> &open_list,unordered_set<pair<int,int>,pair_hash> &closed_list,vector<vector<G_cell*>> &cells)
{
    int x,y;
    while(!open_list.empty())
    { 
        auto top=open_list.top();
        open_list.pop();
        x=get<2>(top);
        y=get<1>(top);
        cells[y][x]->cost=100000000;
    }
    for(const auto& position:closed_list)
        cells[position.first][position.second]->cost=100000000;
}
