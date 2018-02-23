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
	
	int minCost = INFINITY;
	std::vector<int> bestServerLinkedIds;
	// ��ʼ����Ⱥ
	std::vector<Gene> genes;
	std::vector<Gene> next_genes; // ���̶�������һ��
	genes.push_back(Gene(minCostFlow.get_net_node_num(), serverLinkedIds));
	for (int i = 1; i < geneNum; i++) {
		genes.push_back(Gene(minCostFlow.get_net_node_num()));
	}

	// �˻��㷨
	while (T > 0.1) {
		int bestCost; // ��¼����Ⱦɫ���е���С����
		// ��ÿ��Ⱦɫ���������ѡ��
		// ����������ţ�����������һ�����ʽ���
		// ��������޽⣬���ʶȺ����������Ϊһ���ܴ��ֵ
		for (auto gene: genes) {
			if (gene.none()) continue; // ��Ⱦɫ�岻�����������
			std::vector<int> currentServerLinkedIds = gene.get_server_linked_ids();
			// ��¼��Ⱦɫ��ķ���
			int currentCost = minCostFlow.min_cost(currentServerLinkedIds);

			// ��ʼ��������
			// ����ѡ��������е�һ����������ѡ�������ڽڵ��е�һ���ڵ�
			// ��currentMinCostFlow���ҵ�			
			int index = random.random_int(0, currentServerLinkedIds.size()-1);
			// ����minCostFlow�е����˽ṹ��������ѡ�÷���������Χ��һ������Ϊ�µķ������㣬ԭ������������
			int newServerId = minCostFlow.selectRandAdjacent(currentServerLinkedIds[index]);
			
			// �滻ԭ���ķ������ڵ�
			std::vector<int> newServerLinkedIds = currentServerLinkedIds;
			newServerLinkedIds[index] = newServerId;

			// �����Ե������������ڵ�Ϊ�������ڵ�
			if (random.random_double(0, 1) < poi) {
				newServerLinkedIds.push_back(random.random_int(0, minCostFlow.get_net_node_num()));
			}
			// ����������ϣ�
			int newCost = minCostFlow.min_cost(newServerLinkedIds);
			
			if (newCost != -1) {
				// �����н⣬������Խ���
				int dE = newCost - currentCost;
				if (currentCost == -1 || std::min<double>(1, exp(-dE / T)) > random.random_double(0, 1)) {
					gene.set(newServerLinkedIds);
					gene.set_fitness(newCost);
					// �ж��Ƿ�ǰ����
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
			else { // �����޽⣬ֱ�Ӿܾ�����Ľ�
				if (currentCost != -1 && bestCost > currentCost) { // ��ǰ���н��Ҹ���
					bestCost = currentCost;
					bestServerLinkedIds = currentServerLinkedIds;
				}
				gene.set_fitness(currentCost == -1? INFINITY: currentCost);
			}
			
		}

		// ��һ��������Ӧ��
		double sum = 0;
		if (bestCost == INFINITY) bestCost = 0;
		for (auto gene: genes) {
			double dE = gene.get_fitness() - bestCost;
			gene.set_fitness(exp(-dE / T)); // ����Խ�࣬��ѡ�����ԽС
			sum += gene.get_fitness();
		}

		// ����ѡ�����
		for (auto gene: genes) {
			gene.set_p(gene.get_fitness() / sum);
		}

		// ���̶�ѡ��������һ��
		// ���澫Ӣ
		next_genes.push_back(Gene(minCostFlow.get_net_node_num(), bestServerLinkedIds));
		for (int i = 1; i < geneNum; i++ ) {
			next_genes.push_back(genes[select(genes)]);
		}
		
		genes = next_genes;
		next_genes.clear(); // ���̶����

		// ���棬��ʼ
		for (int i = 0; i < geneNum;i+=2) {
			if (random.random_double(0, 1) < crossP) {
				genes[i] * genes[i + 1];
			}
		}

		// ���죬��ʼ
		for (auto gene: genes) {
			if (random.random_double(0, 1) < mutation) {
				gene.mutation();
			}
		}
		// �������

		// �����¼���Ž�
		if (bestCost != 0) minCost = std::min(minCost, bestCost);
	}
}
