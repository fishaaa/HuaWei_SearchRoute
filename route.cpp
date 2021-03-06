#include "route.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <stack>
using namespace std;

stack<string> path;
vector<stack<string> > path_all;
/*************************Algorithms------DFS*****************************************/
void DFS_Visit(map<string, list<vector<string> > > Adj, map<string, int> &color, string u, stack<string> &path, string DestinationID)
{
	color[u] = 1;
	for (auto beg = Adj[u].begin(); beg != Adj[u].end(); ++beg){   //遍历当前顶点所有相邻的点
		string v = beg->at(2);
		if (color[v] == 0){
			color[v] = 1;
		//	cost[v] = cost[u] + stoi(beg->at(3));
			path.push(beg->at(0));
			if (v == DestinationID){
				color[v] = 0;
				path_all.push_back(path);
				path.pop();
				continue;
			}
			DFS_Visit(Adj, color, v, path, DestinationID);
		}
	}
	color[u] = 0;
	if (!path.empty()){
		path.pop();
	}
	
}
void DFS(map<string, list<vector<string>>> Adj, string SourceID, string DestinationID)
{
	/**************DFS算法初始化************************/    //color = 0 未着色 1为着色
	map<string, int> color;
	map<string, int> cost;
	for (auto beg = Adj.begin(); beg != Adj.end(); ++beg){
		color[beg->first] = 0;
	//	cost[beg->first] = 0;
	}
	auto it = Adj.find(SourceID);
	
	//auto it_t = Adj.begin();
	if (color[it->first] == 0){  // 从目标起点开始搜索
		DFS_Visit(Adj, color, it->first, path, DestinationID);
	}
}
/******************************************************************/

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand){

	/*************************得到V'的vector集合*****************************************/
	string IncludeSetTemp = string(demand);
	vector <string> IncludeSet;
	string::size_type temp1 = IncludeSetTemp.find(',');
	string SourceID = IncludeSetTemp.substr(0, temp1);
	IncludeSetTemp.erase(0, temp1 + 1);
	temp1 = IncludeSetTemp.find(',');
	string DestinationID = IncludeSetTemp.substr(0, temp1);
	IncludeSetTemp.erase(0, temp1 + 1);
	while (IncludeSetTemp.find('|') != string::npos){  
		auto temp = IncludeSetTemp.find('|');
		IncludeSet.push_back(IncludeSetTemp.substr(0, temp ));
		IncludeSetTemp.erase(0, temp+1);	
	}
	IncludeSet.push_back(IncludeSetTemp);// 得到V'点


/*************************得到G的邻接表结构*****************************************/
	map <string,list<vector<string>>> Adj;
	vector <vector<string>> topo_str;
	for (int i = 0; i < edge_num; ++i){
		string temp = topo[i];
		vector<string> t;
		while (temp.find(',') != string::npos){
			string::size_type temp1 = temp.find(',');
			t.push_back(temp.substr(0, temp1));
			temp.erase(0, temp1 + 1);
		}
		string::size_type temp2 = temp.find('\n');
		t.push_back(temp.substr(0, temp2));
		topo_str.push_back(t);
		Adj[t[1]].push_back(t);  //根据sourseID的值利用Map进行映射	
	}

	DFS(Adj, SourceID, DestinationID);
	/***************删选对应的路径,获得满足题意的路径**************/
	stack<string>  path_ture;
	vector<stack<string> > path_temp = path_all;
	int k;
	int cost = 0, cost_temp = 0, count = 0;
	string result_temp;
	for (k = 0; k < path_temp.size(); ++k){
		if (path_temp[k].size() < IncludeSet.size())
			continue;
		else{
			vector<string> temp;
			while (!(path_temp[k].empty()))
			{
				string s = path_temp[k].top();
				string ss= topo_str[stoi(s)][2];// 目标经历点
				cost_temp += stoi(topo_str[stoi(s)][3]);
				temp.push_back(ss);
				path_temp[k].pop();
			}
			int n = 0;
			for (int i = 0; i < temp.size(); ++i){
				for (int j = 0; j < IncludeSet.size(); ++j){
					if (temp[i] == IncludeSet[j]){
						++n;
					}
				}
			}
			if (n == IncludeSet.size()){
				++count;
				if (count == 1){
					path_ture=path_all[k];
				}
				else {
					if (cost > cost_temp){
						cost = cost_temp;
						path_ture = path_all[k];
					}
					
				}
				cost_temp = 0;
			}
			n = 0;
			temp.clear();
		}
		//while ()
	}
	string result;
	if (path_ture.empty()) result = "NA";
	vector<string> temp_result;
	while (!path_ture.empty()){
		temp_result.push_back(path_ture.top());
		path_ture.pop();
	}
	for (int i = temp_result.size()-1; i >= 0; --i){
		result += temp_result[i];
		if (i != 0){
			result += "|";
		}
	}
	cout << result;
}
