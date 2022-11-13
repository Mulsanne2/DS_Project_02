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
		if(root->getParent())
			root = root->getParent();
		}
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
	
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	
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

	return true;
}
bool BpTree::printRange(string item, int min, int max) {

	return true;
}
void BpTree::printFrequentPatterns(set<string> pFrequentPattern, string item) {
	*fout << "{";
	set<string> curPattern = pFrequentPattern;
	curPattern.erase(item);
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) {
		string temp = *it++;
		if (temp != item) *fout << temp;
		if (it == curPattern.end()) {
			*fout << "} ";
			break;
		}
		*fout << ", ";
	}
}