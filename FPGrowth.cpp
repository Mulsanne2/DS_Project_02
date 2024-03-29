#include "FPGrowth.h"

FPGrowth::~FPGrowth() { //destructor of FPGrowth

    table->~HeaderTable(); //delete all the tree nodes except root node
    delete fpTree; //delete root node
    delete table; //dele table
    flog.close();
}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) { //create FPtree
    FPNode *CurrentNode = root;
    list<string>::iterator iter;

    for (iter = item_array.begin(); iter != item_array.end(); iter++)
    {

       if (CurrentNode->getChildrenNode(*iter))
        {                                                       // check if there is item node alreay exists
            CurrentNode = CurrentNode->getChildrenNode(*iter); // move to children node
            CurrentNode->updateFrequency(1);                   // increase frequency
        }
        else
        { // if there child node don't exist
            FPNode *newChild = new FPNode;    // make new node
            newChild->setParent(CurrentNode); // set parent node as current
        
            connectNode(table, *iter, newChild);

            CurrentNode->pushchildren(*iter, newChild); // connecte the new Child into children map
            CurrentNode = CurrentNode->getChildrenNode(*iter); // move to children node
            CurrentNode->updateFrequency(1);                   // increase frequency

        }
 
    }
    }

void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) {
    if(!table->getNode(item)){ //if item don't have any connected node in data table
        table->insertNode(item, node);
    }
    else{ //if item has already connected node in data table
        FPNode *LastNode = table->getNode(item);
        while(LastNode->getNext()){ //move to last node
            LastNode = LastNode->getNext();
        }
        LastNode->setNext(node);
    }
}

// bool FPGrowth::contains_single_path(FPNode* pNode) {
// 	if (pNode->getChildren().size() == 0) return true;
// 	else if (pNode->getChildren().size() > 1) return false;
// 	return contains_single_path(pNode->getChildren().begin()->second);
// }

// map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable* pTable, FPNode* pTree) {

// 	return {};
// }



// void FPGrowth::powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* ptr, int depth) {
// 	if (data.size() == depth) {
// 		set<string> set; set.insert(item);
// 		for (int i = 0; i < data.size(); i++) { if (ptr[i] == 1) set.insert(data[i]); }
// 		FrequentPattern->insert(make_pair(set, frequency)); return;
// 	}
// 	ptr[depth] = 1;
// 	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
// 	ptr[depth] = 0;
// 	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
// }

bool FPGrowth::printList() { //print the index table

    list<pair<int, string>> ptr = table->getindexTable(); //get index table

    list<pair<int, string>>::iterator it = ptr.begin(); //declare iterator

    *fout << "========PRINT_ITEMLIST========" << endl;
    *fout << "Item Frequency" << endl;
    while(it != ptr.end()){ //print first data in index node to last data
        *fout << it->second << " " << it->first << endl;
        it++;
    }
    *fout << "===============================" << endl
          << endl;
    return true;
    }

bool FPGrowth::printTree() {
    list<pair<int, string>> ascendingTable = table->getindexTable();
    list<pair<int, string>>::iterator iter;

    *fout << "========PRINT_FPTREE========" << endl;
    *fout << "{StandardItem,Frequency} (Path_Item,Frequency)" << endl;
    for (iter = ascendingTable.begin(); iter != ascendingTable.end(); iter++)
    {
        if (iter->first < threshold) // do not print data which is lower than threshold
            continue;

        *fout << "{" << iter->second << ", " << iter->first << "}" << endl;
        FPNode *LeafTraveler = table->getNode(iter->second);
        FPNode *CURRENT, *Parent; //CURRENT is value that currrent Node
        while (LeafTraveler) //move to next node
        {
            CURRENT = LeafTraveler;
            map<string, FPNode *> TEMPMAP;
            map<string, FPNode *>::iterator iter;

            while(CURRENT->getParent()){ //find the name of it's node
                //print the it's node name and frequency
                *fout << "(" << CURRENT->FINDNAME(CURRENT->getParent()) << ", " << CURRENT->getFrequency() << ")";

                CURRENT = CURRENT->getParent(); //move to the parent
            }
            *fout << endl;
            //move to next connected node
            LeafTraveler = LeafTraveler->getNext();
        }
    }
    *fout << "=========================" << endl
          << endl;

    return true;
}
// void FPGrowth::saveFrequentPatterns(){

// }
