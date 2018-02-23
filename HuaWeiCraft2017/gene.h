#pragma   once 
#ifndef GENE_H
#define GENE_H
#include "random.h"
#include <bitset>
#include <unordered_set>
#include <vector>

class Gene {
	int len;
	std::bitset<10000 + 5> code;
	double fitness; // ��Ӧ��
	double p; // ѡ�и���
public:
	Gene(int len);
	Gene(int len, std::vector<int> serverLinkedIds);
	inline void randomInit();
	inline void randomInit(int len);
	void operator*(Gene &b);
	inline bool operator<(const Gene &b) const;
	void operator=(const Gene &b);// ��ֵ
	inline bool operator==(const Gene &b) const; // �ж������Ƿ����
	void mutation(int loc = -1);// ͻ�䣬[0, len)
	inline bool getBit(int loc);
	inline void setBit(int loc, bool x);
	inline void set(std::vector<int> serverLinkedIds);
	inline bool none() { return code.none(); }
	inline bool any() { return code.any(); }
	std::vector<int> get_server_linked_ids() const;
	inline void set_fitness(double f) { fitness = f; }
	inline double get_fitness() { return fitness; }
	inline void set_p(double p) { this->p = p; }
	inline double get_p() const { return p; }
};
#endif // !GENE_H
