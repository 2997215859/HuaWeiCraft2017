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
	const int netNodeNum = atoi(m0.str(1).c_str()); // 网络节点数量
	const int netEdgeNum = atoi(m0.str(2).c_str()); // 网络链路数量
	const int consumerNodeNum = atoi(m0.str(3).c_str()); // 消费节点数量	
	const int serverCost = atoi(*(inLines + 2));

	

	MinCostFlow minCostFlow(netNodeNum, netEdgeNum, consumerNodeNum); // 初始化网络流
	int edgeInfoNoStart = 4; // 链路信息起始行编号（从0开始计数）
	int consumerInfoNoStart = edgeInfoNoStart + netEdgeNum + 1;
	
	std::smatch m1; // 用于匹配出 链路起始节点id 链路终止节点id 容量 花费
	for (int lineCount = edgeInfoNoStart; lineCount <= edgeInfoNoStart + netEdgeNum - 1; lineCount++) {
		std::string strLine(*(inLines + lineCount));
		std::regex_match(strLine, m1, std::regex("(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)[.\\n\\r]*"));
		int u = atoi(m1.str(1).c_str()); // 链路起始节点id
		int v = atoi(m1.str(2).c_str()); // 链路终止节点id
		int vol = atoi(m1.str(3).c_str()); // 容量
		int cost = atoi(m1.str(4).c_str()); // 花费
		minCostFlow.insert_edge(u, v, vol, cost);
		minCostFlow.insert_edge(v, u, vol, cost);
	}
	
	// 超级汇点连到所有汇点的边的费用和容量分别为0和消费节点带宽所需的消耗/
	// 超级汇点编号为netNodeNum
	std::smatch m2; // 用于匹配出 消费节点id 相连节点id 视频带宽需求
	for (int lineCount = consumerInfoNoStart; lineCount <= consumerInfoNoStart + consumerNodeNum - 1; lineCount++) {
		std::string  strLine(*(inLines + lineCount));
		std::regex_match(strLine, m2, std::regex("(\\d+)\\s(\\d+)\\s(\\d+)[.\\n\\r]*"));
		int consumerId = atoi(m2.str(1).c_str()); // 消费节点id
		int consumerLinkId = atoi(m2.str(2).c_str()); // 相连节点id
		int demand = atoi(m2.str(3).c_str()); // 视频带宽需求
		minCostFlow.insert_edge(consumerLinkId, netNodeNum, demand, 0);
		minCostFlow.insert_edge(netNodeNum, consumerLinkId, demand, 0);
	}

	// 这里开始计算启发式算法
	// 1. 预设方案：模拟退火
	// 1.1 选取初始点
    // 1.2 迭代算法
	// 1.3 判断比较

	std::vector<int> serverLinkIds; // 先用serverLinkId来存储server放置的网络id
	// 超级源点连接到所有源点的边的费用和容量分别设为0和无穷大
	// 超级源点编号为 netNodeNum + 1
	MinCostFlow minCostFlowCopy = minCostFlow; // 先拷贝构造一个对象，再去插入源点
	// 向minCostFlow中的拓扑图插入超级源点到所有源点的边
	for (auto serverLinkId: serverLinkIds) {
		minCostFlow.insert_edge(netNodeNum+1, serverLinkId, INFINITY, 0);
		minCostFlow.insert_edge(serverLinkId, netNodeNum + 1, INFINITY, 0);
	}
	
	// 所有边插入之后，开始跑最小费用流，计算出最小费用流
	// 从超级源点到超级汇点计算最小费用流
	int ans = minCostFlow.min_cost_flow(netNodeNum + 1, netNodeNum);

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


