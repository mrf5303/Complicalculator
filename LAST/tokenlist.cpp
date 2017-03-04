// Token List Implementation file
//
// This tokenizer will scan a character string representing
// an expression, and will return a list of tokens.
// --- only integers and operators are supported (no names)

// The standard C library has some useful functions for us
#include <string.h>
#include <ctype.h>

// And to get the definition of a token:
#include "tokenlist.h"

// First, a helper function that identifies a single token
// from the character string.
  
// getToken:
// Identifies the token found at the given character position
// Parameters:
// 	str	(modified char pointer)		position to scan
//	token	(output Token)			the found token
// The character pointer is advanced past the scanned token.
// If no more non-space data is found, a null pointer is returned.
void getToken( const char *&str, Token &token  )
{
    string temp;			// temporary text to work with

    while (isspace(*str))		// look for a non-space
	str++;
    if (*str == 0)			// at end of string
    {
	token = Token();
    }
    else if (isdigit(*str))
    {
	token = Token(atoi(str));		// convert to integer
	while (isdigit(*str))
	    str++;			// scan past the intie
    }
    else if (isalpha(*str))
    {
	string name = "";
	while (isalnum(*str))		// collect the letters in the name
	{				//    (or digits after a  letter)
	    name += *str;
	    str++;
	}
	token = Token(name);		// store the name
    }
    else
    {
	string op = "";
	op += *str;			// copy an operator
	str++;				// and move on
	if (*str == '=')		// handle != == >= <=
	{
	    op += '=';
	    str++;
	}
	token = Token(op);		// copy the operator
    }
}
 
// tokenize
// converts a character string into a list of tokens
// Parameter:
// 	expr	(input char pointer)	// string to examine
// Returns:	(Token * pointer)	// pointer to first in linked list
// Pre-condition:  str may not be a null pointer
//     and is assumed to actually point at a valid expression.
TokenList::TokenList( const char expr[])
{
    head = NULL;			// clear these here too
    tail = NULL;
    Token current;
    const char *str = expr;		// a copy of the pointer to work with
    getToken( str, current );		// get the first token
    while (!current.isNull())
    {
	push_back( current );
	getToken( str, current );	// get another token
    }
}

//  output operation
//  Display all of the tokens in the list
ostream& operator<<( ostream &stream, TokenList &t )
{
    ListElement *curr;
    for (curr = t.head; curr != NULL; curr = curr->next)
    {
	stream << " " << curr->token;
    }
    return stream;
}

//  Creates an iterator to refer to the beginning of the list
ListIterator TokenList::begin()
{
    return ListIterator( this, head );
}

//  Creates an iterator to refer after the end of the list
//  This list is simple:  it ends with NULL
ListIterator TokenList::end()
{
    return ListIterator( this, NULL );
}
void TokenList::push_back(Token t)
{
	ListElement *l = new ListElement;
	l->token = t;
	l->next = NULL;
	if (tail == NULL)
		tail = head = l;
	else
		tail = tail->next = l;
}

//  Add a new element to the front of the list
void TokenList::push_front(Token t)
{
	ListElement *l = new ListElement;
	l->token = t;
	l->next = head;
	head = l;
	if (tail == NULL)
		tail = l;
}

//  Remove and return the first element of the list
Token TokenList::pop_front()
{
	ListElement *l = head;
	Token t = l->token;
	head = l->next;
	delete l;
	return t;
}
