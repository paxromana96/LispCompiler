#pragma once
#include "token.h"
#ifndef NULL
#define NULL 0
#endif

/**
 * This file describes a struct type representing a node in a syntax tree.
 * It also includes functions to generate, manipulate, and print these nodes.
 */

#define SYNTAX_CAR_TYPE_EMPTY -1
#define SYNTAX_CAR_TYPE_TOKEN 1
#define SYNTAX_CAR_TYPE_SYNTAX_NODE 3
//The following two (_NUMBER and _STRING) are not yet implemented
#define SYNTAX_CAR_TYPE_NUMBER 4
#define SYNTAX_CAR_TYPE_STRING 5

#define EMPTY_SYNTAX_NODE_CDR NULL

/**
 * A small struct that represents a single node of an abstract syntax tree,
 * including both subtrees and atoms.
 * Whether this node is an atom or a subtree, and what sort of atom it is,
 * is determined by its carType.
 *
 * TODO: bundle the car and cdr together, since only non-atoms have a cdr
 *       and everything except SYNTAX_CAR_TYPE_SYNTAX_NODE is an atom.
 */
struct _syntaxnode {
	union {
		struct _syntaxnode * car;
		token * atom;
	};
	struct _syntaxnode * cdr;
	int carType;
} EMPTY_SYNTAX_NODE= {NULL,EMPTY_SYNTAX_NODE_CDR, SYNTAX_CAR_TYPE_EMPTY};

typedef struct _syntaxnode syntaxnode;

/**
 * Allocates a block of memory that can hold num syntaxnodes using malloc(),
 * and returns a pointer to the first one.
 * This can be used to allocate a single value, or an array.
 * These syntaxnodes are not initialized.
 */
syntaxnode * syntaxnodeAlloc(int num){
	return (syntaxnode *) malloc( num * sizeof(syntaxnode));
}

/**
 * Returns true if the given syntax node is the empty syntax node nil.
 * A syntax node is determined to be nil if its carType is SYNTAX_CAR_TYPE_EMPTY.
 *
 * Precondition: node is not NULL
 */
int isEmptySyntaxNode(syntaxnode * node){
	return node->carType == SYNTAX_CAR_TYPE_EMPTY;
}

/**
 * Initializes the given syntaxnode node to nil, the empty node.
 *
 * Precondition: node is not NULL
 */
void initializeToEmptySyntaxNode(syntaxnode * node){
	node->cdr = EMPTY_SYNTAX_NODE_CDR;
	node->carType = SYNTAX_CAR_TYPE_EMPTY;
}

/**
 * Allocates a single syntaxnode struct in heap memory,
 * initializes it to nil (the empty syntax node), and 
 * returns a pointer to it.
 */
syntaxnode * emptySyntaxnodeAlloc(){
	syntaxnode * node = syntaxnodeAlloc(1);
	initializeToEmptySyntaxNode(node);
	return node;
}

/**
 * Returns true if the given syntax node is an atom or not.
 * A syntax node is considered an atom if its carType is not
 * another syntax node. In use, this means that this syntax node
 * and its car are treated as the same thing; within the language,
 * the car is the actual object being manipulated. Such an atom
 * is represented as a syntax node for type consistency in the host
 * language, C.
 *
 * For example, the number 5 in the language would be represented
 * in this source code as a syntax node with a carType field of
 * SYNTAX_CAR_TYPE_NUMBER and a value field of 5.
 */
int isSyntaxNodeAnAtom(syntaxnode * n){
	return n == NULL || n->carType != SYNTAX_CAR_TYPE_SYNTAX_NODE;
}

/**
 * Prints out the value of the syntaxnode n, which is assumed to be an atom, to f.
 */
fprintSyntaxnodeAtom(FILE * f, syntaxnode * n){
	//Use a switch/case because of the enumerated car types like _TOKEN and _NUMBER
	switch(n->carType){
		case SYNTAX_CAR_TYPE_TOKEN:
			fprintTokenText(f, *(n->atom));
			break;
		default:
			fprintf(stderr, "%s\n", "Encountered syntaxnode atom with unexpected carType. Printing <Unexpected atom type>");
			fprintf(f,"%s\n","<Unexpected atom type>");
			break;
	}
}

/**
 * Prints out a textual representation of the syntax subtree n to the FILE f
 *
 * TODO: add support for quoting
 * TODO: make it not print nil at the ends of sublists when quoting
 */
void fprintSyntaxnode(FILE * f, syntaxnode * n){
	if(n == NULL){
		fprintf(f,"nil");
		return;
	}

	if(isEmptySyntaxNode(n)){ //nil
		//do nothing, assume end of list
		fprintf(f,"nil");
	} else if(isSyntaxNodeAnAtom(n)){
		//just print its value
		fprintSyntaxnodeAtom(f,n);
	} else if(n->carType == SYNTAX_CAR_TYPE_SYNTAX_NODE){
		//print car
		if(!isSyntaxNodeAnAtom(n->car))
			fprintf(f,"(");
		fprintSyntaxnode(f, n->car);
		if(!isSyntaxNodeAnAtom(n->car))
			fprintf(f,")");
		//print cdr
		if(n->cdr != NULL){
			fprintf(f," "); /** delimit by spaces, just like the source code of the language,
							 *  so that quoted trees end up on one line. */
			fprintSyntaxnode(f, n->cdr);
		}
	} else {
		fprintf(stderr, "%s\n", "Encountered syntaxnode non-atom of unexpected type. Printing <Unexpected non-atom type>");
		fprintf(f,"%s\n","<Unexpected non-atom type>");
	}
}



void printSyntaxnode(syntaxnode * n){
	fprintSyntaxnode(stdout, n);
}