#ifndef MIN_COST_FLOW_H
#define MIN_COST_FLOW_H
#define MAX_NODE_NUM 1005
#define MAX_EDGE_NUM 40005

#include "edge.h"
#include <vector>

class MinCostFlow {
	
	int netNodeNum;
	int netEdgeNum;
	int consumerNodeNum;

	int gEdgeCount;

	std::vector<Edge> gEdges;

	std::vector<int> gHead;
	std::vector<int> gPre;
	std::vector<int> gPath;
	std::vector<int> gDist;

	
public:
	MinCostFlow(int netNodeNum, int netEdgeNum, int consumerNodeNum);
	MinCostFlow(MinCostFlow &m);
	void insert_edge(int u, int v, int vol, int cost);
	bool spfa(int s, int t);
	int min_cost_flow(int s, int t);
	int min_cost_flow();
};

#endif // MIN_COST_FLOW_H