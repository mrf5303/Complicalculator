// Machine Implementation File
// This implementation represents a simple machine that can evaluate
// arithmetic expressions.  In addition to the expected arithmetic
// operations, it includes relational operations and branching
// for the conditional operator, and simple support for function calls.
//
// The machine is expected to have a pre-allocated stack space
// for named variables and an arbitrarily long list of temporary
// registers, both represented by integer arrays here (to hold values).
// The machine also has a separate memory area holding all of the
// instructions defined below.

#include <iostream>
#include <sstream>
using namespace std;

#include "machine.h"

ostream& operator<<( ostream &stream, const Instruction &i )
{
    return stream << i.toString();
}

void Add::execute(int temp[], int stack[], int &stackPointer, int &programCounter) const
{
	temp[res] = temp[arg1] + temp[arg2];
		
}

string Add::toString() const
{
	string out = "Registers added together";
	return out;
}

void Subtract::execute(int temp[], int stack[], int &stackPointer, int &programCounter) const
{
	temp[res] = temp[arg1] - temp[arg2];
}

string Subtract::toString() const
{
	string out = "Registers subtracted";
	return out;
}

void Modulus::execute(int temp[], int stack[], int &stackPointer, int &programCounter) const
{
	temp[res] = temp[arg1] % temp[arg2];
}

string Modulus::toString() const
{
	string out = "Remainder found";
	return out;
}

void Multiply::execute(int temp[], int stack[], int &stackPointer, int &programCounter) const
{
	temp[res] = temp[arg1] * temp[arg2];
}

string Multiply::toString() const
{
	string out = "Registers multiplied together";
	return out;
}

void Divide::execute(int temp[], int stack[], int &stackPointer, int &programCounter) const
{
	temp[res] = temp[arg1] / temp[arg2];
}

string Divide::toString() const
{
	string out = "Registers dived";
	return out;
}
//  Here follow implementations of toString() and execute()
//  for all of the derived classes of the Instruction.

string Print::toString() const
{
	string out = "Print register";
	return out;
}

void Print::execute(int temp[], int stack[], int &stackPointer, int &programCounter) const
{
	cout << temp[valueTemp] << endl;

}

string Num::toString() const
{
	string out= "number into register";
	return out;

}

void Num::execute(int temp[], int stack[], int &stackPointer, int &programCounter) const
{
	temp[programCounter] = valueTemp;
}

string VtoR::toString() const
{
	string out = "Variable into register";
	return out;
}

void VtoR::execute(int temp[], int stack[], int &stackPointer, int &programCounter) const
{
	temp[programCounter] = stack[valueTemp];

}

string RtoV::toString() const
{
	string out = "register stored in variable";
	return out;
}

void RtoV::execute(int temp[], int stack[], int &stackPointer, int &programCounter) const
{
	stack[stackPointer++] = temp[valueTemp];
}







