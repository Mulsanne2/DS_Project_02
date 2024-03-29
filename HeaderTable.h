#pragma once
#include "FPNode.h"
#include <list>

class HeaderTable
{
private:
	list<pair<int, string>> indexTable;
	map<string, FPNode*> dataTable;
public:
	HeaderTable() { }
	~HeaderTable();
	void insertTable(char* item, int frequency);
	void makeDataTable(); //make data table from index table
	list<pair<int, string>> getindexTable() { return indexTable; }
	map<string, FPNode*> getdataTable() { return dataTable; }
	FPNode* getNode(string item) { return dataTable.find(item)->second; }
	void descendingIndexTable() { indexTable.sort(greater<pair<int, string>>()); }
	void ascendingIndexTable() { indexTable.sort(); }
	int find_frequency(string item);
	bool Find_Item(string item);
	void Increase_Frequency(string item);
	void insertNode(string item, FPNode *newNode); //connect data table map with FPNode
};
