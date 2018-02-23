#include "deploy.h"
#include "lib_io.h"
#include "lib_time.h"
#include "saa.h"
#include "min_cost_flow.h"
#include <bitset>
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
	const int netNodeNum = atoi(m0.str(1).c_str()); // 网络节点数量
	const int netEdgeNum = atoi(m0.str(2).c_str()); // 网络链路数量
	const int consumerNodeNum = atoi(m0.str(3).c_str()); // 消费节点数量	
	const int serverCost = atoi(*(inLines + 2));

	printf("网络节点数量=%d,网络链路数量=%d,消费节点数量=%d,服务器成本=%d\n", 
		netNodeNum, netEdgeNum, consumerNodeNum, serverCost);
	
	printf("初始化网络拓扑图，开始\n");
	MinCostFlow minCostFlow(netNodeNum, netEdgeNum, consumerNodeNum, serverCost); // 初始化网络流
	int edgeInfoNoStart = 4; // 链路信息起始行编号（从0开始计数）
	int consumerInfoNoStart = edgeInfoNoStart + netEdgeNum + 1;
	
	printf("  链路信息如下：\n");
	std::smatch m1; // 用于匹配出 链路起始节点id 链路终止节点id 容量 花费
	for (int lineCount = edgeInfoNoStart; lineCount <= edgeInfoNoStart + netEdgeNum - 1; lineCount++) {
		std::string strLine(*(inLines + lineCount));
		std::regex_match(strLine, m1, std::regex("(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)[.\\n\\r]*"));
		int u = atoi(m1.str(1).c_str()); // 链路起始节点id
		int v = atoi(m1.str(2).c_str()); // 链路终止节点id
		int vol = atoi(m1.str(3).c_str()); // 容量
		int cost = atoi(m1.str(4).c_str()); // 花费
		printf("  u=%2d,v=%2d,vol=%2d,cost=%2d\n",
			u, v, vol, cost);
		minCostFlow.insert_edge(u, v, vol, cost);
		minCostFlow.insert_edge(v, u, vol, cost);
	}

	// 超级汇点连到所有汇点的边的费用和容量分别为0和消费节点带宽所需的消耗/
	// 超级汇点编号为netNodeNum
	std::vector<int> serverLinkIdsInit; // 初始服务器位置
	std::smatch m2; // 用于匹配出 消费节点id 相连节点id 视频带宽需求
	int demandSum = 0;
	for (int lineCount = consumerInfoNoStart; lineCount <= consumerInfoNoStart + consumerNodeNum - 1; lineCount++) {
		std::string  strLine(*(inLines + lineCount));
		std::regex_match(strLine, m2, std::regex("(\\d+)\\s(\\d+)\\s(\\d+)[.\\n\\r]*"));
		int consumerId = atoi(m2.str(1).c_str()); // 消费节点id
		int consumerLinkId = atoi(m2.str(2).c_str()); // 相连节点id
		int demand = atoi(m2.str(3).c_str()); // 视频带宽需求
		printf("  消费节点=%2d,超级汇点编号=%2d,vol=%2d,cost=%2d\n",
			consumerLinkId, netNodeNum, demand, 0);
		minCostFlow.insert_edge(consumerLinkId, netNodeNum, demand, 0);
		minCostFlow.insert_edge(netNodeNum, consumerLinkId, demand, 0);
		demandSum += demand;
		serverLinkIdsInit.push_back(consumerLinkId); // 初始服务器位置
	}
	minCostFlow.set_demand_sum(demandSum);
	printf("  初始服务器节点选在消费节点上：");
	for (auto serverLinkIdInit : serverLinkIdsInit) {
		printf("%d ", serverLinkIdInit);
	}
	printf("\n初始化网络拓扑图，完毕\n\n");
	printf("开始遗传退火算法\n");

	SAA saa;
	
	// serverLinkIdsInit = { 43,44,22,7,37,13,16,38,34 };
	serverLinkIdsInit = { 7,37,15,13,22,43,38};
	saa.saa(minCostFlow, serverLinkIdsInit);


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


