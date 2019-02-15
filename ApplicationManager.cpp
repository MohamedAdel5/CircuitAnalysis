#include "ApplicationManager.h"


ApplicationManager::ApplicationManager(void)
{
	inputsNum = in.getInpNum();
	eArr = new Element*[inputsNum];
	
}
void ApplicationManager::readInputs(){
	for (int i = 0; i < inputsNum; i++)
	{
		eArr[i] = in.readInput(i);
		eArr[i]->setIDu(i+1);
	}
}
void ApplicationManager::setW(double w){
	W = w;
}
int ApplicationManager::getNodesNum(){
	int maxNodeNum = 0;
	for (int i = 0; i < inputsNum; i++)
	{
		if(eArr[i]->getStart() > maxNodeNum || eArr[i]->getEnd() > maxNodeNum)
			maxNodeNum = ( (eArr[i]->getStart()) >= (eArr[i]->getEnd()) )? (eArr[i]->getStart()) : (eArr[i]->getEnd()) ;
	}
	NodesNum = maxNodeNum+1;
	return maxNodeNum+1;
}
int ApplicationManager::getVNum(){
	int count = 0;
	for (int i = 0; i < inputsNum; i++)
	{
		if(eArr[i]->getType() == 0 || eArr[i]->getType() == 5) count++;
	}
	return count;
}
void ApplicationManager::setNodes(){
	nArr = new Node*[getNodesNum()];
///////// Allocating each Node and setting the node number (node ID) for each of them /////////////

	for (int i = 0; i < NodesNum; i++)
	{
		nArr[i] = new Node;
		nArr[i]->setNodeNum(i);
	}

///////// finding the number of connected nodes for each Node ///////////
	for (int i = 0; i < NodesNum; i++)
	{
		int counter = 0;
		for (int j = 0; j < inputsNum; j++)
		{
			if(eArr[j]->getStart() == i || eArr[j]->getEnd() == i){
				counter++;
			}

		}
		nArr[i]->setNumberOfConnectedNodes(counter);
	}
///////////setting the connected nodes & the connected elements for each Node///////////////////

//Note!
// if there are two or more parallel elements between two nodes, the connected node will be repeated 
//in the connectedNode array and the number of connected nodes will be increased

	for (int i = 0; i < NodesNum; i++)
	{
		for (int j = 0; j < inputsNum; j++)
		{
			if(eArr[j]->getStart() == i || eArr[j]->getEnd() == i){
				if(eArr[j]->getStart() == i) {
					nArr[i]->addConnectedNode(eArr[j]->getEnd());
				}else{
					nArr[i]->addConnectedNode(eArr[j]->getStart());
				}
				nArr[i]->addConnectedElement(*eArr[j]); 
			}

		}
	}

}
int ApplicationManager::getMatrixSize(){
	int size = NodesNum - 1; //without counting the ground
	for (int i = 0; i < inputsNum; i++)
	{
		if(eArr[i]->getType() == 0 || eArr[i]->getType() == 5) size++;
	}
	return size;
}
MatrixXcd ApplicationManager::calcMatrix(){
	/*This article illustrates the algorithm in detail [Modified nodal analysis]: https://www.swarthmore.edu/NatSci/echeeve1/Ref/mna/MNA2.html?fbclid=IwAR3Pgi26FHCiQGmVD9Yp14RV4Fw7DyZ_uoiWUqqfZ4s-dHCFAmlQWJGL_bE#Introduction */

	//initialize the big matrix A:
	/*
			 _		_
			|G 	   B |
		A = |C	   D |  where G, B, C, D are sub matrices of the big matrix A
			|_		_|
	*/

	int matrixSize = getMatrixSize();
	MatrixXcd A;
	A.resize(matrixSize, matrixSize);
	A.setZero(A.rows(), A.cols());

	// All matrices are initialized as zero matrices

	//initialize the sub matrix G:

	MatrixXcd G;
	G.resize(NodesNum - 1, NodesNum - 1);
	G.setZero(G.rows(), G.cols());

	//initialize the sub matrix B:
	MatrixXcd B;
	B.resize(NodesNum-1, getVNum());
	B.setZero(NodesNum-1, getVNum());

	//filling matrices G & B:-

	for (int i = 1; i <= NodesNum-1; i++) //looping starts from 1 --> not counting the ground
	{
		Element* connectedElements = nArr[i]->getConnectedElementsArr();

		//filling G:-
		for (int j = 1; j <= G.cols(); j++)
		{
			complex<double> temp(0.0, 0.0);
			if(i == j){
				for (int k = 0; k < nArr[i]->getConnectedNodesNum(); k++)
				{
					
					if(!connectedElements[k].isSource()){
						temp += 1.0/(connectedElements[k].getZ(W));
					}
					
				}
				G(i-1,i-1) = temp;
			}else{
				for (int k = 0; k < nArr[i]->getConnectedNodesNum(); k++)
				{
					if((connectedElements[k].getStart() == j || connectedElements[k].getEnd() == j) && !connectedElements[k].isSource()){
						temp += 1.0/(connectedElements[k].getZ(W));
						
					}
				}
				G(i-1,j-1) = -1.0 * temp;
			}

		}

		// filling B:-
		for (int j = 0; j < nArr[i]->getConnectedNodesNum(); j++)
		{
			if(connectedElements[j].getType() == 0 || connectedElements[j].getType() == 5){
				int temp = (connectedElements[j].getStart() == i)?1:-1;
				B(i-1, connectedElements[j].getIDg()-1) = temp;
			}
		}
	}


	/*cout << "G-----------------------" << endl << endl;
	cout << G << endl;
	cout << "B-----------------------" << endl << endl;
	cout << B << endl;
	cout << "-----------------------" << endl << endl;*/


	// initializing and filling matrix C:-
	MatrixXcd C = B.transpose();

	/*cout << C << endl;*/

	//The Sub matrix D is not declared since it is always a zero matrix [in case there are no dependent sources]

	//Assigning the sub matrices G,B,C,D to big matrix A:-
	for (int i = 0; i < G.rows(); i++)
	{
		int col = 0;
		for (int j = 0; j < G.cols(); j++)
		{
			A(i,j) = G(i,j);
			col++;
		}
		for (int j = 0; j < B.cols(); j++)
		{
			A(i,col) = B(i,j);
			col++;
		}
	}
	for (int i = G.rows(); i < A.rows(); i++)
	{
		for (int j = 0; j < C.cols(); j++)
		{
			A(i,j) = C(i-G.rows(),j);
		}
	}
	/*cout << "A-----------------------" << endl << endl;
	cout << A << endl;*/

	// initializing matrix Z --> Ax=Z , where the unknown values are in the matrix x
	MatrixXcd Z;
	Z.resize(A.rows(),1);
	Z.setZero(A.rows(),1);


	//Filling matrix Z:-
	complex<double> test;
	for (int i = 1; i <= NodesNum-1; i++)
	{
		Element* connectedElements = nArr[i]->getConnectedElementsArr();
		for (int j = 0; j < nArr[i]->getConnectedNodesNum(); j++)
		{
			if (connectedElements[j].getType() == 1)
			{
				int sign = (connectedElements[j].getStart() == i)?-1:1;
				if(connectedElements[j].getAlt()){
					Z(i-1,0) = polar(sign * connectedElements[j].getValue(),connectedElements[j].getPhase());
				}
				else{
					Z(i-1,0) = complex<double>(sign * connectedElements[j].getValue(),0);
				}
				test = Z(i-1,0);
			}
		}
	}
	Element** arrangedVs = new Element*[getVNum()]; 
	for (int i = 0; i < inputsNum; i++)
	{
		if(eArr[i]->getType() == 0) {
			arrangedVs[eArr[i]->getIDg()-1] = eArr[i];				/*Arranging the voltage sources to make it easy to handle*/
		}
	}
	for (int i = G.rows(); i < A.rows(); i++)
	{
		if (arrangedVs[i-G.rows()]->getAlt())
		{
			Z(i,0) = polar(arrangedVs[i-G.rows()]->getValue(), arrangedVs[i-G.rows()]->getPhase());
		}else{
			Z(i,0) = complex<double>(arrangedVs[i-G.rows()]->getValue(), 0);
		}
		test = Z(i,0);
		
	}
	/*cout << "Z-----------------------" << endl;
	cout << Z << endl;
	
	
	cout << "X-----------------------" << endl;*/

	//Finally calculating the matrix x from the euqation---> Ax=Z
	MatrixXcd x =A.colPivHouseholderQr().solve(Z);
	/*cout << x << endl;*/
	return x;
	
}
void ApplicationManager::setVoltagesAndCurrents(){
	MatrixXcd x = calcMatrix();
	nArr[0]->setVoltage(0);
	for (int i = 1; i <= NodesNum-1; i++)
	{
		nArr[i]->setVoltage(x(i-1,0));
	}
	for (int i = 0; i < inputsNum; i++)
	{
		if (eArr[i]->getType() == 0)
		{
			eArr[i]->setCurrent(x(NodesNum-2+eArr[i]->getIDg(),0));
		}else if (eArr[i]->getType() == 1)
		{
			if(eArr[i]->getAlt()){
				eArr[i]->setCurrent(polar(eArr[i]->getValue(), eArr[i]->getPhase()));
			}
			else{
				eArr[i]->setCurrent(eArr[i]->getValue());
			}
		}else{
			eArr[i]->setCurrent((nArr[eArr[i]->getStart()]->getVoltage()-nArr[eArr[i]->getEnd()]->getVoltage())/eArr[i]->getZ(W));
		}
	}
}
void ApplicationManager::displayResult() const{
	cout << "Node Voltages:-" << endl << "------------------------" << endl;
	for (int i = 0; i < NodesNum; i++)
	{
		complex<double> volt = nArr[i]->getVoltage();
		if(volt.imag() == 0)								// to solve the -0 issue in complex class
			volt = complex<double>(volt.real(), 0);
		if(volt.real() == 0)
			volt = complex<double>(0, volt.imag());


		cout << "V(" << i << ") = " << volt << " abs: " << abs(volt) << " Phase:  " << arg(volt)*180/PI << endl;
	}
	cout << endl << "----------------------------------" << endl;
	cout << "Currents in each element:-" << endl << "------------------------" << endl;
	for (int i = 0; i < inputsNum; i++)
	{
		complex<double> current = eArr[i]->getCurrent();
		if(current.imag() == 0)
			current = complex<double>(current.real(), 0);
		if(current.real() == 0)
			current = complex<double>(0, current.imag());


		cout << "I(" << eArr[i]->getStart() << ", " << eArr[i]->getEnd() << ") = " << current <<  " abs: " << abs(current) << " Phase: " << arg(current)*180/PI << endl;

	}
	cout << endl << "----------------------------------" << endl;

}

void ApplicationManager::execute(){
	cout << "NOTE! : You MUST read the manual of the application to be able to deal with it properly in order to get the correct results" << endl;
	cout << "------------------------------------------------------------" << endl << endl;
	cout << "Please Enter the value of the Angular velocity (Omega) - if None Enter \"0\" " << endl;
	W = in.getOmega();
	cout << "---------------------------------------------------------------" << endl;
	readInputs();
	setNodes();
	setVoltagesAndCurrents();
	displayResult();
	cout << endl << endl << endl << "--------------------------------------------------------------------" << endl;
	cout << "Given Elements: " << endl;
	printElements();
	cout << "----------------------------------------------------------------------------" << endl;
	cout << "Given Nodes: " << endl;
	printNodes();
}
void ApplicationManager::printElements(){
	for (int i = 0; i < inputsNum; i++)
	{
		eArr[i]->print();
	}
}
void ApplicationManager::printNodes(){
	for (int i = 0; i < NodesNum; i++)
	{
		nArr[i]->print();
	}
}
ApplicationManager::~ApplicationManager(void)
{
	delete [] eArr;
	for (int i = 0; i < NodesNum; i++)
	{
		delete nArr[i];
	}
	delete [] nArr;
}
