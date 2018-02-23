#include "gene.h"
#include "random.h"
#include <ctime>

Gene::Gene(int len) : len(len) {
	randomInit();
}
Gene::Gene(int len, std::vector<int> serverLinkedIds) {
	this->len = len;
	Gene::set(serverLinkedIds);
}
void Gene::randomInit() {
	for (int i = 0; i < len; i++) {
		code[i] = random.random_int(0, 1);
	}
}
void Gene::randomInit(int len) {
	this->len = len;
	Gene::randomInit();
}
void Gene::operator*(Gene &b) { // ���棬ͬʱ�ı�2��Ⱦɫ��
	int end = random.random_int(1, len); // ������λ�ã�����һ�߾����ˣ���Ϊ��һ�߲��������ｻ������
	int begin = random.random_int(0, end - 1);
	// printf("begin = %d end = %d len = %d\n", begin, end, len);
	for (int i = begin; i < end; ++i)
		if (code[i] != b.code[i]) {
			code[i] = !code[i];
			b.code[i] = !b.code[i];
		}
}

bool Gene::operator<(const Gene &b) const {
	return fitness < b.fitness;
}

void Gene::operator=(const Gene &b) { // ��ֵ
	len = b.len;
	fitness = b.fitness;
	p = b.p;
	code = b.code;
}
bool Gene::operator==(const Gene &b) const { // �ж������Ƿ����
	return code == b.code;
}

void Gene::mutation(int loc) { // ͻ�䣬[0, len)
	if (loc == -1) loc = random.random_int(0, len - 1);
	else if (loc >= len) return;
	// printf("loc = %d\n", loc);
	code[loc] = !code[loc];
}

std::vector<int> Gene::get_server_linked_ids() const {
	std::vector<int> serverLinkedIds;
	for (int i = 0; i < len; ++i)
		if (code[i]) serverLinkedIds.push_back(i);
	return serverLinkedIds;
}
void Gene::set(std::vector<int> serverLinkedIds) {
	code.reset();
	for (auto i: serverLinkedIds) {
		code.set(i);
	}
}
bool Gene::getBit(int loc) {
	return code[loc];
}
void Gene::setBit(int loc, bool x) {
	code[loc] = x;
}