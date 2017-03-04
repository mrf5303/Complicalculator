#include <iostream>
using namespace std;
#include "exprtree.h"
#include "tokenlist.h"
#include "funmap.h"
void buildAssign(ListIterator &iterator, ExprNode *&tree, TokenList Lst);
void buildFunction(ListIterator &iter, FunctionDef &funs, TokenList Lst);
int functionValue(ListIterator iter, FunctionDef funs);
int runFunction(string name, FunctionDef funs);
int solveTree(ExprNode *&node, VarTree &vars);
void compile(const char expr[], VarTree &vars, FunctionDef &funs,
	Instruction *prog[], int &pBegin, int &pEnd)
{

	TokenList Lst(expr);

	ListIterator iter = Lst.begin();
	ExprNode *root;
	if (iter.token().tokenText() == "deffn")
	{
		buildFunction(iter, funs, Lst);
	}
	else if (funs[iter.token().tokenText()].name == iter.token().tokenText())//can find function
	{
		functionValue(iter, funs);
	}
	else
	{
		buildAssign(iter, root, Lst);

	}
	pBegin = 0;
	root->instruct(vars, prog, pEnd);
	prog[pEnd++] = new Print(pEnd);
}
int functionValue(ListIterator iter, FunctionDef funs)
{
	string name = iter.token().tokenText();
	int i = 0;
	iter.advance();
	iter.advance();
	while (iter.tokenChar() == ')')
	{
		funs[name].locals->assign(funs[name].parameter[i], iter.integerValue());



		iter.advance();
		if (iter.tokenChar() == ',')
		{
			iter.advance();
		}
	}
	return runFunction(name, funs);
}

void buildFunction(ListIterator &iter, FunctionDef &funs, TokenList Lst)
{
	iter.advance();
	string name = iter.token().tokenText();

	VarTree locals;
	FunDef fundef;
	iter.advance();
	if (iter.tokenChar() == '(')
		iter.advance();
	for (int i = 0; i < 10 && iter.tokenChar() != ')'; i++)
	{
		fundef.parameter[i] = iter.tokenChar();
		iter.advance();
		if (iter.tokenChar() == ',')
		{
			iter.advance(); //get past comma
		}
	}
	for (int i = 0; i < 10 && isalpha(fundef.parameter[i][0]); i++)
	{
		fundef.locals->lookup(fundef.parameter[i]);

	}
	iter.advance();
	iter.advance();
	ExprNode *def;
	buildAssign(iter, def, Lst);

	fundef.functionBody = def;
	fundef.name = name;
	funs[name] = fundef;
}
