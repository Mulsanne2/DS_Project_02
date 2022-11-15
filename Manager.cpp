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

		cmd = new char[Instruction.length()+1];
		strcpy(cmd, Instruction.c_str());
		if (cmd[0] == 0) //check if there is only NULL on line
			continue;
		char *temp = strtok(cmd, "\t");

		if (strcmp(temp, "LOAD") == 0) // check if instruction is LOAD
		{
			if(LOAD()){ //execute LOAD instruction
				printSuccessCode("LOAD");
			}
			else{
				printErrorCode("LOAD", 100);
			}
		}

		else if (strcmp(temp, "BTLOAD") == 0)
		{
			if(BTLOAD()){
				printSuccessCode("BTLOAD");
			}
			else{
				printErrorCode("BTLOAD", 200);
			}
		}

		else if (strcmp(temp, "PRINT_ITEMLIST") == 0) // check if instruction is PRINT_ITEMLIST
		{
			if (!PRINT_ITEMLIST())
			{
				printErrorCode("PRINT_ITEMPLIST", 300);
			}
		}

		else if (strcmp(temp, "PRINT_FPTREE") == 0) // check if instruction is PRINT_ITEMLIST
		{
			if (!PRINT_FPTREE())
			{
				printErrorCode("PRINT_FPTREE", 400);
			}
		}
		
		else if (strcmp(temp, "PRINT_BPTREE")==0)
		{	
			if(!PRINT_BPTREE()){
				printErrorCode("PRINT_BPTREE", 500);
			}
		}

		else if (strcmp(temp, "PRINT_RANGE") == 0)
		{
			if (!PRINT_RANGE())
			{
				printErrorCode("PRINT_RANGE", 700);
			}
		}

		delete[] cmd;
	}
	fin.close();
	flog.close();
	return;
}

bool Manager::LOAD() //load market.txt and construct FP-Growth which has IndexTable and DataTable
{
	//check if LOAD has already done
	if (!fpgrowth->getHeaderTable()->getindexTable().empty() || !fpgrowth->getHeaderTable()->getdataTable().empty())
	{
		return false; //return false
	}

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

		char * cmdtemp = new char[ItemLine.length()+1];
		strcpy(cmdtemp, ItemLine.c_str());
		if (cmdtemp[0] == 0) // check if there is only NULL on line
			continue;

		char *temp = strtok(cmdtemp, "\t"); //cut the string by TAB
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
		delete[] cmdtemp;
	}
	fpgrowth->getHeaderTable()->descendingIndexTable(); //sort Index Table
	fpgrowth->getHeaderTable()->makeDataTable(); //make data table refer to sorted index table
	readItem.close();

	if(fpgrowth->getHeaderTable()->getindexTable().front().first<threshold){ //if largest item has lower frequency then threshold value
		return false;
	}

	//read market.txt again by frequency and threshold
	ifstream rereadItem;
	rereadItem.open("market.txt");
	string ItemLine2;
	if (!rereadItem) // check if market.txt is opened
	{
		return false;
	}
	while(!rereadItem.eof()){
		getline(rereadItem, ItemLine2); //read line again
		list<string> SortedItem;
		list<pair<int, string>> INDEXTABLE = fpgrowth->getHeaderTable()->getindexTable();
		list<pair<int, string>>::iterator iter;
		for (iter = INDEXTABLE.begin(); iter!=INDEXTABLE.end();iter++){ //visit all index table item by frequency
			if (ItemLine2.find(iter->second) != string::npos && iter->first >= threshold)
			{										// if item is in the market txt.line
				bool findItem = true; //check if line has item
				// string TEMP = iter->second; // get item
				int num = ItemLine2.find(iter->second);
				if (ItemLine2[num - 1] == ' '|| ItemLine2[num + iter->second.length()]==' '){
					findItem = false;
					// continue;
				}
				if(findItem==false){ //find another item
					while(1){
						if (ItemLine2.find(iter->second, num + 1) == string::npos) //find another item
							break;
						num = ItemLine2.find(iter->second, num + 1);
						if (ItemLine2[num - 1] == ' ' || ItemLine2[num + iter->second.length()] == ' ')
						{
							findItem = false;
						}
						else //if we finded the item then put in list
							findItem = true;
					}
				}
				
				if(findItem==true) //if line has it's item then push into list
					SortedItem.push_back(iter->second); // push item in to SortedItem list
			}
		}

		if(SortedItem.empty()) //when there is no item satisfy threshold
			continue;

		// for (string i : SortedItem){
		// 	cout << i << " ";
		// 	flog << i << " ";
		// }
		// cout << endl;
		// flog << endl;

		fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), SortedItem, 1); //push on FPTree
	}
	rereadItem.close();

	return true;
}

bool Manager::BTLOAD() //read result.txt and make B+Tree
{
	//check BTLOAD has Already Done
	/*
	DO IT!
	*/

	ifstream ReadResult;
	ReadResult.open("result.txt");
	if (!ReadResult) // check if result.txt is opened
	{
		flog << "File Open Error" << endl;
		cout << "File Open Error" << endl;
		return false;
	}
	string ResultLine;
	while (!ReadResult.eof())
	{
		getline(ReadResult, ResultLine);

		char *tempResult = new char[ResultLine.length() + 1];
		strcpy(tempResult, ResultLine.c_str());
		if(tempResult[0]==0)
			continue;

		char *temp = strtok(tempResult, "\t"); //read frequecny number

		if(temp[0]<48 || temp[0]>57){ //check first number is number
			cout << "Wrong market.txt format" << endl;
			flog << "Wrong market.txt format" << endl;
			return false;
		}

		int FREQUENCY = stoi(temp);
		temp = strtok(NULL, "\t");
		set<string> lineList;
		while(1){
			if(temp==NULL)
				break;
			if (lineList.find(temp)!=lineList.end())// if there same item on the line return false
				return false;

			string Item = temp;
			lineList.insert(Item); //insert into set
			temp = strtok(NULL, "\t"); //move to next item
		}

		if(lineList.empty()) //if set is empty then move to next line
			continue;

		//insert BpTree
		bptree->Insert(FREQUENCY, lineList);

		delete[] tempResult;
	}

	ReadResult.close(); //close ifstream
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	// check if LOAD has already done
	if (fpgrowth->getHeaderTable()->getindexTable().empty() || fpgrowth->getHeaderTable()->getdataTable().empty()){ 
			return false;
		}
	fpgrowth->printList(); //print header table

	return true;
}

bool Manager::PRINT_FPTREE() {
	if(fpgrowth->getTree()->getChildren().empty()){ //check if FPTREE is empty
			return false;
	}

	fpgrowth->getHeaderTable()->ascendingIndexTable(); //change Header table in ascending order
	fpgrowth->printTree(); //print tree
	return true;
}

bool Manager::PRINT_BPTREE() {
	char *fitem = strtok(NULL, "\t");
	char *fnum = strtok(NULL, "\t");
	if(fitem==NULL||fnum==NULL) //check if input is NULL
		return false;
	if (fnum[0] < 48 || fnum[0] > 57) // check if fnum is number
		return false;
	int MIN_FRE = stoi(fnum); // change into int
	string ITEM = fitem; //change into string

	if(bptree->printFrequency(ITEM, MIN_FRE))
		return true;
	else
		return false;
}

// bool Manager::PRINT_CONFIDENCE(char* item, double rate) {
	
// }

bool Manager::PRINT_RANGE() {
	char *fitem = strtok(NULL, "\t");
	char *fnum = strtok(NULL, "\t");
	char *fnum2 = strtok(NULL, "\t");
	if (fitem == NULL || fnum == NULL) // check if input is NULL
		return false;
	if (fnum[0] < 48 || fnum[0] > 57) // check if fnum is number
		return false;
	if (fnum2[0] < 48 || fnum2[0] > 57) // check if fnum2 is number
		return false;

	int MIN_FRE = stoi(fnum); // change into int
	int MAX_FRE = stoi(fnum2);
	string ITEM = fitem; // change into string

	if (bptree->printRange(ITEM, MIN_FRE,MAX_FRE))
		return true;
	else
		return false;
}

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
