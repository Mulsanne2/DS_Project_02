#include "Manager.h"

void Manager::run(const char* command)
{
	fin.open(command); //open command.txt
	flog.open("log.txt", ios::app);
	if(!fin) //check if file open appropriately
	{
		flog << "File Open Error" << endl;
		return;
	}

	string Instruction;
	while (!fin.eof())
	{
		getline(fin, Instruction); //get line from command

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

		else if (strcmp(temp, "BTLOAD") == 0) // check if instruction is BTLOAD
		{
			if(BTLOAD()){ //execute BTLOAD instruction
				printSuccessCode("BTLOAD");
			}
			else{
				printErrorCode("BTLOAD", 200);
			}
		}

		else if (strcmp(temp, "PRINT_ITEMLIST") == 0) // check if instruction is PRINT_ITEMLIST
		{
			if (!PRINT_ITEMLIST()) //execute PRINT_ITEMLIST instruction
			{
				printErrorCode("PRINT_ITEMPLIST", 300);
			}
		}

		else if (strcmp(temp, "PRINT_FPTREE") == 0) // check if instruction is PRINT_ITEMLIST
		{
			if (!PRINT_FPTREE()) //execute PRINT_FPTREE instruction
			{
				printErrorCode("PRINT_FPTREE", 400);
			}
		}

		else if (strcmp(temp, "PRINT_BPTREE") == 0) // check if instruction is PRINT_BPTREE
		{	
			if(!PRINT_BPTREE()){ //execute PRINT_BPTREE instruction
				printErrorCode("PRINT_BPTREE", 500);
			}
		}

		else if (strcmp(temp, "PRINT_CONFIDENCE") == 0) // check if instruction is PRINT_CONFIDENCE
		{
			if (!PRINT_CONFIDENCE()) //execute PRINT_CONFIDENCE instruction
			{
				printErrorCode("PRINT_CONFIDENCE", 600);
			}
		}

		else if (strcmp(temp, "PRINT_RANGE") == 0) // check if instruction is PRINT_RANGE
		{
			if (!PRINT_RANGE()) //execute PRINT_RANGE instruction
			{
				printErrorCode("PRINT_RANGE", 700);
			}
		}

		else if (strcmp(temp, "EXIT") == 0) // check if instruction is EXIT
		{			
			printSuccessCode("EXIT");
			return;
		}
		
		else{
			flog << "Wrong Instruction" << endl
				 << endl;
			continue;
		}

		delete[] cmd; //delete command line
	}
	
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
	readItem.open("market.txt"); //oepn market.txt file
	if (!readItem) //check if market.txt is opened
	{
		flog << "File Open Error" << endl;
		return false;
	}
	string ItemLine;

	while (!readItem.eof()) //read market.txt until end
	{
		getline(readItem, ItemLine); //read line from market.txt

		char * cmdtemp = new char[ItemLine.length()+1];
		strcpy(cmdtemp, ItemLine.c_str());
		if (cmdtemp[0] == 0) // check if there is only NULL on line
			continue;

		char *temp = strtok(cmdtemp, "\t"); //cut the string by TAB
		list<string> ITEMS;
		bool sameElement = false;
		while (1)
		{
			if(temp==NULL)
				break;
			for (auto iter = ITEMS.begin(); iter != ITEMS.end();iter++){ //check if has same item name before
				if(*iter==temp)
					sameElement = true;
			}
			if(sameElement==true){
				sameElement = false;
				temp = strtok(NULL, "\t");
				continue;
			}
			ITEMS.push_back(temp);

			if (!fpgrowth->getHeaderTable()->Find_Item(temp))
			{									// if there is no item on the index table
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
		rereadItem.close();
		return false;
	}
	while(!rereadItem.eof()){
		getline(rereadItem, ItemLine2); //read line again
		list<string> SortedItem;
		list<pair<int, string>> INDEXTABLE = fpgrowth->getHeaderTable()->getindexTable();
		list<pair<int, string>>::iterator iter;
		for (iter = INDEXTABLE.begin(); iter!=INDEXTABLE.end();iter++){ //visit all index table item by frequency
			if (ItemLine2.find(iter->second) != string::npos && iter->first >= threshold) // if item is in the market txt.line
			{										
				bool findItem = true; //check if line has item
				int num = ItemLine2.find(iter->second);
				if (ItemLine2[num - 1] == ' '|| ItemLine2[num + iter->second.length()]==' '){
					findItem = false;
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

		fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), SortedItem, 1); //push on FPTree
	}
	rereadItem.close(); //close ifstream

	return true; //return true
}

bool Manager::BTLOAD() //read result.txt and make B+Tree
{
	//check BTLOAD has Already Done
	if (bptree->getRoot() != NULL)
		return false;

	ifstream ReadResult;
	ReadResult.open("result.txt");
	if (!ReadResult) // check if result.txt is opened
	{
		flog << "File Open Error" << endl;
		return false;
	}
	string ResultLine;
	while (!ReadResult.eof())
	{
		getline(ReadResult, ResultLine); //read line from market.txt

		char *tempResult = new char[ResultLine.length() + 1];
		strcpy(tempResult, ResultLine.c_str());
		if(tempResult[0]==0)
			continue;

		char *temp = strtok(tempResult, "\t"); //read frequecny number

		if(temp[0]<48 || temp[0]>57){ //check first number is number
			flog << "Wrong market.txt format" << endl;
			return false;
		}

		int FREQUENCY = stoi(temp); //change string into int
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

	fpgrowth->getHeaderTable()->ascendingIndexTable(); //change Index table in ascending order
	fpgrowth->printTree(); //print tree
	fpgrowth->getHeaderTable()->descendingIndexTable(); //go back to original Index table
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

	if(bptree->printFrequency(ITEM, MIN_FRE)) //print frequency
		return true;
	else
		return false;
}

bool Manager::PRINT_CONFIDENCE() {
	char *fitem = strtok(NULL, "\t");
	char *fnum = strtok(NULL, "\t");
	if (fitem == NULL || fnum == NULL) // check if input is NULL
		return false;
	if (fnum[0] < 48 || fnum[0] > 57) // check if fnum is number
		return false;
	string item = fitem; //store item name
	double confi = stod(fnum); //change string into double

	int ItemFrequency = 0;
	list<pair<int, string>> table = fpgrowth->getHeaderTable()->getindexTable(); //get frequency from index table
	for (auto iter = table.begin(); iter != table.end();iter++){
		if(iter->second==item)
			ItemFrequency = iter->first;
	}
	if(ItemFrequency==0)//check if ItemFrequency is 0
		return false;

	double min_frequency = double(ItemFrequency) * confi; //caclulate min frequency

	if(bptree->printConfidence(item, double(ItemFrequency), min_frequency)) //print confidence
		return true;
	else
		return false;
}

bool Manager::PRINT_RANGE() {
	char *fitem = strtok(NULL, "\t");
	char *fnum = strtok(NULL, "\t");
	char *fnum2 = strtok(NULL, "\t");
	if (fitem == NULL || fnum == NULL || fnum2 == NULL) // check if input is NULL
		return false;
	if (fnum[0] < 48 || fnum[0] > 57) // check if fnum is number
		return false;
	if (fnum2[0] < 48 || fnum2[0] > 57) // check if fnum2 is number
		return false;

	int MIN_FRE = stoi(fnum); // change into int
	int MAX_FRE = stoi(fnum2);
	string ITEM = fitem; // change into string

	if (bptree->printRange(ITEM, MIN_FRE,MAX_FRE)) //print range
		return true;
	else
		return false;
}

void Manager::EXIT(){
	printSuccessCode("EXIT"); // execute EXIT
}

void Manager::printErrorCode(string str,int n) {				//ERROR CODE PRINT
	flog << "=========" << str << "=========" << endl;
	flog << " ERROR " << n << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode(string str){ // SUCCESS CODE PRINT

	flog << "=========" << str << "=========" << endl;
	flog << "SUCCESS" << endl;
	flog << "=======================" << endl << endl;
}
