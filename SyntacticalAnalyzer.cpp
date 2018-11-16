#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"
#include "LexicalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	token_type t;
	// program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}

int SyntacticalAnalyzer::literal()
{
	int errors = 0;
	if (token == NUMLIT_T || token == STRLIT_T)
	{
		// Apply rule 10 or 11
		token = lex->GetToken();
	}
	else if (token == SQUOTE_T)
	{
		// Apply rule 
		token = lex->GetToken();
		errors += action();		
	}
}