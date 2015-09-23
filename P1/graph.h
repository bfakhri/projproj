#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;
#define dataType int 

class graph {
// Adj list
private:
	struct graphNode {
		int			id;
		dataType	val;
		graphNode 	*next;
	};

	int size;
	graphNode* newNode(dataType n1);
	// std::vector<graphNode*> graphNodeList;	
	// std::vector< std::vector<graphNode*> *> graphNodeList;
	std::vector< std::vector<int> *> graphNodeList;

public:
	graph();
	graph(int numOfNodes);
	void addNode(dataType n1);
	void addEdge(unsigned int id1, unsigned int id2);
	void printGraph();
	~graph();

	/* data */
};

#endif
