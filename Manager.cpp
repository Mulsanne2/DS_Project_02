#include "Manager.h"

void Manager::run(const char* command)
{
	fin.open(command);
	flog.open("log.txt", ios::app);
	if(!fin)
	{
		flog << "File Open Error" << endl;
		cout << "File Open Error" << endl;
		//EXIT()
		return;
	}

	string Instruction;
	while (!fin.eof())
	{
		getline(fin, Instruction);

		cmd = new char[Instruction.length()];
		strcpy(cmd, Instruction.c_str());
		if (cmd[0] == 0) //check if there is only NULL on line
			continue;
		char *temp = strtok(cmd, " ");

		if(strcmp(temp,"LOAD")==0)
		{
			if(LOAD()){
				printSuccessCode("LOAD");
			}
			else{
				printErrorCode("LOAD", 100);
			}
		}

		else if (strcmp(temp, "PRINT_ITEMLIST") == 0) // check if instruction is PRINT_ITEMLIST
		{
			if (!PRINT_ITEMLIST())
			{
				printErrorCode("PRINT_ITEMPLIST", 300);
			}
		}
		delete[] cmd;
	}
	fin.close();
	return;
}

bool Manager::LOAD() //load market.txt and construct FP-Growth which has IndexTable and DataTable
{
	ifstream readItem;
	readItem.open("market.txt");
	if (!readItem) //check if market.txt is opened
	{
		flog << "File Open Error" << endl;
		cout << "File Open Error" << endl;
		return false;
	}
	string ItemLine;

	while (!readItem.eof()) //read market.txt until end
	{
		getline(readItem, ItemLine);

		cmd = new char[ItemLine.length()];
		strcpy(cmd, ItemLine.c_str());
		if (cmd[0] == 0) // check if there is only NULL on line
			continue;

		char *temp = strtok(cmd, "\t");
		while(1){
			if(temp==NULL)
				break;
			if(!fpgrowth->getHeaderTable()->Find_Item(temp)){ //if there is no item on the index table
				// fpgrowth->getHeaderTable()->insertTable(temp, 1); //insert table
				fpgrowth->createTable(temp, 1); //insert table
			}
			else{
				fpgrowth->getHeaderTable()->Increase_Frequency(temp); //increase Frequency of temp
			}
			temp = strtok(NULL, "\t"); //get next item in the same line
		}
		delete[] cmd;
	}
	fpgrowth->getHeaderTable()->descendingIndexTable();


	return true;
}




// bool Manager::BTLOAD()
// {
	
// 	return true;
// }

bool Manager::PRINT_ITEMLIST() {
	if(fpgrowth->getHeaderTable()->getindexTable().empty()){// || fpgrowth->getHeaderTable()->getdataTable().empty()){ 추후에 뒤 추할 것
		return false;
	}
	fpgrowth->printList();

	return true;
}

// bool Manager::PRINT_FPTREE() {
	
// }

// bool Manager::PRINT_BPTREE(char* item, int min_frequency) {
	
// }

// bool Manager::PRINT_CONFIDENCE(char* item, double rate) {
	
// }

// bool Manager::PRINT_RANGE(char* item, int start, int end) {
	
// }

void Manager::printErrorCode(string str,int n) {				//ERROR CODE PRINT
	flog << "=========" << str << "=========" << endl;
	flog << " ERROR " << n << endl;
	flog << "=======================" << endl << endl;

	cout << "=========" << str << "=========" << endl;
	cout << " ERROR " << n << endl;
	cout << "=======================" << endl
		 << endl;
}

void Manager::printSuccessCode(string str){ // SUCCESS CODE PRINT

	flog << "=========" << str << "=========" << endl;
	flog << "SUCCESS" << endl;
	flog << "=======================" << endl << endl;

	cout << "=========" << str << "=========" << endl;
	cout << "SUCCESS" << endl;
	cout << "=======================" << endl
		 << endl;
}
