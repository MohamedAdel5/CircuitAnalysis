#pragma once
#include "Input.h"
#include "Node.h"
#include <Eigen/Dense>
#define PI 3.14159265358979323846
using namespace Eigen;
class ApplicationManager
{
	Input in;
	int inputsNum; // == elements number
	int NodesNum;
	Element** eArr;
	Node** nArr;
	double W;
public:
	ApplicationManager(void);
	void setW(double w);
	void readInputs(); //must be invoked
	int getNodesNum();
	void setNodes(); //must be invoked

	int getVNum(); //gets number of voltage sources
	int getMatrixSize();
	MatrixXcd calcMatrix();
	void execute();
	void setVoltagesAndCurrents();
	void displayResult() const;

	void printElements(); //for test purposes
	void printNodes(); //for test purposes
	~ApplicationManager(void);
};

