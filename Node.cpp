#include "Node.h"


Node::Node(void)
{
	countNodes = 0;
	countElements = 0;
}
void Node::setNodeNum(int nodeNum){
	NodeNumber = nodeNum;
}
void Node::setNumberOfConnectedNodes(int n){
	nConNodes = n;
	connectedNodes = new int[n];
	connectedElements = new Element[n];
}
void Node::addConnectedNode(int nodeNum){
	connectedNodes[countNodes] = nodeNum;
	countNodes++;
}
void Node::addConnectedElement(Element e_t){
	connectedElements[countElements] = e_t;
	countElements++;
}
int Node::getConnectedNodesNum() const{
	return nConNodes;
}
Element* Node::getConnectedElementsArr(){
	return connectedElements;
}
void Node::print(){
	cout << "Node Number(ID): " << NodeNumber << "\ Connected Nodes: {";
	for (int i = 0; i < nConNodes; i++)
	{
		if(i != 0) cout << ",";
		cout << connectedNodes[i];
	}
	cout << "}\t Connected Elements: {";
	for (int i = 0; i < nConNodes; i++)
	{
		if(i != 0) cout << ",";
		cout << connectedElements[i].getType();
	}
	cout << "}" << endl;
}
void Node::setVoltage(complex<double> v){
	voltage = v;
}

complex<double> Node::getVoltage() const{
	return voltage;
}
Node::~Node(void)
{
}
