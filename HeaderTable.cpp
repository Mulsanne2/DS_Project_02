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

void HeaderTable::makeDataTable(){ //make data table from index table
	list<pair<int, string>>::iterator iter;
	for (iter = indexTable.begin(); iter != indexTable.end();iter++){
		string ITEM = iter->second;
		dataTable.insert({ITEM, NULL});
	}
}

int HeaderTable::find_frequency(string item){ //get frequency of item in index node
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

bool HeaderTable::Find_Item(string item){ //check if index node has same item 
	list<pair<int, string>>::iterator iter = indexTable.begin();
	while (iter != indexTable.end()){
		if(iter->second == item){
			return true;
		}
		iter++;
	}
	return false;
}

void HeaderTable::Increase_Frequency(string item){ //function that increase 1 in index table
	list<pair<int, string>>::iterator iter = indexTable.begin();
	while (iter != indexTable.end())
	{
		if (iter->second == item) //find data which name is same with item
		{
			int Frequency = find_frequency(item);
			Frequency++;
			iter->first = Frequency;
		}
		iter++;
	}
}

void HeaderTable::insertNode(string item, FPNode *newNode){ //insert node
	dataTable.find(item)->second = newNode;
}