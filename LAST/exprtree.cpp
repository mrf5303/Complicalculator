// Expression Tree Implementation File
// These functions manage an expression tree that describes
// arithmetic expressions using integer variables and values.
// Some functions construct a tree from a character string that
// represents the expression, and then the tree can be traversed
// and evaluated.
#include <iostream>
#include <sstream>
using namespace std;
#include "exprtree.h"
#include "tokenlist.h"
#include "funmap.h"
// An assignment may assign any value (boolean or integer) to a variable
// A conditional expression tests a boolean, and evaluates one of two expr's
// An equality expression compares two integer or booleans for equality
// A relational expression compares to sum expressions 
// A sum expression is the sum or difference of one or more products.
// A product expression is the product or quotient of one or more factors.
// A factor may be a number or a parenthesized assignment expression.

void buildAssign(ListIterator &, ExprNode *&, TokenList);
void buildConditional(ListIterator &, ExprNode *&, TokenList);
void buildEquality(ListIterator &, ExprNode *&, TokenList);
void buildRelational(ListIterator &, ExprNode *&, TokenList);
void buildSum(ListIterator &, ExprNode *&, TokenList);	// prototypes all here
void buildProduct(ListIterator &, ExprNode *&, TokenList);	// since they are mutually
void buildFactor(ListIterator &, ExprNode *&, TokenList);	// recursive
int solveTree(ExprNode &node, VarTree &vars);
int runFunction(string name, FunctionDef funs);
// Outputting any tree node will simply output its string version
ostream& operator<<( ostream &stream, const ExprNode &e )
{
    return stream << e.toString();
}

// A Value is just an integer value -- easy to evaluate
// Unfortunately, the string class does not have a constuctor for it
string Value::toString() const
{
    ostringstream convert;	// output string stream
    convert << value;		// 'output' into the stream
    return convert.str();	// and extract its string equivalent
}

int Value::evaluate(VarTree &v) const
{
	return value;
}

void Value::instruct(VarTree &v, Instruction *program[], int &place) const
{
	program[place++] =  new Num(value);
	
}

//  A variable is just an alphabetic string -- easy to display
//  TO evaluate, would need to look it up in the data structure
string Variable::toString() const
{
    return name;
}

int Variable::evaluate(VarTree &v) const
{
	return v.lookup(name);
}

void Variable::instruct(VarTree &v, Instruction *program[], int &place) const
{
	program[place++] = new VtoR(name[0]);
	
}

//  An operation is a computation working on two values
//  All binary operators are included here, both arithmetic and relational
//  The assignemnt operator just works a little differently
string Operation::toString() const
{
    return "(" + left->toString() + " " + oper + " " + right->toString() + ")";
}

int Operation::evaluate(VarTree &v) const
{
	int returnVal = 0;
	if (oper == "=")		// assignment operator
	{
		returnVal = right->evaluate(v);		// returns asigned value
		v.assign(left->toString(), returnVal); // as well as assigning
	}
	else switch (oper[0])
	{
	case '+': returnVal = left->evaluate(v) + right->evaluate(v); break;
	case '-': returnVal = left->evaluate(v) - right->evaluate(v); break;
	case '*': returnVal = left->evaluate(v) * right->evaluate(v); break;
	case '/': returnVal = left->evaluate(v) / right->evaluate(v); break;
	case '%': returnVal = left->evaluate(v) % right->evaluate(v); break;
	case '=': returnVal = left->evaluate(v) == right->evaluate(v); break;
	case '!': returnVal = left->evaluate(v) != right->evaluate(v); break;
	case '<': if (oper == "<")
		returnVal = left->evaluate(v) < right->evaluate(v);
			  else
				  returnVal = left->evaluate(v) <= right->evaluate(v);
		break;
	case '>': if (oper == ">")
		returnVal = left->evaluate(v) > right->evaluate(v);
			  else
				  returnVal = left->evaluate(v) >= right->evaluate(v);
		break;
	}
	return returnVal;
}

void Operation::instruct(VarTree &v, Instruction *program[], int &place) const
{
	
	left->instruct(v, program, place);
	int first = place;
	right->instruct(v, program, place);
	int second = place;
    if (oper == "=")		// assignment operator
    {
		char name = left->toString()[0];
		program[place++] = new RtoV(name); //left has to be variable
		program[place++] = new VtoR(name); // back into register
	
    }
    else switch(oper[0])
    {
	case '+': program[place++]= new Add(place,first,second) ; break;
	case '-': program[place++] = new Subtract(place, first, second); break;
	case '*': program[place++] = new Multiply(place, first, second); break;
	case '/': program[place++] = new Divide(place, first, second); break;
	case '%': program[place++] = new Modulus(place, first, second); break;
    }
    
}

//  A conditional expression is of the form (test ? true : false)
//  Choosing one of two expressions to evaluate after the test.
string Conditional::toString() const
{
    return "(" + test->toString() +  " ? " +
		trueCase->toString() + " : " + falseCase->toString() + ")";
}

int Conditional::evaluate(VarTree &v) const
{
	if (test->evaluate(v) == 0)
		return falseCase->evaluate(v);
	else
		return trueCase->evaluate(v);
}


void Conditional::instruct(VarTree &v, Instruction *program[], int &place) const
{
  /*  if (test->evaluate(v) == 0)
	return falseCase->evaluate(v);
    else
	return trueCase->evaluate(v);*/
}

//  And now all of the functions that build the tree,
//  in no special order

//  Build Assign
//  Parses a possible assignment expression (may have assignment operator)
//  Parameters:
// 	iterator (modified list iterator)	expression toe examine
// 	tree (output ExprNode pointer)		resulting tree
//  Pre-conditions:
//  	the input is a valid arithmetic expression
//  	it need not consist of an assignment operator,
//  	but if it does, a variable must be the left-hand operand
//  	This latter condition is not tested here.
void buildAssign(ListIterator &iterator, ExprNode *&tree, TokenList Lst)
{
    ExprNode *left, *right;		// left and right subtres
    buildConditional( iterator, left,Lst );		// find an expression
	if (iterator != Lst.end() && iterator.token().tokenChar() == '=')
    {
	iterator.advance();
	buildAssign( iterator, right,Lst );
	left = new Operation( left, "=", right );
    }
    tree = left;		// all done
}

// BuildSum
// Parses a sum expression: the sum or difference of one or more products
// placing the result into a tree.
// There may be the possibility of a leading - that would be implicitly
// subtracting the first product from zero.
void buildSum(ListIterator &iterator, ExprNode *&tree, TokenList Lst)
{
    string oper;				// possible operation
    ExprNode *left, *right;			// left and right subtrees

    if (iterator.token().tokenChar() == '-')	// if negative
    {
	iterator.advance();			//    go past the negative sign
	buildProduct(iterator, right, Lst);	//       build a tree 
	left = new Operation( new Value(0), "-", right );
    }
    else
		buildProduct(iterator, left, Lst);

	if (iterator != Lst.end())			// make sure there is more data
   	oper = iterator.token().tokenText();
    while (oper == "+" || oper == "-")
    {
	iterator.advance();			// get past the operator
	buildProduct(iterator, right, Lst);	// build a tree	
	left = new Operation( left, oper, right );
	if (!(iterator != Lst.end()))			// no more operators
	    oper.clear();
	else
   	    oper = iterator.token().tokenChar();
    }

    tree = left;		// all done
}

// BuildProduct
// Parses a product expression: the product or quotient of factors
// Parameters:
// 	iterator (modified list iterator)	expression toe examine
// 	tree (output ExprNode pointer)		resulting tree
void buildProduct(ListIterator &iterator, ExprNode *&tree, TokenList Lst)
{
    string oper;			// possible operation
    ExprNode *left, *right;		// left and right subtrees

	buildFactor(iterator, left, Lst);

	if (iterator != Lst.end())			// make sure there is more data
   	oper = iterator.token().tokenText();
    while (oper == "*" || oper == "/" || oper == "%")
    {
	iterator.advance();			// get past the operator
	buildFactor(iterator, right, Lst);		// build a tree	
	left = new Operation( left, oper, right );
	if (iterator != Lst.end())			// no more operators
	    oper.clear();
	else
   	    oper = iterator.token().tokenText();
    }

    tree = left;		// all done
}

// Evaluate Factor
// A factor may be an integer, a variable, or a parenthesized expression.
void buildFactor(ListIterator &iterator, ExprNode *&tree, TokenList Lst)
{
    Token y = iterator.token();
    if ( iterator.token().isInteger() )
    {
	tree = new Value(iterator.token().integerValue());
	iterator.advance();		// get past the digit
    }
    else if ( isalpha(y.tokenChar()))
    {
	tree = new Variable(iterator.token().tokenText());
	iterator.advance();
    }
    else
    {
	iterator.advance();		// go past assumed (
	buildAssign(iterator, tree, Lst);
	iterator.advance();		// go past assumed )
    }
}

// BuildEquality
// Parses an equality expression: using the == or != operators
// Parameters:
// 	iterator (modified list iterator)	expression toe examine
// 	tree (output ExprNode pointer)		resulting tree
void buildEquality(ListIterator &iterator, ExprNode *&tree, TokenList Lst)
{
    string oper;			// possible operation
    ExprNode *left, *right;		// left and right subtrees

	buildRelational(iterator, left, Lst);

	if (iterator != Lst.end())			// make sure there is more data
   	oper = iterator.token().tokenText();
    while (oper == "==" || oper == "!=")
    {
	iterator.advance();			// get past the operator
	buildRelational(iterator, right, Lst);	// build a tree	
	left = new Operation( left, oper, right );
	if (iterator != Lst.end())			// no more operators
	    oper.clear();
	else
   	    oper = iterator.token().tokenText();
    }

    tree = left;		// all done
}

// BuildRelational
// Parses a relational expression: comparison using < <= > or >=
// Parameters:
// 	iterator (modified list iterator)	expression toe examine
// 	tree (output ExprNode pointer)		resulting tree
void buildRelational(ListIterator &iterator, ExprNode *&tree, TokenList Lst)
{
    string oper;			// possible operation
    ExprNode *left, *right;		// left and right subtrees

	buildSum(iterator, left, Lst);

	if (iterator != Lst.end())			// make sure there is more data
   	oper = iterator.token().tokenText();
    while (oper == ">" || oper == ">=" || oper == "<" || oper == "<=")
    {
	iterator.advance();			// get past the operator
	buildSum(iterator, right, Lst);		// build a tree	
	left = new Operation( left, oper, right );
	if (iterator != Lst.end())			// no more operators
	    oper.clear();
	else
   	    oper = iterator.token().tokenText();
    }

    tree = left;		// all done
}

// BuildConditional
// Parses a conditional expression: possibly using ? and :
// Parameters:
// 	iterator (modified list iterator)	expression toe examine
// 	tree (output ExprNode pointer)		resulting tree
void buildConditional(ListIterator &iterator, ExprNode *&tree, TokenList Lst)
{
    string oper;			// possible operation
    ExprNode *test, *trueCase, *falseCase;	// three subtrees

	buildEquality(iterator, test, Lst);	// get possibly boolean expression

    if (iterator!=Lst.end())			// make sure there is more data
   	oper = iterator.token().tokenText();
    while (oper == "?")				// actually is conditional
    {
	iterator.advance();			// get past the operator
	buildEquality(iterator, trueCase, Lst);	// get the true case
	iterator.advance();			// get past the assumed :
	buildEquality(iterator, falseCase, Lst);	// get the false case
	test = new Conditional( test, trueCase, falseCase );
	if (iterator != Lst.end())			// no more operators
	    oper.clear();
	else
   	    oper = iterator.token().tokenText();
    }

    tree = test;		// all done
}


int runFunction(string name, FunctionDef funs)
{
	return funs[name].functionBody->evaluate(*funs[name].locals);
}

int solveTree(ExprNode *&node, VarTree &vars)
{
	if (node == NULL)
	{
		return 0;
	}
	else
	{
		return node->evaluate(vars);
	}
}
