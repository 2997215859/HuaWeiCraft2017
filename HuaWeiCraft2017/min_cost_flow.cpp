#include "min_cost_flow.h"
#include "random.h"
#include <queue>
#include <climits>

/*
* ����ͼ�в����ڸ�Ȩ�ͻ���SPFA�㷨�ҵ����·��
* ���·��ָ���ǣ���Դ��s���յ�t�������ߵ�cost֮����С��·��
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
	int delete_num = serverLinkIds.size() * 4; // ��Ҫɾ����ô������
	for (int i = 0; i < delete_num;i++) { // ����ָ�ÿ������ָ��ı�
		gHead[gEdges.back().from] = gEdges.back().next;
		gEdges.pop_back();
	}
	gEdgeCount -= delete_num; // ���ָ���·����Ϊ���볬��Դ��֮ǰ����·����
	// ��ʱ��gEdges��gOriginEdgesֻ��vol��һ���ˣ����¸�ֵ����
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

	if (gPre[t] == -1)  //���յ�tû������pre��˵�������ڵ����յ�t��·��
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
			gEdges[gPath[u]].vol -= f; // �������������
			gEdges[gPath[u]^1].vol += f; // �������������
		}
	}
	return {cost, flow};
}

std::pair<int, int> MinCostFlow::min_cost_flow() {
	return min_cost_flow(netNodeNum + 1, netNodeNum);
}

int MinCostFlow::min_cost(const std::vector<int> &serverLinkIds) {
	if (this->serverLinkIds.empty()) { // ��һ���ܷ����������治��������Դ��ıߵ�gEdges;
		gOriginEdges = gEdges;
	}
	if (!this->serverLinkIds.empty()) {
		// ��ɾ��������Դ���Ӧ�ı�
		// ע�⣬�ڹ�������ͼ��ʱ��һ�����������ӳ���Դ��ߣ���Ϊ�˷���ɾ��
		delete_super_source();
	}

	this->serverLinkIds = serverLinkIds;
	insert_server(serverLinkIds);
	std::pair<int, int> cost_flow = min_cost_flow();
	//printf("cost=%d,flow=%d,demandSum=%d", cost_flow.first, cost_flow.second, demandSum);
	if (cost_flow.second < demandSum) { // ���������������
		return -1;
	}
	return cost_flow.first + serverCost * this->serverLinkIds.size();
}

int MinCostFlow::min_cost(Gene gene) {
	return min_cost(gene.get_server_linked_ids());
}

void MinCostFlow::insert_server(const std::vector<int> &serverLinkIds) {
	// ����Դ�����ӵ�����Դ��ıߵķ��ú������ֱ���Ϊ0�������
	// ����Դ����Ϊ netNodeNum + 1

    // ��minCostFlow�е�����ͼ���볬��Դ�㵽����Դ��ı�
	for (auto serverLinkId : serverLinkIds) {
		insert_edge(netNodeNum + 1, serverLinkId, INT_MAX, 0);
		insert_edge(serverLinkId, netNodeNum + 1, INT_MAX, 0);
	}

}

