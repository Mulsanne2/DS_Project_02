#include "BpTree.h"

bool BpTree::Insert(int key, set<string> set) {

	BpTreeNode *FindingNode = searchDataNode(key); //find DataNode which has key value
	/*
	check if node hasn't found
	*/
	if(FindingNode==NULL){ //if node don't have key value then make new Frequent Pattern Node

		FrequentPatternNode *NewFreNode = new FrequentPatternNode;
		NewFreNode->setFrequency(key); // insert key in Freqeunt patterNode
		NewFreNode->InsertList(set);
		
		if(root==NULL){ //when make the fisrt node and set to root
			BpTreeNode *NewDataNode = new BpTreeDataNode;
			NewDataNode->insertDataMap(key, NewFreNode);
			root = NewDataNode; //set root
		}
		else if(!root->getMostLeftChild()){ //check while insert in root
			root->insertDataMap(key, NewFreNode);
			if(excessDataNode(root)) //check root has been over
				splitDataNode(root);
		}
		else{
		BpTreeNode* CurNode = root;
		while(CurNode->getMostLeftChild()) //move to last node
			CurNode = CurNode->getMostLeftChild();

		while(CurNode->getNext()){ //move to right arrange
			if(key<CurNode->getNext()->getDataMap()->begin()->first)
				break;
			CurNode = CurNode->getNext();
		}
		CurNode->insertDataMap(key, NewFreNode);
		if(excessDataNode(CurNode))
			splitDataNode(CurNode);

		CurNode = CurNode->getParent();
		while(CurNode){
			if(excessIndexNode(CurNode))
				splitIndexNode(CurNode);
			CurNode = CurNode->getParent();
		}
		}
		if (root->getParent())
		root = root->getParent();
	}
	else{
		FindingNode->getDataMap()->find(key)->second->InsertList(set); //put item in insert list
	}

	return true;
}

BpTreeNode* BpTree::searchDataNode(int n) {
	BpTreeNode* pCur = root;
	
	if(root==NULL) //check if root is NULL
		return NULL;

	while( pCur->getMostLeftChild()){ //Move to the Left child from the bottom 
		pCur = pCur->getMostLeftChild();
	}
	
	while(pCur){ //get the 
		map<int, FrequentPatternNode *> *CurMap = pCur->getDataMap(); 
		if(CurMap->find(n)!=CurMap->end())
			return pCur;
		pCur = pCur->getNext();
	}
	return NULL;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
	BpTreeDataNode *newDataNode = new BpTreeDataNode;
	int INDEX = ceil(double(order) / 2);
	// int INDEX = order/2;
	for (int i = 0; i < INDEX;i++){ //divide into two Data node, pDataNode will be left side node
		newDataNode->insertDataMap(pDataNode->getDataMap()->rbegin()->first, pDataNode->getDataMap()->rbegin()->second);
		pDataNode->deleteMap(newDataNode->getDataMap()->begin()->first);
	}
	if(!pDataNode->getNext()){ //if pDataNode doesn't have right node
		newDataNode->setPrev(pDataNode);
		pDataNode->setNext(newDataNode);
	}
	else{ //if pDataNode has right node
		newDataNode->setNext(pDataNode->getNext());
		pDataNode->getNext()->setPrev(newDataNode);
		pDataNode->setNext(newDataNode);
		newDataNode->setPrev(pDataNode);
	}

	if(pDataNode->getParent()){//if pDataNode has parent node, then just add on the parent node
		BpTreeNode *ParentNode = pDataNode->getParent();
		pDataNode->getParent()->insertIndexMap(newDataNode->getDataMap()->begin()->first, newDataNode);
		newDataNode->setParent(ParentNode);
	}
	else{
		BpTreeNode *ParentNode = new BpTreeIndexNode;
		ParentNode->insertIndexMap(newDataNode->getDataMap()->begin()->first, newDataNode);
		ParentNode->setMostLeftChild(pDataNode);
		pDataNode->setParent(ParentNode);
		newDataNode->setParent(ParentNode);
	}
	

}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	BpTreeIndexNode* newIndexNode = new BpTreeIndexNode;
	int INDEX = order / 2;
	for (int i = 0; i < INDEX; i++)
	{ // divide into two Index Node, pIndexNode will be left index node
		newIndexNode->insertIndexMap(pIndexNode->getIndexMap()->rbegin()->first, pIndexNode->getIndexMap()->rbegin()->second);
		newIndexNode->getIndexMap()->begin()->second->setParent(newIndexNode);
		pIndexNode->deleteMap(newIndexNode->getIndexMap()->rbegin()->first);
	}
	
	int UpNum = pIndexNode->getIndexMap()->rbegin()->first; //get center node's number which going up
	BpTreeNode *UpNode = pIndexNode->getIndexMap()->rbegin()->second; // get center node's treeNode which going up
	pIndexNode->deleteMap(UpNum); //delete center node in previous node

	if(!pIndexNode->getParent()){ //when pIndexNode doesn't have parent node
		BpTreeIndexNode *newUpIndexNode = new BpTreeIndexNode;
		newUpIndexNode->setMostLeftChild(pIndexNode);
		newUpIndexNode->insertIndexMap(UpNum, newIndexNode);
		newIndexNode->setMostLeftChild(UpNode);
		//set the new parents
		UpNode->setParent(newIndexNode);
		newIndexNode->setParent(newUpIndexNode);
		pIndexNode->setParent(newUpIndexNode);
	}
	else{ //when pIndexNode have parent node then put into parent node
		BpTreeNode *UpIndexNode = pIndexNode->getParent();
		UpIndexNode->insertIndexMap(UpNum, newIndexNode);
		newIndexNode->setParent(UpIndexNode); //set parent
		newIndexNode->setMostLeftChild(UpNode);
		UpNode->setParent(newIndexNode); //set parent
	}

}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > order - 1)return true;//order is equal to the number of elements 
	else return false;
}


bool BpTree::printConfidence(string item, double item_frequency, double min_confidence)
{	

	return true;
}
bool BpTree::printFrequency(string item, int min_frequency)//print winratio in ascending order
{
	bool bar = false;
	if (root == NULL)
		return false;

	BpTreeNode *CurNode = root;

	while (CurNode->getMostLeftChild())
	{
		CurNode = CurNode->getMostLeftChild();
	}
	while(CurNode){
		map<int, FrequentPatternNode *>* Data = CurNode->getDataMap();
		for (auto iter = Data->begin(); iter != Data->end();iter++){ //approach to every data node
			if (iter->first >= min_frequency) //check if frequency is over min_frequency
			{
			multimap<int, set<string>> TEMP = iter->second->getList(); //get the multimap from Frequent node
			for (auto iter2 = TEMP.begin(); iter2 != TEMP.end();iter2++){
				if(iter2->second.find(item)!=iter2->second.end()){
					if (bar == false)
					{
						cout << "========PRINT_BPTREE========" << endl
						<< "FrequentPattern	Frequency" << endl;
						bar = true;
					}
					printFrequentPatterns(iter2->second);
					cout << iter->first << endl;
					}
			}
			}
		}
		CurNode = CurNode->getNext();
		}
		if (bar == false)
		return false;
		cout << "==========================" << endl
			 << endl;
		return true;
}

bool BpTree::printRange(string item, int min, int max) {

	return true;
}

void BpTree::printFrequentPatterns(set<string> pFrequentPattern)
{
	*fout << "{";
	cout << "{";
	set<string> curPattern = pFrequentPattern;
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();)
	{
		string temp = *it++;
		*fout << temp;
		cout << temp;
		if (it == curPattern.end())
		{
			*fout << "} ";
			cout << "} ";
			break;
		}
		*fout << ", ";
		cout << ", ";
	}
}

// void BpTree::printFrequentPatterns(set<string> pFrequentPattern, string item) {
// 	*fout << "{";
// 	set<string> curPattern = pFrequentPattern;
// 	curPattern.erase(item);
// 	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) {
// 		string temp = *it++;
// 		if (temp != item) *fout << temp;
// 		if (it == curPattern.end()) {
// 			*fout << "} ";
// 			break;
// 		}
// 		*fout << ", ";
// 	}
// }