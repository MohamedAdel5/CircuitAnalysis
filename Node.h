#pragma once
#include "Element.h"
#include "DEFS.h"
class Node
{
	int NodeNumber;  //in other words node ID
	int nConNodes; // number of connected Nodes == Number of connected Elements
	int countNodes;
	int countElements;
	int* connectedNodes;
	Element* connectedElements;
	complex<double> voltage;
	
public:
	Node(void);
	void setNodeNum(int nodeNum);
	void setNumberOfConnectedNodes(int n); //sets(dynamically allocate)  connectedNodes array too.
	void setVoltage(complex<double> v);
	int getConnectedNodesNum() const;
	complex<double> getVoltage() const;
	void addConnectedNode(int nodeNum);
	void addConnectedElement(Element e_t);
	Element* getConnectedElementsArr();
	void print(); // for test purposes
	~Node(void);
};

