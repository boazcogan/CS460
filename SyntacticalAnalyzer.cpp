#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"
#include "LexicalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer(char *filename)
{
	lex = new LexicalAnalyzer(filename);
	token_type t;
	// program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer()
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
		errors += quotedLit();
	}
	else 
	{
		errors++;
		lex->ReportError("Terminal: Literal token expected; '" + lex->GetTokenName(token) + "' found.");
	}
}

int SyntacticalAnalyzer::quotedLit()
{
	int errors = 0;
	if (token != IDENT_T && token != NUMLIT_T && token != STRLIT_T && token != CONS_T && token != IF_T && token != DISPLAY_T && token != NEWLINE_T && token != LISTOP_T && token != AND_T && token != OR_T && token != NOT_T && token != DEFINE_T && token != NUMBERP_T && token != LISTP_T && token != ZEROP_T && token != NULLP_T && token != STRINGP_T && token != PLUS_T && token != MINUS_T && token != DIV_T && token != MULT_T && token != MODULO_T && token != ROUND_T && token != EQUALTO_T && token != GT_T && token != LT_T && token != GTE_T && token != LTE_T && token != COND_T && token != ELSE_T)
	{
		// Rule 13 Used
		errors += anyOtherToken();
	}
	else
	{
		errors++;
		lex->ReportError("Terminal: QuotedLit token expected; '" + lex->GetTokenName(token) + "' found.");
	}
	return errors;
}

int SyntacticalAnalyzer::moreTokens()
{
	int errors = 0;
	if (token != IDENT_T && token != NUMLIT_T && token != STRLIT_T && token != CONS_T && token != IF_T && token != DISPLAY_T && token != NEWLINE_T && token != LISTOP_T && token != AND_T && token != OR_T && token != NOT_T && token != DEFINE_T && token != NUMBERP_T && token != LISTP_T && token != ZEROP_T && token != NULLP_T && token != STRINGP_T && token != PLUS_T && token != MINUS_T && token != DIV_T && token != MULT_T && token != MODULO_T && token != ROUND_T && token != EQUALTO_T && token != GT_T && token != LT_T && token != GTE_T && token != LTE_T && token != COND_T && token != ELSE_T)
	{
		// Rule 14 Used
		errors += anyOtherToken();
		errors += moreTokens();
	}
	else if (token == RPAREN_T)
	{
		// Rule 15 used
		// Lambda
	}
	else
	{
		errors++;
		lex->ReportError("Terminal: AnyOtherToken token expected; '" + lex->GetTokenName(token) + "' found.");
	}
	return errors;
}

int SyntacticalAnalyzer::paramList()
{
	int errors = 0;
	if (token == IDENT_T)
	{
		errors += paramList();
	}
	else if (token == RPAREN_T)
	{

	}
	else
	{
		errors++;
		lex->ReportError("Terminal: ParamList token expected; '" + lex->GetTokenName(token) + "' found.");
	}
}

int SyntacticalAnalyzer::elsePart()
{
	int errors = 0;
	if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		errors += stmt();
	}
	else
	{
		errors++;	
		lex->ReportError("Terminal: ElsePart token expected; '" + lex->GetTokenName(token) + "' found.");
	}
}
