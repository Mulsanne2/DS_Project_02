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

	// ~Manager()//destructor
	// {
	// 	/* You must fill here */
	// }

	ifstream fin;
	ofstream flog;
	

	void run(const char* command);
	bool LOAD();
	bool BTLOAD();
	bool PRINT_ITEMLIST();
	bool PRINT_FPTREE();
	bool PRINT_BPTREE();
	// bool PRINT_RANGE(char* item, int start, int end);
	// bool PRINT_CONFIDENCE(char* item, double rate);
	

	void printErrorCode(string str, int n);
	void printSuccessCode(string str);
};

