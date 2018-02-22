#include "saa.h"
#include <ctime>
double SAA::rnd(double dbLow, double dbUpper){
	double dbTemp = rand() / ((double)RAND_MAX + 1.0);
	return dbLow + dbTemp*(dbUpper - dbLow);
}
void SAA::saa(MinCostFlow minCostFlow, std::vector<int> serverLinkIds) {
	int i;
	double T = tInit;
	MinCostFlow minCostFlowBest = minCostFlow; // ��������һ���µ���Ϊ׹����ز�ͼ
	while (T > tMin) {
		for (int i = 0; i < num; i++) {
			time_t tm;
			srand((unsigned int)time(&tm));  // ��ʼ����������� 
			// �����µķ������ڵ�,Ȼ�������С������������ȥ�������С����
			std::vector<int> currentServerLinkIds; // to do
			MinCostFlow currentMinCostFlow = minCostFlow;
			double dE = currentMinCostFlow.min_cost(currentServerLinkIds) - minCostFlowBest.min_cost(serverLinkIds);
			if (dE < 0) { // �ƶ���õ����õĽ⣬�����ƶ�
				minCostFlowBest = currentMinCostFlow;
			} else if (exp(-dE / (T*k)) > rnd(0.0, 1.0)) {
				// ��һ���������ܾ����������������ܾ��ĸ��ʻ������¶ȵĽ��ͣ�Ҳ���ǵ������������ӣ������Ҳ���ǽ��ܵĸ��ʻ�Խ��ԽС��
				// ����exp( dE/T )��ȡֵ��Χ��(0,1) ��dE/TԽ����exp( dE/T )ҲԽ��  
				minCostFlowBest = currentMinCostFlow;
			}
		}
		T = r * T; //�����˻� ��0<r<1 ��rԽ�󣬽���Խ����rԽС������Խ��  
	}
}