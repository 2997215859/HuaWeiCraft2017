#ifndef MIN_COST_FLOW_H
#define MIN_COST_FLOW_H
#define MAX_NODE_NUM 1005
#define MAX_EDGE_NUM 40005

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

	std::vector<Edge> gEdges;

	std::vector<int> gHead;
	std::vector<int> gPre;
	std::vector<int> gPath;
	std::vector<int> gDist;

	std::vector<int> serverLinkIds;

	int min_cost_flow(int s, int t);
	int min_cost_flow();

	
public:
	MinCostFlow(int netNodeNum, int netEdgeNum, int consumerNodeNum, int serverCost);
	MinCostFlow(MinCostFlow &m);
	void insert_edge(int u, int v, int vol, int cost);
	void insert_server(const std::vector<int> &serverLinkedIds);
	void delete_super_source();
	bool spfa(int s, int t);
	
	inline int get_net_node_num() {return netNodeNum;}
	inline int get_server_cost() { return serverCost; }
	
	int min_cost(const std::vector<int> &serverLinkIds); // ���Ϸ�����֮����ܹ�����
	int min_cost(Gene gene); // ����gene

	int selectRandAdjacent(int u); // ���ѡȡu��Χ�Ľڵ�
	std::vector<int> MinCostFlow::selectAdjacent(int u); // ��ȡu��Χ�Ľڵ�
};

#endif // MIN_COST_FLOW_H