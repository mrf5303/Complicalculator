// Variable Tree Implementation File
// This is a simple binary search tree associating variables
// with integer values.
#include <iostream>
#include <string>
using namespace std;

#include "vartree.h"
int recursiveCount(TreeNode *root, int &count);
//  recursiveSearch
//  A recursive tree-traversal function to search for a variable.
//  If the variable does not exist, it is created with a value of 0.
//  Parameters:
//  	node	(modified TreeNode ptr)	root of sub-tree to search
//  	name	(input string)		name of variable
//  NOTE: node May be a null pointer, indicating an empty tree
//  Post-condition:
//      return value points at a node with the given name,
//      even if it had to be created to do so
TreeNode* VarTree::recursiveSearch( TreeNode *&node, string name )
{

    if (node == NULL)				// not present
	node  = new TreeNode( name, 0 );		//    create with 0
    else if (node->name < name)
	return recursiveSearch( node->right, name );
    else if (node->name > name)
	return recursiveSearch( node->left, name );
    return node;

}

//  lookup
//  Searches for a variable to get its value
//  If the variable does not yet exist, it is created with value 0.
//  Parameters:
//  	name	(input char array)	name of variable
//  Returns:				value of variable
int VarTree::lookup( string name )
{
    TreeNode *node = recursiveSearch( root, name );
    return node->value;
}

//  assign
//  Assigns a value to a variable.
//  If the variable does not yet exist, it is created.
//  Parameters:
//  	name	(input string)		name of variable
//  	value	(input integer)		value to assign
void VarTree::assign( string name, int value )
{
    TreeNode *node = recursiveSearch( root, name );
    node->value = value;
}

int VarTree::size()
{
	int count = 1;
	if (root != NULL)
		recursiveCount(root, count);
	else
		count = 0;
	return count;
}

int VarTree::recursiveCount(TreeNode *root, int &count)
{
	
		
		if (root->right != NULL)
		{
			count++;
			recursiveCount(root->right, count);
		}
		if (root->left != NULL)
		{
			count++;
			recursiveCount(root->left, count);
		}
	
	
	return count;
}

//  EXTRA CREDIT:  Implement the following, without any loops

ostream& operator<<( ostream& stream, TreeNode &t )
{
    if (t.left != NULL)	stream << *t.left;
    stream << " " << t.name << "=" << t.value << " ";
    if (t.right != NULL) stream << *t.right;
    return stream;
}

ostream& operator<<( ostream& stream, VarTree &t )
{
    return stream << *t.root;
}

