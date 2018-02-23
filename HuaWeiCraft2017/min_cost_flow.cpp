#include "min_cost_flow.h"
#include "random.h"
#include <queue>
#include <climits>

/*
* 假设图中不存在负权和环，SPFA算法找到最短路径
* 最短路径指的是，从源点s到终点t所经过边的cost之和最小的路径
*/
MinCostFlow::MinCostFlow(int netNodeNum, int netEdgeNum, int consumerNodeNum, int serverCost) :
	netNodeNum(netNodeNum),
	netEdgeNum(netEdgeNum),
	consumerNodeNum(consumerNodeNum),
	serverCost(serverCost),
	gHead(netNodeNum+2, -1),
	gPre(netNodeNum+2, -1),
	gPath(netNodeNum+2, -1),
	gDist(netNodeNum+2, -1) {}

MinCostFlow::MinCostFlow(MinCostFlow & m) = default;

std::vector<int> MinCostFlow::selectAdjacent(int u) {
	std::vector<int> adjacentIds;
	for (int e = gHead[u]; e != -1; e = gEdges[e].next) {
		adjacentIds.push_back(gEdges[e].to);
	}
	return adjacentIds;
}

int MinCostFlow::selectRandAdjacent(int u) {
	std::vector<int> adjacentIds = selectAdjacent(u);
	return adjacentIds[Random::random_int(0, adjacentIds.size()-1)];
}

void MinCostFlow::insert_edge(int u, int v, int vol, int cost) {
	Edge e = {u, v, vol, cost, gHead[u]};
	gEdges.push_back(e);
	gHead[u] = gEdgeCount++;

	Edge reverseE = {v, u, 0, -cost, gHead[v]};
	gEdges.push_back(reverseE);
	gHead[v] = gEdgeCount++;
}

void MinCostFlow::delete_super_source() {
	int delete_num = serverLinkIds.size() * 4; // 需要删除这么多条边
	for (int i = 0; i < delete_num;i++) { // 逐个恢复每个点所指向的边
		gHead[gEdges.back().from] = gEdges.back().next;
		gEdges.pop_back();
	}
	gEdgeCount -= delete_num; // 并恢复链路计数为加入超级源点之前的链路计数
	// 此时，gEdges与gOriginEdges只有vol不一样了，重新赋值即可
	gEdges = gOriginEdges;
}

bool MinCostFlow::spfa(int s, int t) {
	gPre = std::vector<int>(netNodeNum + 2, -1);
	gDist = std::vector<int>(netNodeNum + 2, INT_MAX);
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
	}

	if (gPre[t] == -1)  //若终点t没有设置pre，说明不存在到达终点t的路径
		return false;
	return true;
}

std::pair<int, int> MinCostFlow::min_cost_flow(int s, int t) {
	int cost = 0;
	int flow = 0;
	while (spfa(s, t)) {
		int f = INT_MAX;
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
	return {cost, flow};
}

std::pair<int, int> MinCostFlow::min_cost_flow() {
	return min_cost_flow(netNodeNum + 1, netNodeNum);
}

int MinCostFlow::min_cost(const std::vector<int> &serverLinkIds) {
	if (this->serverLinkIds.empty()) { // 第一次跑费用流，保存不包含超级源点的边的gEdges;
		gOriginEdges = gEdges;
	}
	if (!this->serverLinkIds.empty()) {
		// 先删除掉超级源点对应的边
		// 注意，在构建拓扑图的时候，一定是在最后添加超级源点边，是为了方便删除
		delete_super_source();
	}

	this->serverLinkIds = serverLinkIds;
	insert_server(serverLinkIds);
	std::pair<int, int> cost_flow = min_cost_flow();
	//printf("cost=%d,flow=%d,demandSum=%d", cost_flow.first, cost_flow.second, demandSum);
	if (cost_flow.second < demandSum) { // 最大流不满足需求
		return -1;
	}
	return cost_flow.first + serverCost * this->serverLinkIds.size();
}

int MinCostFlow::min_cost(Gene gene) {
	return min_cost(gene.get_server_linked_ids());
}

void MinCostFlow::insert_server(const std::vector<int> &serverLinkIds) {
	// 超级源点连接到所有源点的边的费用和容量分别设为0和无穷大
	// 超级源点编号为 netNodeNum + 1

    // 向minCostFlow中的拓扑图插入超级源点到所有源点的边
	for (auto serverLinkId : serverLinkIds) {
		insert_edge(netNodeNum + 1, serverLinkId, INT_MAX, 0);
		insert_edge(serverLinkId, netNodeNum + 1, INT_MAX, 0);
	}

}

