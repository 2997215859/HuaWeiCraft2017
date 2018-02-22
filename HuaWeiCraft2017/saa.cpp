#include "saa.h"
#include <ctime>
double SAA::rnd(double dbLow, double dbUpper){
	double dbTemp = rand() / ((double)RAND_MAX + 1.0);
	return dbLow + dbTemp*(dbUpper - dbLow);
}
void SAA::saa(MinCostFlow minCostFlow, std::vector<int> serverLinkIds) {
	int i;
	double T = tInit;
	MinCostFlow minCostFlowBest = minCostFlow; // 拷贝构造一个新的作为坠吼的拓补图
	while (T > tMin) {
		for (int i = 0; i < num; i++) {
			time_t tm;
			srand((unsigned int)time(&tm));  // 初始化随机数种子 
			// 产生新的服务器节点,然后放入最小费用流对象中去计算出最小费用
			std::vector<int> currentServerLinkIds; // to do
			MinCostFlow currentMinCostFlow = minCostFlow;
			double dE = currentMinCostFlow.min_cost(currentServerLinkIds) - minCostFlowBest.min_cost(serverLinkIds);
			if (dE < 0) { // 移动后得到更好的解，接收移动
				minCostFlowBest = currentMinCostFlow;
			} else if (exp(-dE / (T*k)) > rnd(0.0, 1.0)) {
				// 以一个概率来拒绝或接受它，而这个拒绝的概率会随着温度的降低（也即是迭代次数的增加）而变大（也就是接受的概率会越来越小）
				// 函数exp( dE/T )的取值范围是(0,1) ，dE/T越大，则exp( dE/T )也越大  
				minCostFlowBest = currentMinCostFlow;
			}
		}
		T = r * T; //降温退火 ，0<r<1 。r越大，降温越慢；r越小，降温越快  
	}
}