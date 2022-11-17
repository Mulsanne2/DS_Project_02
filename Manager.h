#pragma once
#include <iostream>
#include <fstream>
#include "FPGrowth.h"
#include "BpTree.h"
using namespace std;

class Manager
{
private:
	char* cmd; //to read each command
	FPGrowth* fpgrowth;
	BpTree* bptree;
	int threshold;

public:
	Manager(int threshold, int bpOrder){
		fpgrowth = new FPGrowth(&flog, threshold);
		this->threshold = threshold;
		bptree = new BpTree(&flog, bpOrder);
	}

	~Manager()//destructor
	{
		fin.close(); //close ifstream
		flog.close(); //close ofstream
		delete fpgrowth; //delete FpTree and HeaderTable
		delete bptree; //delete BpTree
	}

	ifstream fin;
	ofstream flog;
	

	void run(const char* command);
	bool LOAD();
	bool BTLOAD();
	bool PRINT_ITEMLIST();
	bool PRINT_FPTREE();
	bool PRINT_BPTREE();
	bool PRINT_RANGE();
	bool PRINT_CONFIDENCE();
	void EXIT();

	void printErrorCode(string str, int n);
	void printSuccessCode(string str);
};

