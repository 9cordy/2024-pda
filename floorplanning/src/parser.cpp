#include<fstream>
#include<vector>
#include<list>
#include<queue>
#include<unordered_map>
#include<algorithm>
#include<random>
#include<cmath>
#include"parser.h"
#include"bstar_tree.h"

using namespace std;

void read_block(ifstream& fin,int &out_w,int &out_h,int &block_num,int &terminal_num,vector<Block*> &block_vec,vector<Block*> &terminal_vec,unordered_map<string,Block*> &block_map,unordered_map<string,Block*> &terminal_map)
{    
	string temp;
	fin>>temp>>out_w>>out_h>>temp>>block_num>>temp>>terminal_num;
	Block* max_block=nullptr;
	for(int i=0;i<block_num;i++)//input block
	{
		int w,h;
		Block* b=new Block;
		fin>>b->name>>b->w>>b->h;
		if(b->w>out_w||b->h>out_h)//determine rotatable or not
		{
			swap(b->w,b->h);
			b->fixed=1; 
		}
		if(b->w>out_h||b->h>out_w)
			b->fixed=1; 	
		block_vec.push_back(b);
		block_map[b->name]=b;
		if(i==0)
			max_block=b;
		else if((b->w)*(b->h)>(max_block->w)*(max_block->h))
			max_block=b;
	}
	for(int i=0;i<terminal_num;i++)//input terminal
	{
		int x,y;
		Block* b=new Block;
		b->terminal=1;
		fin>>b->name>>temp>>b->x>>b->y;
		terminal_vec.push_back(b);
		terminal_map[b->name]=b;
	}
	fin.close();
}

void read_net(ifstream& fin,int &net_num,vector<Net*> &net_vec)
{   
    string temp; 
	fin>>temp>>net_num;
	for(int i=0;i<net_num;i++)//input net
	{
		int num;
		fin>>temp>>num;
		Net* n=new Net(num);
		for(int j=0;j<num;j++)
		{
			fin>>temp;
			n->block_names.push_back(temp);
		}
		net_vec.push_back(n);
	}
	fin.close();
}

void write_file(ofstream& fout,int HPWL,double time,int cost,BS_tree* bst,vector<Block*> &block_vec)
{    
	fout<<cost<<endl;
	fout<<HPWL<<endl;
	fout<<(bst->x_max)*(bst->y_max)<<endl;
	fout<<bst->x_max<<" "<<bst->y_max<<endl;
	fout<<time<<endl;
	for(Block* block:block_vec)
		fout<<block->name<<" "<<block->x<<" "<<block->y<<" "<<block->x+block->w<<" "<<block->y+block->h<<endl;
    fout.close();
}