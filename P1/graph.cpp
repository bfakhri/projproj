#include "graph.h"

graph::graph() {	
	size = 0;
}

graph::graph(int numOfNodes) : size(numOfNodes), graphNodeList(numOfNodes) {	
	// size = 0; // don't know the usage
	// std::vector graphNodeList(numOfNodes);
	cout << "size of list is " << graphNodeList.size() << endl;
}

graph::graphNode* graph::newNode(dataType n1) {
	// USELSS


	graphNode *Nptr;
	Nptr = new graphNode();
	Nptr->val 	= n1;
	Nptr->id 	= this->size;
	Nptr->next	= NULL;

	// cout << "id: " << Nptr->id << endl;

	return Nptr;
}

void graph::addNode(dataType n1) {
	// USELSS


	// cout << "# addNode\n";

	// std::vector<graphNode> *tmpVectorPtr = new std::vector<graphNode*>();

	// // new node
	// graphNode *Nptr;
	// Nptr = newNode(n1);
	// this->size++;
	// tmpVectorPtr->push_back(Nptr);

	// // add to node list
	// graphNodeList.push_back(tmpVectorPtr);

}

void graph::addEdge(unsigned int id1, unsigned int id2) {
	if (id1 >= size || id2 >= size) return;

	// cout << "# addEdge\n";
	if ( graphNodeList[id1] == NULL ) { // No edge yet, new a vector
		graphNodeList[id1] = new std::vector<int>();
	}
	if ( graphNodeList[id2] == NULL ) { // No edge yet, new a vector
		graphNodeList[id2] = new std::vector<int>();
	}

	(graphNodeList[id1])->push_back(id2); // doesn't check existance	
	(graphNodeList[id2])->push_back(id1); // doesn't check existance	
	
}

void graph::printGraph() {
	cout << "# print Graph - size : " << this->size << endl;
	// for (std::vector<graphNode*>::iterator itr = graphNodeList.begin(); itr != graphNodeList.end(); itr++) {
	// 	cout << "| " << (*itr)->val << " |- ";

	// 	graphNode *NodePtr = (*itr)->next;
	// 	while (NodePtr) {
	// 		cout << NodePtr->val << ' ';
	// 		NodePtr = NodePtr->next;
	// 	}

	// 	cout << endl;
	// }

	for ( std::vector< std::vector<int> *>::iterator nodeItr = graphNodeList.begin();
		 nodeItr != graphNodeList.end(); nodeItr++ ) {

		printf("[%d] ", (int)(nodeItr - graphNodeList.begin()));
		if (*nodeItr == NULL) {
			cout << endl;
			continue;
		}

		for ( std::vector<int>::iterator edgeItr = (*nodeItr)->begin();
		 edgeItr != (*nodeItr)->end(); edgeItr++ ) {
			cout << (*edgeItr) << ' ';
		}

		cout << endl;
	}

}

graph::~graph() {	
	// for (std::vector<graphNode*>::iterator itr = graphNodeList.begin(); itr != graphNodeList.end(); itr++) {
	// 	cout << "delete " << (*itr)->val << endl;
	// 	delete *itr;
	// }
	for (std::vector<std::vector<int> *>::iterator itr = graphNodeList.begin(); itr != graphNodeList.end(); itr++) {
		if ((*itr) == NULL) continue;	
		// cout << "# delete " << itr - graphNodeList.begin() << endl;
		delete (*itr);
	}
}
