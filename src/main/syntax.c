#pragma once

#include "../syntax.c"

int main(int argc, char* argv[]){
	int errorcode = 0;
	//First, parse tokens from stdin
	struct parsenode program;
	if((errorcode = buildParseTree(stdin, &program)) != 0){
		puts("Error while parsing.");
		return errorcode;
	}
	puts("Successful parsing!");
	#ifdef DO_PRINT_RESULT_PARSE_TREE
		printParseNode(program);
	#endif

	//Second, generate a syntax tree from the given parse tree
	syntaxnode syntaxtree;
	if((errorcode = generateSyntaxTree(&program, &syntaxtree)) != 0){
		puts("Error while generating syntax tree.");
		return errorcode;
	}
	puts("\nSuccessful syntax tree generation!");
	#ifdef DO_PRINT_RESULT_SYNTAX_TREE 
		printSyntaxnode(&syntaxtree); 
	#endif

	return errorcode;
}