#include "saa.h"
#include "random.h"
#include "gene.h"
#include <ctime>
#include <algorithm>
double SAA::rnd(double dbLow, double dbUpper){
	double dbTemp = rand() / ((double)RAND_MAX + 1.0);
	return dbLow + dbTemp*(dbUpper - dbLow);
}
int SAA::select(const std::vector<Gene> &genes) {
	double r = Random().random_double(0, 1);
	double s = 0;
	for (size_t i = 0; i < genes.size();i++) {
		s += genes[i].get_p();
		if (s >= r) {
			return i;
		}
	}
	return 0;
}
void SAA::saa(MinCostFlow minCostFlow, std::vector<int> serverLinkedIds){
	double T = 20.0;double poi = 0.05;double delta = 0.999;
	int geneNum = 26; double crossP = 0.95; double mutation = 0.15;

	printf("  初始温度=%f，增点概率=%f,退火速率=%f,种群大小=%d，交叉概率=%f,变异概率=%f", 
		T, poi, delta, geneNum, crossP, mutation);
	
	int minCost = INT_MAX;
	std::vector<int> bestServerLinkedIds;
	// 初始化种群
	printf(" 初始化种群\n");
	std::vector<Gene> genes;
	std::vector<Gene> next_genes; // 轮盘赌生成下一代
	genes.push_back(Gene(minCostFlow.get_net_node_num(), serverLinkedIds));
	for (int i = 1; i < geneNum; i++) {
		genes.push_back(Gene(minCostFlow.get_net_node_num()));
	}
	int iterationCnt = 0;
	printf("  开始迭代退火算法\n");
	
	// 退火算法
	while (T > 0.1) {
		int bestCost = INT_MAX; // 记录所有染色体中的最小费用
		// 对每条染色体进行邻域选择
		// 如果邻域解更优，则保留，否则一定概率接收
		// 如果本就无解，则适度函数无穷大，设为一个很大的值
		for (auto gene: genes) {
			if (gene.none()) continue; // 空染色体不进行领域操作
			std::vector<int> currentServerLinkedIds = gene.get_server_linked_ids();
			printf("\n  该染色体当前服务器节点如下：");
			for (auto currentServerLinkedId : currentServerLinkedIds) {
				printf("%d ", currentServerLinkedId);
			}
			// 记录该染色体的费用
			int currentCost = minCostFlow.min_cost(currentServerLinkedIds);
			system("pause");
			// 开始计算邻域
			// 任意选择服务器中的一个，并任意选择其相邻节点中的一个节点
			// 从currentMinCostFlow中找到			
			int index = random.random_int(0, currentServerLinkedIds.size()-1);
			// 依靠minCostFlow中的拓扑结构，任意挑选该服务器点周围的一个点作为新的服务器点，原服务器点舍弃
			// 注意不要选到超级源点和超级汇点
			int newServerId = -1;
			do {
				newServerId = minCostFlow.selectRandAdjacent(currentServerLinkedIds[index]);
			} while (newServerId >= minCostFlow.get_net_node_num());
			
			// 替换原来的服务器节点
			std::vector<int> newServerLinkedIds = currentServerLinkedIds;
			newServerLinkedIds[index] = newServerId;

			// 概率性的随机增加任意节点为服务器节点
			if (random.random_double(0, 1) < poi) {
				newServerLinkedIds.push_back(random.random_int(0, minCostFlow.get_net_node_num()));
			}
			printf("\n  该染色体邻域服务器节点如下：");
			for (auto newServerLinkedId : newServerLinkedIds) {
				printf("%d ", newServerLinkedId);
			}
			// 计算邻域完毕！
			int newCost = minCostFlow.min_cost(newServerLinkedIds);

			if (newCost != -1) {
				// 领域有解，则概率性接收
				int dE = newCost - currentCost;
				if (currentCost == -1 || std::min<double>(1, exp(-dE / T)) > random.random_double(0, 1)) {
					gene.set(newServerLinkedIds);
					gene.set_fitness(newCost);
					// 判断是否当前最优
					if (bestCost > newCost) {
						bestCost = newCost;
						bestServerLinkedIds = newServerLinkedIds;
					}
				}
				else {
					gene.set_fitness(currentCost);
					if (bestCost > currentCost) {
						bestCost = currentCost;
						bestServerLinkedIds = currentServerLinkedIds;
					}
				}
			}
			else { // 领域无解，直接拒绝领域的解
				if (currentCost != -1 && bestCost > currentCost) { // 当前域有解且更优
					bestCost = currentCost;
					bestServerLinkedIds = currentServerLinkedIds;
				}
				gene.set_fitness(currentCost == -1? INT_MAX : currentCost);
			}
			
		}

		// 进一步计算适应度
		double sum = 0;
		if (bestCost == INT_MAX) bestCost = 0;
		for (auto gene: genes) {
			double dE = gene.get_fitness() - bestCost;
			gene.set_fitness(exp(-dE / T)); // 费用越多，则被选择概率越小
			sum += gene.get_fitness();
		}

		// 计算选择概率
		for (auto gene: genes) {
			gene.set_p(gene.get_fitness() / sum);
		}

		// 轮盘赌选择，生成下一代
		// 保存精英
		next_genes.push_back(Gene(minCostFlow.get_net_node_num(), bestServerLinkedIds));
		for (int i = 1; i < geneNum; i++ ) {
			next_genes.push_back(genes[select(genes)]);
		}
		
		genes = next_genes;
		next_genes.clear(); // 轮盘赌完毕

		// 交叉，开始
		for (int i = 0; i < geneNum;i+=2) {
			if (random.random_double(0, 1) < crossP) {
				genes[i] * genes[i + 1];
			}
		}

		// 变异，开始
		for (auto gene: genes) {
			if (random.random_double(0, 1) < mutation) {
				gene.mutation();
			}
		}
		// 变异结束

		// 保存记录最优解
		if (bestCost != 0) minCost = std::min(minCost, bestCost);
		T *= delta;
		++iterationCnt;
		printf("\niterationCnt: %d, minCost: %d/%d\n\n", iterationCnt, minCostFlow.get_consumer_node_num() * minCostFlow.get_server_cost());
	}
	printf("遗传退火算法结束：T=%lf iterationCnt=%d\n", T, iterationCnt);
}
