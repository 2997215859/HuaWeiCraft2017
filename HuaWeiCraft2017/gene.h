#ifndef GENE_H
#define GENE_H
#include <bitset>
#include <unordered_set>

class Gene {
private:
	int len;
	std::bitset<10000 + 5> code;
public:
	double fitness; // 适应度
	double p; // 选中概率
	
	Gene(int len);
	inline void randomInit();
	inline void randomInit(int len);
	void operator*(Gene &b);
	inline bool operator<(const Gene &b) const;
	inline void operator=(const Gene &b);// 赋值
	inline bool operator==(const Gene &b) const; // 判断序列是否相等
	inline void Gene::mutation(int loc = -1);// 突变，[0, len)
	inline bool getBit(int loc);
	inline void setBit(int loc, bool x);

	inline std::unordered_set<int> get_server_linked_ids() const;
};
#endif // !GENE_H
