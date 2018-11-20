#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"
#include "LexicalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer(char *filename)
{

	lex = new LexicalAnalyzer(filename);
	token = lex->GetToken();
	int errors = 0;
	errors += program();
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
}

int SyntacticalAnalyzer::program()
{
	int errors = 0;
	if (token == LPAREN_T)
	{
		token = lex->GetToken();
		errors += define();
		if (token == LPAREN_T)
		{
			token = lex->GetToken();
			errors += moreDefines();
			if (token == EOF_T)
			{
				return errors;
			}
			else
			{
				lex->ReportError("Terminal: EOF_T token expected; '" + lex->GetTokenName(token) + "' found.");
				errors++;
			}
		}
		else
		{
			lex->ReportError("Terminal: LPAREN_T token expected; '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
	}
	else
	{
		lex->ReportError("Terminal: LPAREN_T token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	return errors;
}

int SyntacticalAnalyzer::moreDefines()
{
	int errors = 0;
	if (token == IDENT_T)
	{
		token = lex->GetToken();
		errors += stmtList();
		if (token == RPAREN_T)
		{
			token = lex->GetToken();
		}
		else
		{
			lex->ReportError("Terminal: RPAREN_T token expected; '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
	}
	else if (token == DEFINE_T)
	{
		errors += define();
		if (token == LPAREN_T)
		{
			token = lex->GetToken();
			errors += moreDefines();
		}
	}
	else
	{
		lex->ReportError("Terminal: DEFINE_T token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
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
	

	if (token == LPAREN_T)
	{
		token = lex->GetToken();
		errors += moreTokens();
		if (token == RPAREN_T)
		{
			token = lex->GetToken();
		}
		else
		{
			lex->ReportError("Terminal: ElsePart token expected '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
	}

	else if (token == SQUOTE_T)
	{
		token = lex->GetToken();
		errors += anyOtherToken();
	}

	else if (token != IDENT_T && token != NUMLIT_T && token != STRLIT_T && token != CONS_T && token != IF_T && token != DISPLAY_T && token != NEWLINE_T && token != LISTOP_T && token != AND_T && token != OR_T && token != NOT_T && token != DEFINE_T && token != NUMBERP_T && token != LISTP_T && token != ZEROP_T && token != NULLP_T && token != STRINGP_T && token != PLUS_T && token != MINUS_T && token != DIV_T && token != MULT_T && token != MODULO_T && token != ROUND_T && token != EQUALTO_T && token != GT_T && token != LT_T && token != GTE_T && token != LTE_T && token != COND_T && token != ELSE_T)
	{
		lex->ReportError("any_other_token expetokened; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	else
	{
		token = lex->GetToken();
	}
	return errors;
}

int SyntacticalAnalyzer::action()
{ //questions
	int errors = 0;

	if (token == IF_T)
	{
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
		errors += elsePart();
	}

	else if (token == COND_T)
	{
		token = lex->GetToken();
		if (token == LPAREN_T)
		{
			token = lex->GetToken();
			errors += stmtPairBody();
		}
		else
		{
			lex->ReportError("Terminal: (LPAREN_T) expected; '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
	}
	//<stmt>
	else if (token == LISTOP_T || token == NOT_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T || token == NULLP_T || token == STRINGP_T || token == ROUND_T || token == DISPLAY_T)
	{
		token = lex->GetToken();
		errors += stmt();
	}
	//<stmt> <stmt>
	else if (token == CONS_T || token == MODULO_T)
	{
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
	}
	//<stmt_list>
	else if (token == AND_T || token == OR_T || token == PLUS_T || token == MULT_T || token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T || token == LTE_T || token == IDENT_T)
	{
		token = lex->GetToken();
		errors += stmtList();
	}

	//<stmt> <stmt_list>
	else if (token == MINUS_T || token == DIV_T)
	{
		token = lex->GetToken();
		errors += stmt();
		errors += stmtList();
	}

	//NEWLINE_T
	else if (token == NEWLINE_T)
	{
		token = lex->GetToken();
	}

	else
	{
		lex->ReportError("Terminal: action expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
}

int SyntacticalAnalyzer::stmtPair()
{
	int errors = 0;

	if (token == LPAREN_T)
	{
		token = lex->GetToken();
		errors += stmtPairBody();
	}
	else if (token == RPAREN_T)
	{
	}
	else
	{
		lex->ReportError("Terminal: stmt_pair expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
}

int SyntacticalAnalyzer::stmtPairBody()
{
	int errors = 0;
	token = lex->GetToken();

	if (token == ELSE_T)
	{
		token = lex->GetToken();
		errors += stmt();

		if (token == RPAREN_T)
		{
			token = lex->GetToken();
			errors += stmtPair();
		}
		else
		{
			lex->ReportError("Terminal: RPAREN_T expetokened; '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
	}
	else if (token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		errors += stmt();
		errors += stmt();

		if (token == RPAREN_T)
		{
			token = lex->GetToken();
			errors += stmtPair();
		}
		else
		{
			lex->ReportError("Terminal: RPAREN_T expetokened; '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
		errors += stmtPair();
	}
}
int SyntacticalAnalyzer::define()
{
	int errors = 0;
	if (token == DEFINE_T)
	{
		token = lex->GetToken();
		if (token == LPAREN_T)
		{
			token = lex->GetToken();
			if (token == IDENT_T)
			{
				token = lex->GetToken();
				errors += paramList();
				if (token == RPAREN_T)
				{
					token = lex->GetToken();
					errors += stmt();
					errors += stmtList();
					if (token == RPAREN_T)
					{
						token = lex->GetToken();
					}
					else
					{
						lex->ReportError("Terminal: RPAREN_T token expected; '" + lex->GetTokenName(token) + "' found.");
						errors++;
					}
				}
				else
				{
					lex->ReportError("Terminal: RPAREN_T token expected; '" + lex->GetTokenName(token) + "' found.");
					errors++;
				}
			}
			else
			{
				lex->ReportError("Terminal: IDENT_T token expected; '" + lex->GetTokenName(token) + "' found.");
				errors++;
			}
		}
		else
		{
			lex->ReportError("Terminal: LPAREN_T token expected; '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
	}
	else
	{
		lex->ReportError("Terminal: DEFINE_T token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	return errors;
}

int SyntacticalAnalyzer::stmtList()
{
	int errors = 0;
	if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		errors += stmt();
		errors += stmtList();
	}
	else if (token == RPAREN_T)
	{
		//lambda
	}
	else
	{
		lex->ReportError("Terminal: PAREN_T or IDENT_T token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	return errors;
}

int SyntacticalAnalyzer::stmt()
{
	int errors = 0;
	if (token == IDENT_T)
	{
		token = lex->GetToken();
	}
	else if (token == LPAREN_T)
	{
		token = lex->GetToken();
		errors += action();
		if (token == RPAREN_T)
		{
			token = lex->GetToken();
		}
		else
		{
			lex->ReportError("Terminal: LPAREN_T token expected; '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
	}
	else if (token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		errors += literal();
	}
	else
	{
		lex->ReportError("Terminal: IDENT_T, LPAREN, or Literal token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	return errors;
}
