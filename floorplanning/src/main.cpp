#include<iostream>
#include<fstream>
#include<vector>
#include<list>
#include<queue>
#include<unordered_map>
#include<algorithm>
#include<random>
#include<cmath>
#include<chrono>
#include"parser.h"
#include"bstar_tree.h"
using namespace std;


int main(int argc, char **argv)
{	
	int out_w,out_h,block_num,terminal_num,net_num,HPWL;
	double alpha=stod(argv[1]);
	auto start = std::chrono::high_resolution_clock::now();
	// time_t start_time,end_time;
	BS_tree* bst;
	vector<Block*> block_vec;
	vector<Block*> terminal_vec;
	vector<Net*> net_vec;
	list<Contour> contour_list;
	unordered_map<string,Block*> block_map;
	unordered_map<string,Block*> terminal_map;
	// time(&start_time);
	ifstream fin;
	ofstream fout;
	fin.open(argv[2]);
	read_block(fin,out_w,out_h,block_num,terminal_num,block_vec,terminal_vec,block_map,terminal_map);
	fin.open(argv[3]);
	read_net(fin,net_num,net_vec);
	build_tree(bst,block_vec);
	simulate_annealing(out_w,out_h,block_num,alpha,HPWL,bst,block_vec,terminal_vec,net_vec,contour_list,block_map,terminal_map);
	double cost=cost_calculation(alpha,HPWL,bst,net_vec,block_map,terminal_map);
	// time(&end_time);
	auto end = std::chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	fout.open(argv[4]);
	write_file(fout,HPWL,elapsed_seconds.count(),cost,bst,block_vec);
	return 0;
}