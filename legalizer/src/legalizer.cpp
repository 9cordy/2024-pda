#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<list>
#include<cfloat>
#include<queue>
#include<set>
#include<map>
#include<utility>
#include<unordered_map>
#include<algorithm>
#include<random>
#include<cmath>
#include"legalizer.h"
#include"parser.h"

using namespace std;

void initial_placement(double row_height,unordered_map<string,Cell*> &cells,unordered_map<double,Placement_row*> &placement_rows)
{
    for(auto it=cells.begin();it!=cells.end();)//iterate by cell
    {
        Cell* cell=it->second;
        cell->occupied_rows=static_cast<int>(cell->h/row_height);//multi_row or single_row
        
        if(cell->h>cell->occupied_rows*row_height)
            cell->occupied_rows++;
        int count_rows=cell->occupied_rows;
        bool placed=0;
        for(int i=0;i<count_rows;i++)
        {
            double y_position=cell->y+i*row_height;
            auto current_row=placement_rows.find(y_position);
            if(current_row==placement_rows.end())
                continue;
            Placement_row* row=current_row->second;
            double endpoint=cell->x+cell->w;
            double site_start_point=row->x;
            double site_number=row->occupied_sites.size();
            //for occupied_sites
            double start_index=cell->x-site_start_point;
            double end_index=endpoint-site_start_point;
            //indices within the window of occupied_sites
            if(start_index<0)
                start_index=0;
            if(end_index>site_number) 
                end_index=site_number;
            for (double j=start_index;j<end_index;j++) 
            {
                row->occupied_sites[static_cast<int>(j)]={1,cell->name};
                placed=1;
            }
        }        
        if(!placed)
            it=cells.erase(it);
        else 
        {
            cell->placed=placed;
            it++;
        }
    }
    for(auto it=placement_rows.begin();it!=placement_rows.end();it++)
    {
        bool in_interval=0;
        double start=0;
        Placement_row* row=it->second;
        for(size_t i=0;i<row->occupied_sites.size();i++)
        {
            if(!row->occupied_sites[i].first) 
            {
                if(!in_interval) 
                {
                    start=i;
                    in_interval=1;
                }
            }
            else 
            {
                if(in_interval) 
                {
                    row->empty_intervals.insert({start+row->x,(static_cast<double>(i-1)+row->x)});//first 0 last 0
                    in_interval=0;
                }
            }
        }
        if(in_interval)
            row->empty_intervals.insert({start+row->x,(static_cast<double>(row->occupied_sites.size()-1)+row->x)});
    }
}

void legalize_all(ifstream& fin,ofstream& fout,double alpha,double beta,double &row_max_y,double row_height,unordered_map<string,Cell*> &cells,unordered_map<double,Placement_row*> &placement_rows)
{
    string temp;
    stringstream buffer;
    buffer<<fin.rdbuf();
    fin.close();
    vector<string> banking_cells;
    while(buffer>>temp)
    {
        while(1)
        {
            buffer>>temp;
            if(temp=="-->")
                break;
            banking_cells.push_back(temp);
        }
        Cell target_cell;
        buffer>>target_cell.name>>target_cell.x>>target_cell.y>>target_cell.w>>target_cell.h;
        target_cell.initial_x=target_cell.x;
        target_cell.initial_y=target_cell.y;
        target_cell.occupied_rows=static_cast<int>(target_cell.h/row_height);
        if(target_cell.h>target_cell.occupied_rows*row_height)
            target_cell.occupied_rows++;
        cells[target_cell.name]=new Cell(target_cell);
        for(string cell_name:banking_cells)
        {            
            Cell* cell=cells[cell_name];
            remove_cell(cell,row_height,placement_rows);
            cells.erase(cell_name);
        }
        banking_cells.clear();
        set<pair<double,double>> new_intervals;
        vector<Cell*> moved_cells;
        find_insertion_point(row_max_y,row_height,cells[target_cell.name],moved_cells,cells,placement_rows,new_intervals);
        write_file(fout,target_cell.name,moved_cells,cells);
        moved_cells.clear();
    }
}

void insert_cell(Cell* &cell,double &row_height,unordered_map<double,Placement_row*> &placement_rows,set<pair<double,double>> &new_intervals)
{
    for(double i=0;i<cell->h;i+=row_height)
    {
        Placement_row* row=placement_rows[cell->y+i];
        if(row)
        {
            double j=cell->x-row->x;
            if(j<0)
                j=0;            
            for (auto it=row->empty_intervals.begin();it!=row->empty_intervals.end();)
            {
                double interval_start=it->first;
                double interval_end=it->second;
                if(interval_end<cell->x||interval_start>cell->x+cell->w) 
                    it++;
                else
                {
                    if (interval_start<cell->x)
                        new_intervals.insert({interval_start,cell->x-1});
                    if (interval_end>cell->x+cell->w)
                        new_intervals.insert({cell->x+cell->w,interval_end});
                    it=row->empty_intervals.erase(it);
                }
            }
            row->empty_intervals.insert(new_intervals.begin(),new_intervals.end()); 
            new_intervals.clear();
        }
    }        
}

void remove_cell(Cell* &cell,double &row_height,unordered_map<double,Placement_row*> &placement_rows)
{    
    for(double i=0;i<cell->h;i+=row_height)
    {
        Placement_row* row=placement_rows[cell->y+i];
        if(row)
        {
            double j=cell->x-row->x;
            if(j<0)
                j=0;
            double interval_start=cell->x;
            double interval_end=cell->x+cell->w-1;
            auto it=row->empty_intervals.lower_bound({interval_start,interval_start});
            if(it!=row->empty_intervals.begin()&&std::prev(it)->second==interval_start-1)
            {
                it--;
                interval_start=it->first;
                row->empty_intervals.erase(it);
            }
            it=row->empty_intervals.lower_bound({interval_end,interval_end});
            if(it!=row->empty_intervals.end()&&it->first==interval_end+1)
            {
                interval_end=it->second;                
                row->empty_intervals.erase(it);
            }
            row->empty_intervals.insert({interval_start,interval_end});
        }              
    }        
}

void find_insertion_point(double &row_max_y,double &row_height,Cell* &target_cell,vector<Cell*> &moved_cells,unordered_map<string,Cell*> &cells,unordered_map<double,Placement_row*> &placement_rows,set<pair<double,double>> &new_intervals) 
{    
    deque<pair<double,double>> distance_with_row;
    if(target_cell->y+target_cell->h>=row_max_y)
        target_cell->y-=(5*row_height);
    get_all_row_distance(row_height,target_cell,placement_rows,distance_with_row);
    bool found_insertion_point=0;
    double factor=1;
    while(!found_insertion_point)
    {
        if(distance_with_row.empty())
        {
            compact(row_height,moved_cells,cells,placement_rows,new_intervals);
            get_all_row_distance(row_height,target_cell,placement_rows,distance_with_row);
        }
        Placement_row* row=placement_rows[distance_with_row.front().second];
        if(target_cell->x<row->x)
            target_cell->x=row->x+10*target_cell->w;
        else if(target_cell->x+target_cell->w>row->x+row->sites)
            target_cell->x=row->x+row->sites-10*target_cell->w;        
        found_insertion_point=search_row(factor,row_height,target_cell,placement_rows,distance_with_row,new_intervals);
    }
}

void get_all_row_distance(double &row_height,Cell* &target_cell,unordered_map<double,Placement_row*> &placement_rows,deque<pair<double,double>> &distance_with_row)
{
    distance_with_row.clear();
    for(auto it=placement_rows.begin();it!=placement_rows.end();it++)
    {
        Placement_row* row=it->second;
        if(!row)
            continue;
        double manhattan_distance;
        manhattan_distance=abs(target_cell->y-row->y);
        distance_with_row.push_back({manhattan_distance,it->first});
    }
    sort(distance_with_row.begin(),distance_with_row.end());
}

bool search_row(double &factor,double &row_height,Cell* &target_cell,unordered_map<double,Placement_row*> &placement_rows,deque<pair<double,double>> &distance_with_row,set<pair<double,double>> &new_intervals)
{
    if(distance_with_row.empty())
        return 0;
    double current_row_y=distance_with_row.front().second;
    Placement_row* row=placement_rows[current_row_y];
    if(!row)
        return 0;
    distance_with_row.pop_front();
    double target_x=target_cell->x-row->x;
    int step=0;
    while(step<row->sites)
    {
        if(target_x+step<=row->sites)
        {
            int distance=target_x+step;
            double current_x=row->x+distance;
            double start_x=max(current_x,row->x);
            double end_x=min(current_x+target_cell->w,row->x+row->sites);
            double end_y=current_row_y+target_cell->h;
            double left_most_x=0;
            if(current_row_available(left_most_x,current_row_y,start_x,end_x,placement_rows))
            {
                if(other_rows_available(left_most_x,row_height,current_x,current_row_y,start_x,end_x,end_y,target_cell,placement_rows))
                {
                    target_cell->x=current_x;
                    target_cell->y=current_row_y;
                    insert_cell(target_cell,row_height,placement_rows,new_intervals);
                    return 1;                    
                }
            } 
        }
        if(step>0&&target_x-step>=0)
        {
            int distance=target_x-step;
            double current_x=row->x+distance;
            double start_x=max(current_x,row->x);
            double end_x=min(current_x+target_cell->w,row->x+row->sites);
            double end_y=current_row_y+target_cell->h;
            double left_most_x=0;  
            if(current_row_available(left_most_x,current_row_y,start_x,end_x,placement_rows))
            {                
                if(other_rows_available(left_most_x,row_height,current_x,current_row_y,start_x,end_x,end_y,target_cell,placement_rows))
                {
                    target_cell->x=current_x;
                    target_cell->y=current_row_y;
                    insert_cell(target_cell,row_height,placement_rows,new_intervals);
                    return 1;                    
                }
            }          
        }
        step+=200;
    }
    return 0;
}

bool current_row_available(double &left_most_x,double current_row_y,double start_x,double end_x,unordered_map<double,Placement_row*> &placement_rows)
{
    Placement_row* row=placement_rows[current_row_y];
    if(!row)
        return 0;
    auto it=row->empty_intervals.lower_bound({start_x,start_x});//{{1,5},{10,15},{20,25}} if start=11 get {10,15}
    if(it==row->empty_intervals.end())
    {
        it--;
        if(start_x>=it->first&&end_x<=it->second)
        {
            left_most_x=max(it->first,left_most_x);            
            return 1;
        }
        return 0;
    }
    if(it==row->empty_intervals.begin())
    {
        if(start_x<it->first)
            return 0;
    }
    else if(start_x<it->first)
        it--;
    if(start_x>=it->first&&end_x<=it->second)
    {
        left_most_x=max(it->first,left_most_x);
        return 1;
    }
    return 0;
}

bool other_rows_available(double &left_most_x,double row_height,double current_x,double current_row_y,double start_x,double end_x,double end_y,Cell* &target_cell,unordered_map<double,Placement_row*> &placement_rows)
{    
    for(int j=1;j<=placement_rows.size();j++)
    {
        double other_row_y=current_row_y+j*row_height;
        Placement_row* other_row=placement_rows[other_row_y];
        if(!other_row)
            return 0;
        if(other_row_y>=end_y)
            return 1;
        else
        {
            double other_start_x=max(current_x,other_row->x);
            double other_end_x=min(current_x+target_cell->w,other_row->x+other_row->sites);
            if(other_end_x-other_start_x<target_cell->w)
                return 0;
            if(current_row_available(left_most_x,other_row_y,other_start_x,other_end_x,placement_rows))
                continue;
            else
                return 0;
        }
    }
}

void compact(double &row_height,vector<Cell*> &moved_cells,unordered_map<string,Cell*> &cells,unordered_map<double,Placement_row*> &placement_rows,set<pair<double,double>> &new_intervals)
{
    vector<pair<double,Cell*>> compact_cells;
    for(auto it=cells.begin();it!=cells.end();it++)
    {
        Cell* cell=it->second;
        if((cell->occupied_rows==1)&&(!cell->fixed))
            compact_cells.push_back({cell->x,cell});
    }
    sort(compact_cells.begin(),compact_cells.end());
    size_t half_size=compact_cells.size()/3;
    for(auto it=compact_cells.begin();it!=compact_cells.begin()+half_size;it++)
    {
        Cell* cell=it->second;
        remove_cell(cell,row_height,placement_rows);
        Placement_row* row=placement_rows[cell->y];
        double target_x=0;
        int step=0;
        while(step<row->sites)
        {
            if(target_x+step<=row->sites)
            {
                int distance=target_x+step;
                double current_x=row->x+distance;
                double start_x=max(current_x,row->x);
                double end_x=min(current_x+cell->w,row->x+row->sites);
                double left_most_x=0;
                if(current_row_available(left_most_x,cell->y,start_x,end_x,placement_rows))
                {                    
                    cell->x=left_most_x;
                    insert_cell(cell,row_height,placement_rows,new_intervals);
                    moved_cells.push_back(cell);
                    break;
                } 
            }
            step+=1000;
        }
    }
}

void print_empty_intervals(unordered_map<double,Placement_row*> &placement_rows)
{
    for(auto it=placement_rows.begin();it!=placement_rows.end();it++)
    {
        cout<<it->first<<": "<<endl;
        for(auto it2:it->second->empty_intervals)
            cout<<static_cast<long long>(it2.first)<<" "<<static_cast<long long>(it2.second)<<endl;
        cout<<endl;
    }
}

void print_all_row_distance(deque<pair<double,double>> &distance_with_row)
{
    for(auto it=distance_with_row.begin();it!=distance_with_row.end();it++)
        cout<<it->second<<" distance: "<<it->first<<endl;
}