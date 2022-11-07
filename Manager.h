#pragma once
#include <iostream>
#include <fstream>
#include "FPGrowth.h"
// #include "BpTree.h"
using namespace std;

class Manager
{
private:
	char* cmd; //to read each command
	FPGrowth* fpgrowth;
	// BpTree* bptree;
	int threshold;

public:
	// Manager(int threshold, int bpOrder)	//constructor  bpOrder은 bptree를 위한것이니 일단 냅두자.
	// {
	// 	/* You must fill here */
	// }

	/*나중에 지우기!!*/
	Manager(int threshold){
		fpgrowth = new FPGrowth(&flog, threshold);
		this->threshold = threshold;
	}

	// ~Manager()//destructor
	// {
	// 	/* You must fill here */
	// }

	ifstream fin;
	ofstream flog;
	

	void run(const char* command);
	bool LOAD();
	// bool BTLOAD();
	
	bool PRINT_ITEMLIST();
	bool PRINT_FPTREE();

	// bool PRINT_RANGE(char* item, int start, int end);
	// bool PRINT_CONFIDENCE(char* item, double rate);
	// bool PRINT_BPTREE(char* item, int min_frequency);

	void printErrorCode(string str, int n);
	void printSuccessCode(string str);
};

