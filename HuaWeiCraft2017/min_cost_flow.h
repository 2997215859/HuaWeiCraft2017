#ifndef MIN_COST_FLOW_H
#define MIN_COST_FLOW_H

#include "edge.h"
#include <vector>

class MinCostFlow {
	std::vector<Edge> gEdges;

	std::vector<int> gHead;
	std::vector<int> gPre;
	std::vector<int> gPath;
	std::vector<int> gDist;

	int gEdgeCount;

	void insert_edge(int u, int v, int vol, int cost);
	bool spfa(int s, int t);
	int min_cost_flow(int s, int t);
};

#endif // MIN_COST_FLOW_H