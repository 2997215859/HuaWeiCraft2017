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
	
	// ��ʼ����Ⱥ
	std::vector<Gene> genes;
	genes.push_back(Gene(minCostFlow.get_net_node_num(), serverLinkedIds));
	for (int i = 1; i <= geneNum; i++) {
		genes.push_back(Gene(minCostFlow.get_net_node_num()));
	}

	// �˻��㷨
	while (T > 0.1) {
		// ��ÿ��Ⱦɫ���������ѡ��
		// ����������ţ�����������һ�����ʽ���
		// ��������޽⣬���ʶȺ����������Ϊһ���ܴ��ֵ
		for (auto gene: genes) {
			if (gene.none()) continue; // ��Ⱦɫ�岻�����������
			MinCostFlow currentMinCostFlow = minCostFlow;
			std::vector<int> currentServerLinkedIds = gene.get_server_linked_ids();
			// ��¼��Ⱦɫ��ķ���
			int currentCost = currentMinCostFlow.min_cost(currentServerLinkedIds);		

			// ��ʼ��������
			// ����ѡ��������е�һ����������ѡ�������ڽڵ��е�һ���ڵ�
			// ��currentMinCostFlow���ҵ�			
			int index = Random::rnd(0, currentServerLinkedIds.size()-1);
			// ����minCostFlow�е����˽ṹ��������ѡ�÷���������Χ��һ������Ϊ�µķ������㣬ԭ������������
			int newServerId = minCostFlow.selectRandAdjacent(currentServerLinkedIds[index]);
			
			// �滻ԭ���ķ������ڵ�
			std::vector<int> newServerLinkedIds = currentServerLinkedIds;
			newServerLinkedIds[index] = newServerId;

			// �����Ե������������ڵ�Ϊ�������ڵ�
			if (Random::random_double(0, 1) < poi) {
				newServerLinkedIds.push_back(Random::random_int(0, minCostFlow.get_net_node_num));
			}
			// ����������ϣ�
			MinCostFlow adjacentMinCostFlow = minCostFlow;
			
		}
	}
}
