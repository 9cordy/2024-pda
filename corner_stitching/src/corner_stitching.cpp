#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<algorithm>
#include"corner_stitching.h"

using namespace std;

void check_all_pointers(int out_w,int out_h,Tile* block,vector<Tile*> &check_merge_tiles,vector<Tile*> &tiles)
{
    vector<Tile*> check_new_pointer=check_merge_tiles;
    for(Tile* tile:check_new_pointer)
    {
        if((tile->x==block->x+block->w)&&tile->y!=0)//(tile->lb==nullptr)
        {
            for(Tile* pointer:check_merge_tiles)
            {
                if((tile->x>=pointer->x)&&(tile->x<pointer->x+pointer->w)&&(tile->y==pointer->y+pointer->h))
                {
                    tile->lb=pointer;
                    break;
                }
            }
            if(tile->lb==nullptr)
            {
                for(Tile* pointer:tiles)
                {
                    if((tile->x>=pointer->x)&&(tile->x<pointer->x+pointer->w)&&(tile->y==pointer->y+pointer->h))
                    {
                        tile->lb=pointer;
                        break;
                    }
                }
            }
        }
        if((tile->x+tile->w==block->x)&&(tile->y+tile->h!=out_h))
        {
            for(Tile* pointer:check_merge_tiles)
            {
                if((tile->x+tile->w>pointer->x)&&(tile->x+tile->w<=pointer->x+pointer->w)&&(tile->y+tile->h==pointer->y))
                {
                    tile->rt=pointer;
                    break;
                }
            }
            if(tile->rt==nullptr)
            {
                for(Tile* pointer:tiles)
                {
                    if((tile->x+tile->w>pointer->x)&&(tile->x+tile->w<=pointer->x+pointer->w)&&(tile->y+tile->h==pointer->y))
                    {
                        tile->rt=pointer;
                        break;
                    }
                }
            }
        }
    }
    if((block->x>0)&&(block->bl==nullptr))
    {
        for(Tile* tile:tiles)
        {
            if((block->x==tile->x+tile->w)&&(block->y>=tile->y)&&(block->y<tile->y+tile->h))
            {
                block->bl=tile;
                break;
            }
        }
    }
    if((block->y>0)&&(block->lb==nullptr))
    {
        for(Tile* tile:tiles)
        {
            if((block->x>=tile->x)&&(block->x<tile->x+tile->w)&&(block->y==tile->y+tile->h))
            {
                block->lb=tile;
                break;
            }
        }
    }
    if((block->x+block->w<out_w)&&(block->tr==nullptr))
    {
        for(Tile* tile:tiles)
        {
            if((block->x+block->w==tile->x)&(block->y+block->h>tile->y)&(block->y+block->h<=tile->y+tile->h))
            {
                block->tr=tile;
                break;
            }
        }
    }
    if((block->y+block->h<out_h)&&(block->rt==nullptr))
    {
        for(Tile* tile:tiles)
        {
            if((block->x+block->w>tile->x)&(block->x+block->w<=tile->x+tile->w)&(block->y+block->h==tile->y))
            {
                block->rt=tile;
                break;
            }
        }
    }
    for(auto it=check_merge_tiles.begin();it!=check_merge_tiles.end();)
    {
        Tile* tile=*it;
        if(tile->y+tile->h==block->y+block->h)
        {
            if(check_top_merge(tile))
            {
                top_merge(tile,tiles);
                it=check_merge_tiles.erase(it);
                check_merge_tiles.push_back(tile);//merge again if possible
            }
        }
        if(check_bottom_merge(tile))
        {
            bottom_merge(tile,tiles);
            it=check_merge_tiles.erase(it);
            check_merge_tiles.push_back(tile);//merge again if possible
        }
        else
            it++;
    }
    check_merge_tiles.clear();
    check_new_pointer.clear();
}

Tile* point_finding(int point_x,int point_y,int out_w,int out_h,vector<Tile*> &tiles)//left and bottom belongs to a block
{
    Tile* current=tiles[0];
    while(1)
    {
        if((point_x>=current->x)&&(point_x<current->x+current->w)&&(point_y>=current->y)&&(point_y<current->y+current->h))
            return current;
        if((point_x>=current->x)&&(point_x<=current->x+current->w)&&(point_y>=current->y)&&(point_y<=current->y+current->h)&&(point_x==out_w))
            return current;
        if((point_x>=current->x)&&(point_x<current->x+current->w)&&(point_y>=current->y)&&(point_y<=current->y+current->h)&&(point_y==out_h))
            return current;
        //vertical
        while(point_y<current->y)
        {
            if(current->lb!=nullptr)
                current=current->lb;
            else 
                break;
        }
        while(point_y>=current->y+current->h)
        {
            if(current->rt!=nullptr)
                current=current->rt;
            else 
                break;
        }
        //horizontal
        while(point_x<current->x)
        {
            if(current->bl!=nullptr)
                current=current->bl;
            else 
                break;
        }
        while(point_x>=current->x+current->w)
        {
            if(current->tr!=nullptr)
                current=current->tr;
            else 
                break;
        }
    }
}

void block_creating(int out_w,int out_h,Tile* block,vector<Tile*> &tiles,vector<Tile*> &check_merge_tiles)
{
    vector<Tile*> affected_tiles;
    Tile* top_tile=point_finding(block->x,(block->y+block->h),out_w,out_h,tiles);//chance of getting block tile
    Tile* bottom_tile=point_finding(block->x,block->y,out_w,out_h,tiles);//always a blank tile
    if(top_tile->y==block->y+block->h)
    {
        Tile* temp=top_tile->lb;
        if((top_tile->x<block->x+block->w)&&(top_tile->x+top_tile->w>=block->x+block->w))
            block->rt=top_tile;
        else
        {
            Tile* temp2=top_tile->tr;
            while((temp2->x>=block->x+block->w)||(temp2->x+temp2->w<block->x+block->w))
            {
                temp2=temp2->tr;
                while(temp2->y!=block->y+block->h)
                    temp2=temp2->lb;
            }
            block->rt=top_tile;
        }
        if(top_tile->x==block->x)
            top_tile->lb=block;
        while((temp->x+temp->w)<=block->x)
        {
            if(temp->x+temp->w==block->x)
            {  
                Tile* temp2=temp->tr;
                temp->tr=block;
                temp=temp2;
            }
            else if (temp->tr!=nullptr)
                temp=temp->tr;
        }
        top_tile=temp;
        if(top_tile->rt!=nullptr)
        {
            Tile* temp=top_tile->rt;
            while((temp->x>=block->x)&&(temp->x<block->x+block->w))
            {
                temp->lb=block;
                if((block->x+block->w>temp->x)&&(block->x+block->w<=temp->x+temp->w))
                    block->rt=temp;
                if(temp->bl!=nullptr)
                    temp=temp->bl;
                else
                    break;
            }
        }
    }
    if(bottom_tile->y==block->y)
    {
        if(bottom_tile->lb!=nullptr)
        {
            Tile* temp=bottom_tile->lb;
            while((temp->x>block->x)||(temp->x+temp->w<=block->x))
                temp=temp->tr;
            if((temp->x<=block->x)&&(temp->x+temp->w>block->x))
            {
                block->lb=temp;
                while((temp->x+temp->w>block->x)&&(temp->x+temp->w<=block->x+block->w))
                {
                    temp->rt=block;
                    if(temp->tr!=nullptr)
                        temp=temp->tr;
                    else
                        break;
                }
            }
        }
    }
    affected_tiles.push_back(top_tile);
    bool reached_bottom=0;
    Tile* current=top_tile;
    if((current->x==block->x)&&(current->bl!=nullptr))
    {
        Tile* temp=current->bl;
        if((block->y>=temp->y)&&(block->y<temp->y+temp->h))
            block->bl=temp;
        while((temp->x+temp->w==block->x)&&(temp->y+temp->h>block->y)&&(temp->y+temp->h<=block->y+block->h))
        {
            temp->tr=block;
            if((block->y>=temp->y)&&(block->y<temp->y+temp->h))
                block->bl=temp;
            if(temp->rt!=nullptr)
                temp=temp->rt;
            else
                break;
        }
    }
    if((current->x+current->w==block->x+block->w)&&(current->tr!=nullptr))
    {
        Tile* temp=current->tr;
        if((block->y+block->h>temp->y)&&(block->y+block->h<=temp->y+temp->h))
            block->tr=temp;
        while((temp->x==block->x+block->w)&&(temp->y>=block->y)&&(temp->y<block->y+block->h))
        {
            temp->bl=block;
            if((block->y+block->h>temp->y)&&(block->y+block->h<=temp->y+temp->h))
                block->tr=temp;
            if(temp->lb!=nullptr)
                temp=temp->lb;
            else
                break;
        }
    }
    while(top_tile!=bottom_tile)
    {
        if(current==bottom_tile)
            reached_bottom=1;
        if(!reached_bottom)//trace left edge
        {
            current=current->lb;
            while((current->x+current->w)<=block->x)
            {
                if(current->tr!=nullptr)
                    current=current->tr;
                else
                    break;
            }
        }
        else//trace right edge
        {
            if(current->rt!=nullptr)
                current=current->rt;
            else 
                break;
            while(current->x>(block->x+block->w))
            {
                if(current->bl!=nullptr)
                    current=current->bl;
                else
                    break;
            }           
        }
        if(current==top_tile||((block->x+block->w==current->x)&&(current->y+current->h>=block->y+block->h)))
            break;
        if(find(affected_tiles.begin(),affected_tiles.end(),current)==affected_tiles.end()&&current!=block)
        {
            if(current->type==1)
            {
                if(!reached_bottom)
                {
                    current->tr=block;
                    if((block->y>=current->y)&&(block->y<current->y+current->h))
                        block->bl=current;
                }
                else
                {
                    current->bl=block;
                    if((block->y+block->h>current->y)&&(block->y+block->h<=current->y+current->h))
                        block->tr=current;
                }
            }
            else
            {
                if((current->x==block->x)&&current->bl!=nullptr)
                {
                    Tile* temp=current->bl;
                    if((block->y>=temp->y)&&(block->y<temp->y+temp->h))
                        block->bl=temp;
                    while((temp->x+temp->w==block->x)&&(temp->y+temp->h>block->y)&&(temp->y+temp->h<=block->y+block->h))
                    {
                        temp->tr=block;
                        if((block->y>=temp->y)&&(block->y<temp->y+temp->h))
                            block->bl=temp;
                        if(temp->rt!=nullptr)
                            temp=temp->rt;
                        else
                            break;
                    }
                }
                if((current->x+current->w==block->x+block->w)&&current->tr!=nullptr)
                {
                    Tile* temp=current->tr;
                    if((block->y+block->h>temp->y)&&(block->y+block->h<=temp->y+temp->h))
                        block->tr=temp;
                    while((temp->x==block->x+block->w)&&(temp->y>=block->y)&&(temp->y<block->y+block->h))
                    {
                        temp->bl=block;
                        if((block->y+block->h>temp->y)&&(block->y+block->h<=temp->y+temp->h))
                            block->tr=temp;
                        if(temp->lb!=nullptr)
                            temp=temp->lb;
                        else
                            break;
                    }
                }
                affected_tiles.push_back(current);
            }
        }
    }
    for(Tile* tile:affected_tiles)
        split_tile(out_w,out_h,tile,block,tiles,check_merge_tiles);
    affected_tiles.clear();
}

void split_tile(int out_w,int out_h,Tile* tile,Tile* block,vector<Tile*> &tiles,vector<Tile*> &check_merge_tiles)//deal with inner pointers here
{
    vector<Tile*> new_tiles;
    bool top_split=0;
    bool bottom_split=0;
    Tile* top=nullptr;
    Tile* bottom=nullptr;
    Tile* left=nullptr;
    Tile* right=nullptr;
    if((block->y+block->h)<(tile->y+tile->h))
    {
        top_split=1;
        top=new Tile(tile->x,(block->y+block->h),tile->w,((tile->y+tile->h)-(block->y+block->h)),0);
        if(tile->bl!=nullptr)
        {
            Tile* temp=tile->bl;
            while((top->y<temp->y)||(top->y>=temp->y+temp->h))
            {
                if(temp->rt!=nullptr)
                    temp=temp->rt;
                else
                    break;
            }
            if((top->y>=temp->y)&&(top->y<temp->y+temp->h))
                top->bl=temp;     
        }
        new_tiles.push_back(top);
    }
    if(block->y>tile->y)
    {
        bottom_split=1;
        bottom=new Tile(tile->x,tile->y,tile->w,(block->y-tile->y),0);
        if(tile->tr!=nullptr)
        {
            Tile* temp=tile->tr;
            while((bottom->y+bottom->h<=temp->y)||(bottom->y+bottom->h>temp->y+temp->h))
            {
                if(temp->lb!=nullptr)
                    temp=temp->lb;
                else
                    break;
            }
            if((bottom->y+bottom->h>temp->y)&&(bottom->y+bottom->h<=temp->y+temp->h))
                bottom->tr=temp;     
        }
        new_tiles.push_back(bottom);
    }
    if(block->x>tile->x)
    {
        if(top_split&&bottom_split)
        {
            left=new Tile(tile->x,block->y,(block->x-tile->x),block->h,0);
            left->rt=top;
            left->lb=bottom;
            top->lb=left;
            if(tile->bl!=nullptr)
            {
                Tile* temp=tile->bl;
                while((left->y<temp->y)||(left->y>=temp->y+temp->h))
                {
                    if(temp->rt!=nullptr)
                        temp=temp->rt;
                    else
                        break;
                }
                if((temp->x+temp->w==left->x)&&(left->y>=temp->y)&&(left->y<temp->y+temp->h))
                    left->bl=temp;
            }
        }
        else if(top_split)
        {
            left=new Tile(tile->x,tile->y,(block->x-tile->x),((block->y+block->h)-tile->y),0);
            left->rt=top;
            top->lb=left;
        }
        else if(bottom_split)
        {
            left=new Tile(tile->x,block->y,(block->x-tile->x),((tile->y+tile->h)-block->y),0);
            left->lb=bottom;
            if((left->y+left->h==block->y+block->h)&&(left->y+left->h<out_h))
            {
                Tile* temp=block->rt;
                while((left->x+left->w<=temp->x)||(left->x+left->w>temp->x+temp->w)||(left->y+left->h!=temp->y))
                    temp=temp->bl;
                if((left->x+left->w>temp->x)&&(left->x+left->w<=temp->x+temp->w)&&(left->y+left->h==temp->y))
                    left->rt=temp; 
            }
            if(tile->bl!=nullptr)
            {
                Tile* temp=tile->bl;
                while((left->y<temp->y)||(left->y>=temp->y+temp->h))
                {
                    if(temp->rt!=nullptr)
                        temp=temp->rt;
                    else
                        break;
                }
                if((temp->x+temp->w==left->x)&&(left->y>=temp->y)&&(left->y<temp->y+temp->h))
                    left->bl=temp;
            }
        }
        else
        {
            left=new Tile(tile->x,tile->y,(block->x-tile->x),tile->h,0);
            if((left->y+left->h==block->y+block->h)&&(left->y+left->h<out_h))
            {
                Tile* temp=block->rt;
                while((left->x+left->w<=temp->x)||(left->x+left->w>temp->x+temp->w)||(left->y+left->h!=temp->y))
                    temp=temp->bl;
                if((left->x+left->w>temp->x)&&(left->x+left->w<=temp->x+temp->w)&&(left->y+left->h==temp->y))
                    left->rt=temp; 
            }
        }
        new_tiles.push_back(left);
    }
    if((block->x+block->w)<(tile->x+tile->w))
    {
        if(top_split&&bottom_split)
        {
            right=new Tile((block->x+block->w),block->y,((tile->x+tile->w)-(block->x+block->w)),block->h,0);
            right->rt=top;
            right->lb=bottom;
            bottom->rt=right;
            if(tile->tr!=nullptr)
            {
                Tile* temp=tile->tr;
                while((right->y+right->h<=temp->y)||(right->y+right->h>temp->y+temp->h))
                {
                    if(temp->lb!=nullptr)
                        temp=temp->lb;
                    else
                        break;
                }
                if((right->x+right->w==temp->x)&&(right->y+right->h>temp->y)&&(right->y+right->h<=temp->y+temp->h))
                    right->tr=temp;
            }
        }
        else if(top_split)
        {
            right=new Tile((block->x+block->w),tile->y,((tile->x+tile->w)-(block->x+block->w)),((block->y+block->h)-tile->y),0);
            right->rt=top;
            if((right->y==block->y)&&(right->y!=0))
            {
                Tile* temp=block->lb;
                while((right->x<temp->x)||(right->x>=temp->x+temp->w)||(right->y!=temp->y+temp->h))
                    temp=temp->tr;
                if((right->x>=temp->x)&&(right->x<temp->x+temp->w)&&(right->y==temp->y+temp->h))
                    right->lb=temp;                
            }
            if(tile->tr!=nullptr)
            {
                Tile* temp=tile->tr;
                while((right->y+right->h<=temp->y)||(right->y+right->h>temp->y+temp->h))
                {
                    if(temp->lb!=nullptr)
                        temp=temp->lb;
                    else
                        break;
                }
                if((right->x+right->w==temp->x)&&(right->y+right->h>temp->y)&&(right->y+right->h<=temp->y+temp->h))
                    right->tr=temp;
            }
        }
        else if(bottom_split)
        {
            right=new Tile((block->x+block->w),block->y,((tile->x+tile->w)-(block->x+block->w)),((tile->y+tile->h)-block->y),0);
            right->lb=bottom;
            bottom->rt=right;
        }
        else
        {
            right=new Tile((block->x+block->w),tile->y,((tile->x+tile->w)-(block->x+block->w)),tile->h,0);
            if((right->y==block->y)&&(right->y!=0))
            {
                Tile* temp=block->lb;
                while((right->x<temp->x)||(right->x>=temp->x+temp->w)||(right->y!=temp->y+temp->h))
                    temp=temp->tr;
                if((right->x>=temp->x)&&(right->x<temp->x+temp->w)&&(right->y==temp->y+temp->h))
                    right->lb=temp;                
            }
        }
        new_tiles.push_back(right);
    }
    for(Tile* new_tile:new_tiles)
    {
        update_tile_pointers(new_tile,tile,block,tiles);
        check_merge_tiles.push_back(new_tile);
        tiles.push_back(new_tile);
    }
    tiles.erase(remove(tiles.begin(),tiles.end(),tile),tiles.end());
    delete tile;
}

void update_tile_pointers(Tile* new_tile,Tile* old_tile,Tile* block,vector<Tile*> &tiles)//outer pointer
{
    //inheritance
    if((new_tile->y==old_tile->y)&&(new_tile->x==old_tile->x)) 
    {
        new_tile->lb=old_tile->lb;
        new_tile->bl=old_tile->bl;
    }
    if((new_tile->y+new_tile->h==old_tile->y+old_tile->h)&&(new_tile->x+new_tile->w==old_tile->x+old_tile->w))
    {
        new_tile->rt=old_tile->rt;
        new_tile->tr=old_tile->tr;
    }
    //update neighbor
    for(Tile* tile:tiles)
    {
        if(tile==old_tile)
            continue;
        if(tile->lb==old_tile)
        {
            if((tile->x>=new_tile->x)&&(tile->x<new_tile->x+new_tile->w)&&(tile->y==new_tile->y+new_tile->h))
                tile->lb=new_tile;
            if((new_tile->x+new_tile->w>tile->x)&&(new_tile->x+new_tile->w<=tile->x+tile->w)&&(tile->y==new_tile->y+new_tile->h))
                new_tile->rt=tile;
            if((tile->x>=block->x)&&(tile->x<block->x+block->w)&&(tile->y==block->y+block->h))
                tile->lb=block;
            if((block->x+block->w>tile->x)&&(block->x+block->w<=tile->x+tile->w)&&(tile->y==block->y+block->h))
                block->rt=tile;
        }
        if(tile->bl==old_tile)
        {
            if((tile->x==new_tile->x+new_tile->w)&&(tile->y>=new_tile->y)&&(tile->y<new_tile->y+new_tile->h))
                tile->bl=new_tile;
            if((tile->x==new_tile->x+new_tile->w)&&(new_tile->y+new_tile->h>tile->y)&&(new_tile->y+new_tile->h<=tile->y+tile->h))
                new_tile->tr=tile;
            if((tile->x==block->x+block->w)&&(tile->y>=block->y)&&(tile->y<block->y+block->h))
                tile->bl=block;
            if((tile->x==block->x+block->w)&&(block->y+block->h>tile->y)&&(block->y+block->h<=tile->y+tile->h))
                block->tr=tile;
        }
        if(tile->rt==old_tile)
        {
            if((tile->x+tile->w>new_tile->x)&&(tile->x+tile->w<=new_tile->x+new_tile->w)&&(tile->y+tile->h==new_tile->y))
                tile->rt=new_tile;
            if((new_tile->x>=tile->x)&&(new_tile->x<tile->x+tile->w)&&(tile->y+tile->h==new_tile->y))
                new_tile->lb=tile;
            if((tile->x+tile->w>block->x)&&(tile->x+tile->w<=block->x+block->w)&&(tile->y+tile->h==block->y))
                tile->rt=block;
            if((block->x>=tile->x)&&(block->x<tile->x+tile->w)&&(tile->y+tile->h==block->y))
                block->lb=tile;
        }
        if(tile->tr==old_tile)
        {
            if((tile->x+tile->w==new_tile->x)&&(tile->y+tile->h>new_tile->y)&&(tile->y+tile->h<=new_tile->y+new_tile->h))
                tile->tr=new_tile;
            if((tile->x+tile->w==new_tile->x)&&(new_tile->y>=tile->y)&&(new_tile->y<tile->y+tile->h))
                new_tile->bl=tile;
            if((tile->x+tile->w==block->x)&&(tile->y+tile->h>block->y)&&(tile->y+tile->h<=block->y+block->h))
                tile->tr=block;
            if((tile->x+tile->w==block->x)&&(block->y>=tile->y)&&(block->y<tile->y+tile->h))
                block->bl=tile;
        }
    }
    //new_block
    if((new_tile->y==block->y+block->h)&&(new_tile->x==block->x))
        new_tile->lb=block;
    if(new_tile->x==block->x+block->w)
        new_tile->bl=block;
    if((new_tile->y+new_tile->h==block->y)&&(new_tile->x+new_tile->w==block->x+block->w))
        new_tile->rt=block;
    if(new_tile->x+new_tile->w==block->x)
        new_tile->tr=block;
    //initialize block pointers
    if((new_tile->x+new_tile->w>block->x)&&(new_tile->y+new_tile->h==block->y))
        block->lb=new_tile;
    if((new_tile->x+new_tile->w==block->x)&&(new_tile->y<=block->y)&&(new_tile->y+new_tile->h>block->y))
        block->bl=new_tile;
    if((new_tile->x<block->x+block->w)&&(new_tile->x+new_tile->w>=block->x+block->w)&&(new_tile->y==block->y+block->h))///add more condition!!
        block->rt=new_tile;
    if((new_tile->x==block->x+block->w)&&(new_tile->y<block->y+block->h)&&(new_tile->y+new_tile->h>=block->y+block->h))
        block->tr=new_tile;
}

bool check_top_merge(Tile* current)
{
    if(current->rt==nullptr)
        return 0;
    if(current->type==1)
        return 0;
    if(current->rt->type==1)
        return 0;
    if(current->x!=current->rt->x)
        return 0;
    if(current->w!=current->rt->w)
        return 0;
    if((current->y+current->h)!=current->rt->y)
        return 0;
    return 1;
}

void top_merge(Tile* current,vector<Tile*> &tiles)//merge bottom tile //bl<- ->tr//remember to remove current->lb from Tiles!!
{
    Tile* temp=current->rt;
    for(Tile* tile:tiles)
    {
        if(tile->lb==temp)
            tile->lb=current;
        if(tile->bl==temp)
            tile->bl=current;
        if(tile->tr==temp)
            tile->tr=current;
    }
    current->h=(current->h)+(temp->h);
    current->rt=temp->rt;
    current->tr=temp->tr;
    temp->lb=nullptr;
    temp->bl=nullptr;
    temp->rt=nullptr;
    temp->tr=nullptr;
    tiles.erase(remove(tiles.begin(),tiles.end(),temp),tiles.end());
}

bool check_bottom_merge(Tile* current)
{
    if(current->lb==nullptr)
        return 0;
    if(current->type==1)
        return 0;
    if(current->lb->type==1)
        return 0;
    if(current->x!=current->lb->x)
        return 0;
    if(current->w!=current->lb->w)
        return 0;
    if(current->y!=(current->lb->y+current->lb->h))
        return 0;
    return 1;
}

void bottom_merge(Tile* current,vector<Tile*> &tiles)//merge bottom tile //bl<- ->tr//remember to remove current->lb from Tiles!!
{
    Tile* temp=current->lb;
    for(Tile* tile:tiles)
    {
        if(tile->bl==temp)
            tile->bl=current;
        if(tile->rt==temp)
            tile->rt=current;
        if(tile->tr==temp)
            tile->tr=current;
    }
    current->y=temp->y;
    current->h=(current->h)+(temp->h);
    current->lb=temp->lb;
    current->bl=temp->bl;
    temp->lb=nullptr;
    temp->bl=nullptr;
    temp->rt=nullptr;
    temp->tr=nullptr;
    tiles.erase(remove(tiles.begin(),tiles.end(),temp),tiles.end());
}

void final_merge(vector<Tile*> &tiles)
{
    for(auto it=tiles.begin();it!=tiles.end();)
    {
        Tile* tile=*it;
        if(check_bottom_merge(tile))
        {
            bottom_merge(tile,tiles);
            if((*it)!=tile)
                it--;
            it=tiles.erase(it);
            tiles.push_back(tile);//merge again if possible
        }
        else
            it++;
    }
}

void neighbor_finding(int out_w,int out_h,vector<Tile*> &tiles,map<int,int> &block_neighbor,map<int,int> &blank_neighbor)
{
    Tile* current;char c;
    vector<Tile*> count_block_neighbor;
    vector<Tile*> count_blank_neighbor;
    for(Tile* tile:tiles)
    {
        if(tile->type==1)
        {
            if(tile->x>0)//bl->rt
            {
                current=tile->bl;
                count_neighbor(current,count_block_neighbor,count_blank_neighbor);
                while(current->y+current->h<tile->y+tile->h)
                {
                    current=current->rt;
                    count_neighbor(current,count_block_neighbor,count_blank_neighbor);
                }
            }
            if(tile->y>0)//lb->tr
            {
                current=tile->lb;
                count_neighbor(current,count_block_neighbor,count_blank_neighbor);
                while(current->x+current->w<tile->x+tile->w)
                {
                    current=current->tr;
                    count_neighbor(current,count_block_neighbor,count_blank_neighbor);
                }
            }
            if(tile->x+tile->w<out_w)//tr->lb
            {
                current=tile->tr;
                count_neighbor(current,count_block_neighbor,count_blank_neighbor);
                while(current->y>tile->y)
                {
                    current=current->lb;
                    count_neighbor(current,count_block_neighbor,count_blank_neighbor);
                }
            }
            if(tile->y+tile->h<out_h)//rt->bl
            {
                current=tile->rt;
                count_neighbor(current,count_block_neighbor,count_blank_neighbor);
                while(current->x>tile->x)
                {
                    current=current->bl;
                    count_neighbor(current,count_block_neighbor,count_blank_neighbor);
                }
            }
            block_neighbor[tile->block_num]=count_block_neighbor.size();
            blank_neighbor[tile->block_num]=count_blank_neighbor.size();
            count_block_neighbor.clear();
            count_blank_neighbor.clear();
        }
    }
}

void count_neighbor(Tile* current,vector<Tile*> &count_block_neighbor,vector<Tile*> &count_blank_neighbor)
{
    if(current->type==1)
    {
        if(find(count_block_neighbor.begin(),count_block_neighbor.end(),current)==count_block_neighbor.end())
            count_block_neighbor.push_back(current);
    }
    else
    {
        if(find(count_blank_neighbor.begin(),count_blank_neighbor.end(),current)==count_blank_neighbor.end())
            count_blank_neighbor.push_back(current);   
    }
}

