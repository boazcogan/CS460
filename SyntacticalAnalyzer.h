#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;



class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
	
    private:
	ofstream p2file;
	int literal();
	int quotedLit();
	int moreTokens();
	int paramList();
	int elsePart();
	int stmtPair();
	int stmtPairBody();
	int action();
	int anyOtherToken();
	LexicalAnalyzer * lex;
	token_type token;
	int program();
	int moreDefines();
	int define();
	int stmtList();
	int stmt();
	int * rules;
};
	
#endif
