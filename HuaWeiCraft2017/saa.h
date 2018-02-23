#ifndef SAA_H
#define SAA_H
#include "min_cost_flow.h"
#include "random.h"
#include <bitset>

class SAA {
	double k = 0.1;
	double r = 0.9; // ���ڿ��ƽ��µĿ���
	double tInit = 2000; // ϵͳ���¶ȣ�ϵͳ��ʼӦ��Ҫ����һ�����µ�״̬
	double tMin = 20; // �¶ȵ����ޣ����¶�T�ﵽtMin����ֹͣ����
	int num = 30000; // ��������
public:
	void saa(MinCostFlow minCostFlow, std::vector<int> serverLinkIds);
	double rnd(double dbLow, double dbUpper);
	int select(const std::vector<Gene>& genes);
};

#endif // !SAA_H
