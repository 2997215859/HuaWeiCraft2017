#include "saa.h"
#include "random.h"
#include "gene.h"
#include <ctime>
double SAA::rnd(double dbLow, double dbUpper){
	double dbTemp = rand() / ((double)RAND_MAX + 1.0);
	return dbLow + dbTemp*(dbUpper - dbLow);
}
void SAA::saa(MinCostFlow minCostFlow, std::vector<int> serverLinkedIds){
	double T = 20.0;double poi = 0.05;double delta = 0.999;
	int geneNum = 26; double crossP = 0.95; double mutation = 0.15;
	
	// 初始化种群
	std::vector<Gene> genes;
	genes.push_back(Gene(minCostFlow.get_net_node_num(), serverLinkedIds));
	for (int i = 1; i <= geneNum; i++) {
		genes.push_back(Gene(minCostFlow.get_net_node_num()));
	}

	// 退火算法
	while (T > 0.1) {
		// 对每条染色体进行邻域选择
		// 如果邻域解更优，则保留，否则一定概率接收
		// 如果本就无解，则适度函数无穷大，设为一个很大的值
		for (auto gene: genes) {
			if (gene.none()) continue; // 空染色体不进行领域操作
			MinCostFlow currentMinCostFlow = minCostFlow;
			std::vector<int> currentServerLinkedIds = gene.get_server_linked_ids();
			// 记录该染色体的费用
			int currentCost = currentMinCostFlow.min_cost(currentServerLinkedIds);		

			// 开始计算邻域
			// 任意选择服务器中的一个，并任意选择其相邻节点中的一个节点
			// 从currentMinCostFlow中找到			
			int index = Random::rnd(0, currentServerLinkedIds.size()-1);
			// 依靠minCostFlow中的拓扑结构，任意挑选该服务器点周围的一个点作为新的服务器点，原服务器点舍弃
			int newServerId = minCostFlow.selectRandAdjacent(currentServerLinkedIds[index]);
			
			// 替换原来的服务器节点
			std::vector<int> newServerLinkedIds = currentServerLinkedIds;
			newServerLinkedIds[index] = newServerId;

			// 概率性的随机增加任意节点为服务器节点
			if (Random::random_double(0, 1) < poi) {
				newServerLinkedIds.push_back(Random::random_int(0, minCostFlow.get_net_node_num));
			}
			// 计算邻域完毕！
			MinCostFlow adjacentMinCostFlow = minCostFlow;
			
		}
	}
}
