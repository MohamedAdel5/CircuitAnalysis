#include "Element.h"


Element::Element(void)
{
	alternative = false;
}
void Element::setIDu(int i){
	IDu = i;
}
void Element::setIDg(int i){
	IDg = i;
}
void Element::setStart(int s){
	start = s;
}
void Element::setEnd(int e){
	end = e;
}
void Element::setType(Element_Type e_t){
	type = e_t;
}
void Element::setValue(double v){
	value = v;
}
void Element::setAlt(bool a){
	alternative = a;
}
void Element::setPhase(double p){
	phase = p;
}
void Element::setCurrent(complex<double> i){
	current = i;
}
complex<double> Element::getCurrent(){
	return current;
}

complex<double> Element::getZ(double w){
	if(type == 2) return complex<double> (0, w * value);
	else if(type == 3) return complex<double> (0, -1.0/(w * value));
	else if(type == 4) return complex<double>(value,0);
}
int Element::getStart() const{
	return start;
}
int Element::getEnd() const{
	return end;
}
Element_Type Element::getType() const{
	return type;
}
int Element::getIDu() const{
	return IDu;
}
int Element::getIDg() const{
	return IDg;
}
double Element::getValue() const{
	return value;
}
bool Element::getAlt() const{
	return alternative;
}
double Element::getPhase() const{
	return phase;
}
bool Element::grounded()const{
	return (start == 0 || end == 0)? true :false;
}
bool Element::isSource(){
	if(getType() == 2 || getType() == 3 || getType() == 4){
		return false;
	}
	return true;
}
void Element::setZ(){
	if(!isSource()){
		Z = polar(value, phase);
	}
}
void Element::print() const{
	
	cout << "Type: " << type << "\t IDu: " << IDu << "\t IDg: " << IDg << "\t start: " << start << "\t end: "  << end << "\t value: "  << value << "\t alternative: "   << " " << alternative << "\t phase:  " ;
	if(alternative)
	{cout << phase << endl;}
	else{cout << "no phase" << endl;}
	
}
Element::~Element(void)
{
}
