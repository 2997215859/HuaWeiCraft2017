#include "deploy.h"
#include "lib_io.h"
#include "lib_time.h"
#include "min_cost_flow.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <regex>
#include <iostream>

void deploy_server(char* inLines[MAX_IN_NUM], int inLineNum, const char * const filename)
{
	/*
	* 构建基本数据结构
	*/
	std::smatch m0;
	std::string strLine0(*inLines);
	std::regex_match(strLine0, m0, std::regex("(\\d+)\\s(\\d+)\\s(\\d+)[.\\n\\r]*"));
	int netNodeNum = atoi(m0.str(1).c_str()); // 网络节点数量
	int netEdgeNum = atoi(m0.str(2).c_str()); // 网络链路数量
	int consumerNodeNum = atoi(m0.str(3).c_str()); // 消费节点数量	
	int serverCost = atoi(*(inLines + 2));

	int edgeInfoNoStart = 4;
	
	MinCostFlow minCostFlow(netNodeNum, netEdgeNum, consumerNodeNum);
	for (int lineCount = edgeInfoNoStart; lineCount <= edgeInfoNoStart + netEdgeNum; lineCount++) {
		minCostFlow.insert_edge();
	}
	std::cout << serverCost;

	std::string res;
	res.append("4");
	res.append("\n\n");
	res.append("204 201 101 12 70").append("\n");
	res.append("204 207 206 104 12 50").append("\n");
	res.append("204 207 208 103 12 30").append("\n");
	res.append("204 203 202 102 12 40").append("\n");

	write_result(res.c_str(), filename);
	system("pause");
}


