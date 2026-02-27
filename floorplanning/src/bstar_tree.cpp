#include<iostream>
#include<fstream>
#include<vector>
#include<list>
#include<queue>
#include<unordered_map>
#include<algorithm>
#include<random>
#include<cmath>
#include"bstar_tree.h"

void build_tree(BS_tree* &bst,vector<Block*> &block_vec)
{
    queue<Block*> q;
    bst=new BS_tree;
    bst->root=block_vec[rand()%block_vec.size()];
    q.push(bst->root);
    bool lr=0;
    for(Block* block:block_vec)
    {
        if(block!=bst->root)
        {
            if(!lr)
            {
                block->parent=q.front();
                q.front()->left_child=block;
                q.push(block);
            }
            else
            {
                block->parent=q.front();
                q.front()->right_child=block;
                q.push(block);
                q.pop();//has both child
            }
            lr=!lr;
        }
    }
}

void ini(BS_tree* &bst,list<Contour> &contour_list)
{
    bst->x_max=0;
    bst->y_max=0;
    contour_list.clear();
    update_coor(bst->root,bst,contour_list);
}

int update_contour(BS_tree* &bst,Block* current,list<Contour> &contour_list)
{
    int x0,x1,y0,y1;
    x0=current->x;
    x1=x0+current->w;
    y0=0;
    y1=0;
    list<Contour>::iterator contour=contour_list.begin();
    while(contour!=contour_list.end())
    {
        if(contour->x_right<=x0)
            contour++;
        else if(contour->x>=x1)
            break;
        else
        {
            y0=max(contour->y,y0);
            if(contour->x>=x0&&contour->x_right<=x1)
                contour=contour_list.erase(contour);
            else if(contour->x>=x0&&contour->x_right>=x1)
            {
                contour->x=x1;
                continue;
            }
            else if(contour->x<=x0&&contour->x_right<=x1)
            {
                contour->x_right=x0;
                continue;
            }
            else
            {
                contour_list.insert(contour,Contour(contour->x,x0,contour->y));
                contour->x=x1;
                continue;
            }
        }
    }
    y1=y0+current->h;
    contour_list.insert(contour,Contour(x0,x1,y1));
    if(y1>bst->y_max)
        bst->y_max=y1;
    return y0;
}

void update_coor(Block* current,BS_tree* &bst,list<Contour> &contour_list)
{
    if(current)
    {
        const Block* parent_block=current->parent;
        if(parent_block==nullptr)
        {
            current->x=0;
            current->y=0;
            Contour c(current->x,current->w,current->h);
            contour_list.push_back(c);
            bst->x_max=current->w;
            bst->y_max=current->h;
        }
        else if(current==parent_block->left_child)
        {
            current->x=parent_block->x+parent_block->w;
            if(current->x+current->w>bst->x_max)
                bst->x_max=current->x+current->w;
            current->y=update_contour(bst,current,contour_list);
        }
        else if(current==parent_block->right_child)
        {
            current->x=parent_block->x;
            if(current->x+current->w>bst->x_max)
                bst->x_max=current->x+current->w;
            current->y=update_contour(bst,current,contour_list);
        }
        update_coor(current->left_child,bst,contour_list);
        update_coor(current->right_child,bst,contour_list);
    }
}

void block_rotate(Block* current)
{
	swap(current->w,current->h);
}

bool block_move(Block* current,Block* target,bool move_left,int child_pos)
{
    if(move_left)
    {
        if(child_pos==1)
        {
            current->parent->left_child=current->left_child;
            current->left_child->parent=current->parent;
            current->left_child=nullptr;
        }
        else if(child_pos==2)
        {
            current->parent->left_child=current->right_child;
            current->right_child->parent=current->parent;
            current->right_child=nullptr;
        }
        else
            current->parent->left_child=nullptr;
    }
    else
    {
        if(child_pos==1)
        {
            current->parent->right_child=current->left_child;
            current->left_child->parent=current->parent;
            current->left_child=nullptr;
        }
        else if(child_pos==2)
        {
            current->parent->right_child=current->right_child;
            current->right_child->parent=current->parent;
            current->right_child=nullptr;
        }
        else
            current->parent->right_child=nullptr;
    }
    bool move_to_left;
    if(target->left_child==nullptr)
        move_to_left=1;
    else if(target->right_child==nullptr)
        move_to_left=0;
    else
        move_to_left=rand()%2;
    if(move_to_left)
        target->left_child=current;
    else
        target->right_child=current;
    current->parent=target;
    return move_to_left;
}

void block_swap(Block* current,Block* target,unordered_map<string,Block*> &block_map)//swap all information
{
	swap(current->name,target->name);
	swap(current->h,target->h);
	swap(current->w,target->w);
	swap(current->fixed,target->fixed);
	swap(block_map[current->name],block_map[target->name]);
}

double area_cost(int out_w,int out_h,double alpha,int HPWL,BS_tree* &bst)
{
	double total=max(bst->x_max-out_w,0)+max(bst->y_max-out_h,0);
	if(total<1.0e-8)
		return ((out_w*out_h)*-5.0/(alpha*bst->x_max*bst->y_max+(1-alpha)*HPWL));
	else
		return total*10;
}

double cost_calculation(double alpha,int &HPWL,BS_tree* &bst,vector<Net*> &net_vec,unordered_map<string,Block*> &block_map,unordered_map<string,Block*> &terminal_map)
{
    int net_HPWL=0;
    int max_x,max_y,min_x,min_y,current_x,current_y;
    unordered_map<string,Block*>::iterator current;
    for(Net* net:net_vec)
    {
        max_x=0;
        max_y=0;
        min_x=100000000;
        min_y=100000000;
        for(string block_name:net->block_names)
        {
            current=terminal_map.find(block_name);
            if(current!=terminal_map.end())
            {
                current_x=current->second->x;
                current_y=current->second->y;
            }
            else
            {
                current_x=block_map[block_name]->x+(block_map[block_name]->w)/2;
                current_y=block_map[block_name]->y+(block_map[block_name]->h)/2;
            }
            if(current_x>max_x)
                max_x=current_x;
            if(current_y>max_y)
                max_y=current_y;
            if(current_x<min_x)
                min_x=current_x;
            if(current_y<min_y)
                min_y=current_y;
        }
        net_HPWL+=(max_x-min_x+max_y-min_y);
    }
    int total_cost=alpha*bst->x_max*bst->y_max+(1.0-alpha)*net_HPWL;
    HPWL=net_HPWL;////////////////////
    return total_cost;
}

double accept_prob(double current_cost,double new_cost,int temperature)
{
	return exp((current_cost-new_cost)/temperature);
}

void simulate_annealing(int out_w,int out_h,int block_num,double alpha,int &HPWL,BS_tree* &bst,vector<Block*> &block_vec,vector<Block*> &terminal_vec,vector<Net*> &net_vec,list<Contour> &contour_list,unordered_map<string,Block*> &block_map,unordered_map<string,Block*> &terminal_map)
{
    random_device rd;
    default_random_engine gen(rd());
    double cost,cost1,cost2,prob,temperature,not_frozen;
    bool converge=0;
    temperature=100000;
    not_frozen=1.0e-05;
    while(!converge)
    {
        int recover=0;
        size_t k=1000;
        uniform_int_distribution<> op_dis(0,2);
		uniform_int_distribution<> index_dis(0,block_num-1);
		uniform_real_distribution<> prob_dis(0.0,1.0);
        for(size_t i=0;i<k;i++)
        {
            int op,index,target_index;
            ini(bst,contour_list);
			cost=cost_calculation(alpha,HPWL,bst,net_vec,block_map,terminal_map);
            cost1=area_cost(out_w,out_h,alpha,HPWL,bst);
            op=op_dis(gen);
            index=index_dis(gen);
            target_index=index_dis(gen);
            if(op==0)//rotate
            {
                while(block_vec[index]->fixed==1)
                    index=index_dis(gen);
                block_rotate(block_vec[index]);
                ini(bst,contour_list);
				cost=cost_calculation(alpha,HPWL,bst,net_vec,block_map,terminal_map);
				cost2=area_cost(out_w,out_h,alpha,HPWL,bst);
				prob=accept_prob(cost1,cost2,temperature);
                if(cost1-cost2<0&&prob_dis(gen)>=prob)//recover
				{
					recover++;
					block_rotate(block_vec[index]);
				}
            }
            else if(op==1)//move
            {
                int child_pos;
                bool move_left,move_to_left;
                while((block_vec[index]->left_child!=nullptr&&block_vec[index]->right_child!=nullptr)||block_vec[index]==bst->root)
                    index=index_dis(gen);//don't move root //don't move node with two children
                while((block_vec[target_index]->left_child!=nullptr&&block_vec[target_index]->right_child!=nullptr)||block_vec[target_index]==block_vec[index]||block_vec[target_index]==block_vec[index]->parent)
					target_index=index_dis(gen);//choose new target when it is same as index or parent of index //can't move to node with two children
                Block* old_parent=block_vec[index]->parent;
                if(block_vec[index]==block_vec[index]->parent->left_child)
					move_left=1;
				else
					move_left=0;
                if(block_vec[index]->left_child==nullptr&&block_vec[index]->right_child==nullptr)
					child_pos=0;
				else if(block_vec[index]->left_child!=nullptr)
					child_pos=1;
				else
					child_pos=2;
                move_to_left=block_move(block_vec[index],block_vec[target_index],move_left,child_pos);
                ini(bst,contour_list);
				cost=cost_calculation(alpha,HPWL,bst,net_vec,block_map,terminal_map);
				cost2=area_cost(out_w,out_h,alpha,HPWL,bst);
				prob=accept_prob(cost1,cost2,temperature);
                if(cost1-cost2<0&&prob_dis(gen)>=prob)//recover
                {
                    recover++;
					block_vec[index]->parent=old_parent;
					if(move_to_left)
						block_vec[target_index]->left_child=nullptr;
					else
						block_vec[target_index]->right_child=nullptr;
                    if(move_left)
					{
						if(child_pos==1)
						{
							block_vec[index]->left_child=old_parent->left_child;
							block_vec[index]->left_child->parent=block_vec[index];
						}
						else if(child_pos==2)
						{
							block_vec[index]->right_child=old_parent->left_child;
							block_vec[index]->right_child->parent=block_vec[index];
						}
						old_parent->left_child=block_vec[index];
					}
                    else
					{
						if(child_pos==1)
						{
							block_vec[index]->left_child=old_parent->right_child;
							block_vec[index]->left_child->parent=block_vec[index];
						}
						else if(child_pos==2)
						{
							block_vec[index]->right_child=old_parent->right_child;
							block_vec[index]->right_child->parent=block_vec[index];
						}
						old_parent->right_child=block_vec[index];
					}
                }
            }
            else//swap
            {
                while(target_index==index)
                    target_index=index_dis(gen);
                block_swap(block_vec[index],block_vec[target_index],block_map);
                ini(bst,contour_list);
				cost=cost_calculation(alpha,HPWL,bst,net_vec,block_map,terminal_map);
				cost2=area_cost(out_w,out_h,alpha,HPWL,bst);
				prob=accept_prob(cost1,cost2,temperature);
                if(cost1-cost2<0&&prob_dis(gen)>=prob)//recover
				{
					recover++;
					block_swap(block_vec[index],block_vec[target_index],block_map);
				}
            }
            temperature*=0.3;
            double recover_rate=1.0*recover/k;
			if(recover_rate>0.96&&bst->x_max<=out_w&&bst->y_max<=out_h)
				converge=true;
			if(temperature<not_frozen&&bst->x_max<=out_w&&bst->y_max<=out_h)
				converge=true;
        }
    }
    ini(bst,contour_list);
}