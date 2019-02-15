#pragma once
#include "DEFS.h"
#include <iostream>
#include <complex>
using namespace std;
class Element
{
	int IDu, IDg, start, end;
	Element_Type type;
	double value;
	double phase;
	bool alternative;
	complex<double> current;
	complex<double> Z;
public:
	Element(void);
	void setIDu(int i);
	void setIDg(int i);
	void setStart(int s);
	void setEnd(int e);
	void setType(Element_Type e_t);
	void setValue(double v);
	void setAlt(bool a);
	void setPhase(double p);
	void setZ();
	void setCurrent(complex<double> i);
	

	complex<double> getZ(double w);
	complex<double> getCurrent();

	int getStart() const;
	int getEnd() const;
	int getIDu() const;
	double getValue() const;
	int getIDg() const;
	bool getAlt() const;
	double getPhase() const;
	bool isSource();
	Element_Type getType() const;
	bool grounded() const; // is connected to the ground
	void print() const;		// for test purposes
	~Element(void);
};

