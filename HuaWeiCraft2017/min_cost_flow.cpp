#include "min_cost_flow.h"
#include <queue>
#include <climits>
/*
* ����ͼ�в����ڸ�Ȩ�ͻ���SPFA�㷨�ҵ����·��
* ���·��ָ���ǣ���Դ��s���յ�t�������ߵ�cost֮����С��·��
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
			gEdges[gPath[u]].vol -= f; // �������������
			gEdges[gPath[u]^1].vol += f; // �������������
		}
	}
}