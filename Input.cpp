#include "Input.h"

int Input::countInp = 0;
Input::Input()
{
	
}
int Input::getInpNum(){
	fstream Infile("input.txt", ios::out|ios::in);
	string s;
	 if(!Infile.is_open()){
		cout << "error openeing the file" << endl;
		return -1;						//for handling errors
	 }else{
		while(getline(Infile, s)){		//getting number of lines ( == number of inputs) in the file	//good() function is not used yet
			countInp++;
		 }
		return countInp;
	 }
}
Element* Input::readInput(int n){
	Element* newElement = new Element;
	 fstream Infile("input.txt", ios::out|ios::in);
	 string s;
	 if(!Infile.is_open()){
		cout << "error openeing the file" << endl;
	 }else{
		 
		 int i = 0;
		 while(getline(Infile, s)){
			if(i == n){
				break;
			}
			i++;
		 }
		
		istringstream iss(s);
		string temp;
		int count = 0;
		while(iss >> temp){		//getting number of inputs in the line
			count++;
		}
		iss.clear();
		iss.seekg(0, ios::beg);
		for (int j = 0; j < count; j++)
		{
			
			switch(j){
				case 0:
				{	
					iss >> temp; //instead of making a new string i used the temp one - created before for counting -
					if(setType(temp, newElement) == -1) exit(0);
					break;
				}
				case 1:
				{
					int x;
					iss>> x;
					newElement->setIDg(x);
					break;
				}
				case 2:
				{
					int x;
					iss>> x;
					newElement->setStart(x);
					break;
				}
				case 3:
				{
					int x;
					iss>> x;
					newElement->setEnd(x);
					break;
				}
				case 4:
				{
					double x;
					iss>> x;
					newElement->setValue(x);
					break;
					
				}
				case 5:
				{
					double x;
					iss>> x;
					newElement->setPhase(x);
					newElement->setAlt(true);
					break;
				}
				default:
				{
					exit(0);
					break;
				}
				}
			}
	 }	
	 return newElement;
}
int Input::setType(string t, Element* e){

	if(t == "Vs"){
		e->setType(Vs);
	}else if(t == "I"){
		e->setType(I);
	}else if(t == "L"){
		e->setType(L);
	}else if(t == "C"){
		e->setType(C);
	}else if(t == "R"){
		e->setType(R);
	}else if(t == "DVs"){
		e->setType(DVs);
	}else if(t == "DVs"){
		e->setType(DVs);
	}else{
		return -1;
	}
	return 1;
}
double Input::getOmega(){
	double x = 0;
	cin >> x;
	return x;
}
Input::~Input(void)
{
}
