#include "FPNode.h"

FPNode::FPNode()	//constructor
{
	parent = NULL;
	next = NULL;
	frequency = 0;
}


FPNode::~FPNode()//destructor
{
	parent = NULL;
	next = NULL;
	// delete all the children map nodes
	map<string, FPNode *>::iterator iter = children.begin();
	while(iter!=children.end()){
		children.erase(iter++);
	}
}

FPNode* FPNode::getChildrenNode(string item) { //get children node which name is item
	map<string, FPNode*>::iterator it = children.find(item);
	if (it == children.end()) return NULL;
	FPNode* findNode = it->second;
	return findNode;
}

string FPNode::FINDNAME(FPNode *Parent){ //find name from parent which is same node
	map<string, FPNode *>::iterator iter;
	for (iter = Parent->children.begin(); iter != Parent->children.end(); iter++)
	{
		if (iter->second == this)
		{
			return iter->first;
		}
	}
	return " ";
}
