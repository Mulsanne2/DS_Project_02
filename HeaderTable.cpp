#include "HeaderTable.h"

HeaderTable::~HeaderTable() {
	//1) delete data Table
	FPNode *toDelete;
	FPNode *nextDelete;
	map<string, FPNode *>::iterator iter1 = dataTable.begin();
	while(iter1!=dataTable.end()){
		toDelete = iter1->second; //get the fist node from data table
		while(toDelete!=NULL){
			nextDelete = toDelete->getNext(); //store next node to delete
			toDelete->setNext(NULL); //erase next pointer
			delete toDelete; //delete currnet node
			toDelete = nextDelete; //move to next node
		}
		iter1++;
	}
	dataTable.clear();
	// 2) delete index Table
	indexTable.clear();
}

void HeaderTable::insertTable(char* item, int frequency) {
	indexTable.push_back(make_pair(frequency, item));
}

void HeaderTable::makeDataTable(){
	list<pair<int, string>>::iterator iter;
	for (iter = indexTable.begin(); iter != indexTable.end();iter++){
		string ITEM = iter->second;
		dataTable.insert({ITEM, NULL});
	}
}

int HeaderTable::find_frequency(string item){
	int frequency = 0;
	list<pair<int, string>>::iterator iter = indexTable.begin();
	while (iter != indexTable.end())
	{
		if (iter->second == item)
		{
			frequency = iter->first;
			return frequency;
		}
		iter++;
	}
	return frequency;
}

bool HeaderTable::Find_Item(string item){
	list<pair<int, string>>::iterator iter = indexTable.begin();
	while (iter != indexTable.end()){
		if(iter->second == item){
			return true;
		}
		iter++;
	}
	return false;
}

void HeaderTable::Increase_Frequency(string item){
	list<pair<int, string>>::iterator iter = indexTable.begin();
	while (iter != indexTable.end())
	{
		if (iter->second == item)
		{
			int Frequency = find_frequency(item);
			Frequency++;
			iter->first = Frequency;
		}
		iter++;
	}
}

void HeaderTable::insertNode(string item, FPNode *newNode){
	dataTable.find(item)->second = newNode;
}