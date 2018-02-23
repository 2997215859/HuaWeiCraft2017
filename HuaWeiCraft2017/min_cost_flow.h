#ifndef MIN_COST_FLOW_H
#define MIN_COST_FLOW_H


#include "edge.h"
#include "gene.h"
#include "random.h"
#include <vector>

class MinCostFlow {
	Random random;
	int netNodeNum;
	int netEdgeNum;
	int consumerNodeNum;
	int serverCost;

	int gEdgeCount;

	int demandSum;

	std::vector<Edge> gEdges;

	std::vector<int> gHead;
	std::vector<int> gPre;
	std::vector<int> gPath;
	std::vector<int> gDist;

	std::vector<int> serverLinkIds;

	std::pair<int, int> min_cost_flow(int s, int t);
	std::pair<int, int> min_cost_flow();

	
public:
	MinCostFlow(int netNodeNum, int netEdgeNum, int consumerNodeNum, int serverCost);
	MinCostFlow(MinCostFlow &m);
	void insert_edge(int u, int v, int vol, int cost);
	void insert_server(const std::vector<int> &serverLinkedIds);
	void delete_super_source();
	bool spfa(int s, int t);
	
	inline void set_demand_sum(int demandSum) { this->demandSum = demandSum; }
	inline int get_demand_sum() { return demandSum; }
	inline int get_net_node_num() {return netNodeNum;}
	inline int get_server_cost() { return serverCost; }
	inline int get_consumer_node_num() { return consumerNodeNum; }
	
	int min_cost(const std::vector<int> &serverLinkIds); // 加上服务器之后的总共花费
	int min_cost(Gene gene); // 传入gene

	int selectRandAdjacent(int u); // 随机选取u周围的节点
	std::vector<int> MinCostFlow::selectAdjacent(int u); // 获取u周围的节点
};

#endif // MIN_COST_FLOW_H