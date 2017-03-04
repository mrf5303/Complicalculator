//  Expression Tree Header File
//  Describes the elements of an expression tree, using
//  derived classes to represent polymorphism.
//  All objects in this structure are immutable --
//  once constructed, they are never changed.
//  They only be displayed or evaluated.
#include <iostream>
using namespace std;
#include "vartree.h"
#include "machine.h"
class ExprNode
{
    public:
    friend ostream& operator<<( ostream&, const ExprNode & );
    virtual string toString() const = 0;	// facilitates << operator
	virtual void instruct(VarTree &v, Instruction *program[], int &place) const = 0;
	virtual int evaluate(VarTree &v) const{ return 0; }
	// evaluate this node
};

class Value: public ExprNode
{
    private:
	int value;
    public:
	string toString() const;	// facilitates << operator
	void instruct(VarTree &v, Instruction *program[], int &place) const;
	int evaluate(VarTree &v) const;
	Value(int v)
	{
	    value = v;
	}
};

class Variable: public ExprNode
{
    private:
	string name;
    public:
	string toString() const ;	// facilitates << operator
	void instruct( VarTree &v , Instruction *program[], int &place ) const;
	int evaluate(VarTree &v) const;
	Variable(string var)
	{
	    name = var;
	}
};

class Operation: public ExprNode
{
    private:
	string oper;
	ExprNode *left, *right;	 // operands
    public:
	string toString() const;	// facilitates << operator
	void instruct(VarTree &v, Instruction *program[], int &place) const;
	int evaluate(VarTree &v) const;
	Operation( ExprNode *l, string o, ExprNode *r )
	{
	    left = l;
	    right = r;
	    oper = o;
	}
};

class Conditional: public ExprNode
{
    private:
	ExprNode *test, *trueCase, *falseCase;
    public:
	string toString() const;	// facilitates << operator
	void instruct(VarTree &v, Instruction *program[], int &place) const;
	int evaluate(VarTree &v) const;
	Conditional( ExprNode *b, ExprNode *t, ExprNode *f)
	{
	    test = b;
	    trueCase = t;
	    falseCase = f;
	}
};
