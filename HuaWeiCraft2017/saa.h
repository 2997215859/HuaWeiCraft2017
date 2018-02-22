#ifndef SAA_H
#define SAA_H
#include "min_cost_flow.h"
class SAA {
	double k = 0.1;
	double r = 0.9; // 用于控制降温的快慢
	double tInit = 2000; // 系统的温度，系统初始应该要处于一个高温的状态
	double tMin = 20; // 温度的下限，若温度T达到tMin，则停止搜索
	int num = 30000; // 迭代次数
public:
	void saa(MinCostFlow minCostFlow, std::vector<int> serverIds);
	double rnd(double dbLow, double dbUpper);
};

#endif // !SAA_H
