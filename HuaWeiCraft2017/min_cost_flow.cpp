#include "min_cost_flow.h"
#include <queue>
/*
* 假设图中不存在负权和环，SPFA算法找到最短路径
* 最短路径指的是，从源点s到终点t所经过边的cost之和最小的路径
*/
void MinCostFlow::insert_edge(int u, int v, int vol, int cost) {
	gEdges[gEdgeCount].to = v;
	gEdges[gEdgeCount].vol = vol;
	gEdges[gEdgeCount].cost = cost;
	gEdges[gEdgeCount].next = gHead[u];
	gHead[u] = gEdgeCount++;

	gEdges[gEdgeCount].to = v;
	gEdges[gEdgeCount].vol = 0;
	gEdges[gEdgeCount].cost = -cost;
	gEdges[gEdgeCount].next = gHead[v];
	gHead[v] = gEdgeCount++;
}
bool MinCostFlow::spfa(int s, int t) {
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