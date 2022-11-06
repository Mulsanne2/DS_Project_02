#include "HeaderTable.h"

// HeaderTable::~HeaderTable() {

// }

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
	// auto iter = indexTable.begin();
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


