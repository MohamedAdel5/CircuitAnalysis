#pragma once
#include "DEFS.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include<sstream>
#include "Element.h"
class Input
{
	
	static int countInp;
public:
	Input();
	int getInpNum();
	double getOmega();
	Element* readInput(int n);
	int setType(string t, Element* e);
	~Input(void);
};

