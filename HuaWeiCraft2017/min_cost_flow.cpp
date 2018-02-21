#include "min_cost_flow.h"
#include <queue>
#include <climits>
/*
* 假设图中不存在负权和环，SPFA算法找到最短路径
* 最短路径指的是，从源点s到终点t所经过边的cost之和最小的路径
*/
MinCostFlow::MinCostFlow(int netNodeNum, int netEdgeNum, int consumerNodeNum) :
	netNodeNum(netNodeNum),
	netEdgeNum(netEdgeNum),
	consumerNodeNum(consumerNodeNum),
	gHead(netNodeNum, -1),
	gPre(netNodeNum, -1),
	gPath(netNodeNum, -1),
	gDist(netNodeNum, -1) {}

MinCostFlow::MinCostFlow(MinCostFlow & m) = default;

void MinCostFlow::insert_edge(int u, int v, int vol, int cost) {
	Edge e = {v, vol, cost, gHead[u]};
	gEdges.push_back(e);
	gHead[u] = gEdgeCount++;

	Edge reverseE = {v, 0, -cost, gHead[u]};
	gEdges.push_back(reverseE);
	gHead[v] = gEdgeCount++;
}
bool MinCostFlow::spfa(int s, int t) {
	gDist = std::vector<int>(MAX_NODE_NUM, -1);
	gPre = std::vector<int>(MAX_NODE_NUM, -1);
	gDist[s] = 0;
	std::queue<int> q;
	q.push(s);
	while (!q.empty()) {
		int u = q.front();
		q.pop();

		for (int e = gHead[u]; e != -1; e = gEdges[e].next) {
			int v = gEdges[e].to;
			if (gEdges[e].vol > 0 && gDist[u] + gEdges[e].cost < gDist[v]) {
				gDist[v] = gDist[u] + gEdges[e].cost;
				gPre[v] = u;
				gPath[v] = e;
				q.push(v);
			}
		}
		if (gPre[t] == -1) {
			return false;
		}
		return true;
	}
}

int MinCostFlow::min_cost_flow(int s, int t) {
	int cost = 0;
	int flow = 0;
	while (spfa(s, t)) {
		int f = INFINITY;
		for (int u = t; u != s; u = gPre[u]) {
			if (gEdges[gPath[u]].vol < f) {
				f = gEdges[gPath[u]].vol;
			}
		}
		flow += f;
		cost += gDist[t] * f;
		for (int u = t; u != s; u = gPre[u]) {
			gEdges[gPath[u]].vol -= f; // 正向边容量减少
			gEdges[gPath[u]^1].vol += f; // 反向边容量增加
		}
	}
	return cost;
}

int MinCostFlow::min_cost_flow() {
	return min_cost_flow(netNodeNum + 1, netNodeNum);
}