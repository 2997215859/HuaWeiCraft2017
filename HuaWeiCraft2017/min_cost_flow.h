#ifndef MIN_COST_FLOW_H
#define MIN_COST_FLOW_H
#define int MAX_NODE_NUM 1005
#define int MAX_EDGE_NUM 40005

#include "edge.h"
#include <vector>

class MinCostFlow {
	
	std::vector<Edge> gEdges;

	std::vector<int> gHead(MAX_NODE_NUM, -1);
	std::vector<int> gPre(MAX_NODE_NUM, -1);
	std::vector<int> gPath(MAX_NODE_NUM, -1);
	std::vector<int> gDist(MAX_NODE_NUM, -1);

	int gEdgeCount;

	void insert_edge(int u, int v, int vol, int cost);
	bool spfa(int s, int t);
	int min_cost_flow(int s, int t);
};

#endif // MIN_COST_FLOW_H