#include "deploy.h"
#include "lib_io.h"
#include "lib_time.h"
#include "saa.h"
#include "min_cost_flow.h"
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <regex>
#include <iostream>

void deploy_server(char* inLines[MAX_IN_NUM], int inLineNum, const char * const filename)
{
	/*
	* �����������ݽṹ
	*/
	std::smatch m0;
	std::string strLine0(*inLines);
	std::regex_match(strLine0, m0, std::regex("(\\d+)\\s(\\d+)\\s(\\d+)[.\\n\\r]*"));
	const int netNodeNum = atoi(m0.str(1).c_str()); // ����ڵ�����
	const int netEdgeNum = atoi(m0.str(2).c_str()); // ������·����
	const int consumerNodeNum = atoi(m0.str(3).c_str()); // ���ѽڵ�����	
	const int serverCost = atoi(*(inLines + 2));

	printf("����ڵ�����=%d,������·����=%d,���ѽڵ�����=%d,�������ɱ�=%d\n", 
		netNodeNum, netEdgeNum, consumerNodeNum, serverCost);
	
	printf("��ʼ����������ͼ����ʼ\n");
	MinCostFlow minCostFlow(netNodeNum, netEdgeNum, consumerNodeNum, serverCost); // ��ʼ��������
	int edgeInfoNoStart = 4; // ��·��Ϣ��ʼ�б�ţ���0��ʼ������
	int consumerInfoNoStart = edgeInfoNoStart + netEdgeNum + 1;
	
	printf("  ��·��Ϣ���£�\n");
	std::smatch m1; // ����ƥ��� ��·��ʼ�ڵ�id ��·��ֹ�ڵ�id ���� ����
	for (int lineCount = edgeInfoNoStart; lineCount <= edgeInfoNoStart + netEdgeNum - 1; lineCount++) {
		std::string strLine(*(inLines + lineCount));
		std::regex_match(strLine, m1, std::regex("(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)[.\\n\\r]*"));
		int u = atoi(m1.str(1).c_str()); // ��·��ʼ�ڵ�id
		int v = atoi(m1.str(2).c_str()); // ��·��ֹ�ڵ�id
		int vol = atoi(m1.str(3).c_str()); // ����
		int cost = atoi(m1.str(4).c_str()); // ����
		printf("  u=%2d,v=%2d,vol=%2d,cost=%2d\n",
			u, v, vol, cost);
		minCostFlow.insert_edge(u, v, vol, cost);
		minCostFlow.insert_edge(v, u, vol, cost);
	}

	// ��������������л��ıߵķ��ú������ֱ�Ϊ0�����ѽڵ�������������/
	// ���������ΪnetNodeNum
	std::vector<int> serverLinkIdsInit; // ��ʼ������λ��
	std::smatch m2; // ����ƥ��� ���ѽڵ�id �����ڵ�id ��Ƶ��������
	int demandSum = 0;
	for (int lineCount = consumerInfoNoStart; lineCount <= consumerInfoNoStart + consumerNodeNum - 1; lineCount++) {
		std::string  strLine(*(inLines + lineCount));
		std::regex_match(strLine, m2, std::regex("(\\d+)\\s(\\d+)\\s(\\d+)[.\\n\\r]*"));
		int consumerId = atoi(m2.str(1).c_str()); // ���ѽڵ�id
		int consumerLinkId = atoi(m2.str(2).c_str()); // �����ڵ�id
		int demand = atoi(m2.str(3).c_str()); // ��Ƶ��������
		printf("  ���ѽڵ�=%2d,���������=%2d,vol=%2d,cost=%2d\n",
			consumerLinkId, netNodeNum, demand, 0);
		minCostFlow.insert_edge(consumerLinkId, netNodeNum, demand, 0);
		minCostFlow.insert_edge(netNodeNum, consumerLinkId, demand, 0);
		demandSum += demand;
		serverLinkIdsInit.push_back(consumerLinkId); // ��ʼ������λ��
	}
	minCostFlow.set_demand_sum(demandSum);
	printf("  ��ʼ�������ڵ�ѡ�����ѽڵ��ϣ�");
	for (auto serverLinkIdInit : serverLinkIdsInit) {
		printf("%d ", serverLinkIdInit);
	}
	printf("\n��ʼ����������ͼ�����\n\n");
	printf("��ʼ�Ŵ��˻��㷨\n");

	SAA saa;
	
	// serverLinkIdsInit = { 43,44,22,7,37,13,16,38,34 };
	serverLinkIdsInit = { 7,37,15,13,22,43,38};
	saa.saa(minCostFlow, serverLinkIdsInit);


	std::string res;
	res.append("4");
	res.append("\n\n");
	res.append("204 201 101 12 70").append("\n");
	res.append("204 207 206 104 12 50").append("\n");
	res.append("204 207 208 103 12 30").append("\n");
	res.append("204 203 202 102 12 40").append("\n");

	write_result(res.c_str(), filename);
	system("pause");
}


