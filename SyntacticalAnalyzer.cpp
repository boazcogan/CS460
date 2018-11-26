#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"
#include "LexicalAnalyzer.h"

using namespace std;

// Authors: Jeff Hultman, Boaz Cogan, Sergio Hernandez
// Description: Syntatical Analyzer for a Scheme->c++ compiler
// Note: For documentation, only first examples or simialr cases are documented.



SyntacticalAnalyzer::SyntacticalAnalyzer(char *filename)
{

	rules = new int[82];
	for (int i = 0; i < 81; i++)
	{
		rules[i] = 0;
	}
	lex = new LexicalAnalyzer(filename);
	token = lex->GetToken();
	int errors = 0;
	errors += program();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer()
{
	delete lex;
}
// Author of Function Below: Boaz Cogan
int SyntacticalAnalyzer::program() // Rule 1
{
	rules[1] = 1;
	int errors = 0;
	if (token == LPAREN_T) // Check if we are looking at the correct starting token
	{
		token = lex->GetToken(); // Walk over LPAREN_T
		errors += define(); // Apply rule 4 and collect errors
		if (token == LPAREN_T)
		{
			token = lex->GetToken();
			errors += moreDefines(); // Apply rule 2 or 3, moreDefines() will determine the correct rule used
			if (token == EOF_T)
			{
				return errors;
			}
			else
			{
				lex->ReportError("Terminal: EOF_T token expected; '" + lex->GetTokenName(token) + "' found."); // Report error to our LexicalAnalyzer
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
// Author of Function Below: Boaz Cogan
int SyntacticalAnalyzer::moreDefines() // Rule 2-3
{
	int errors = 0;
	if (token == IDENT_T) // Check if rule 3 is used as IDENT_T will be the first expected token in rule 3
	{
		rules[3] = 1;
		token = lex->GetToken();
		errors += stmtList(); // Apply rule 5 or 6
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
	else if (token == DEFINE_T) // If rule 3 is not correct check if rule 2 is
	{
		rules[2] = 1;
		errors += define(); // Apply rule 4
		if (token == LPAREN_T)
		{
			token = lex->GetToken();
			errors += moreDefines(); // Apply ru;e 2 or 3
		}
	}
	else // Neither rule 2 or 3 are correct, report error
	{
		lex->ReportError("Terminal: DEFINE_T token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	return errors;
}
// Author of Function Below: Boaz Cogan
int SyntacticalAnalyzer::define() // Rule 4
{
	rules[4] = 1;
	int errors = 0;
	if (token == DEFINE_T) // Check for correct Token for the start of rule 4
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
// Author of Function Below: Boaz Cogan
int SyntacticalAnalyzer::stmtList() // Rule 5-6
{
	int errors = 0;
	if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T) // Check firsts of the current rule we are lloking at to determine the correct rule to apply 
	{
		rules[5] = 1;
		errors += stmt();
		errors += stmtList();
	}
	else if (token == RPAREN_T)
	{
		rules[6] = 1;
		//lambda
	}
	else
	{
		lex->ReportError("Terminal: PAREN_T or IDENT_T token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	return errors;
}
// Author of Function Below: Boaz Cogan
int SyntacticalAnalyzer::stmt() // Rule 7-9
{
	int errors = 0;
	if (token == IDENT_T)
	{
		rules[8] = 1;
		token = lex->GetToken();
	}
	else if (token == LPAREN_T)
	{
		rules[9] = 1;
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
		rules[7] = 1;
		errors += literal();
	}
	else
	{
		lex->ReportError("Terminal: IDENT_T, LPAREN, or Literal token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	return errors;
}
// Author of Function Below: Jeff Hultman
int SyntacticalAnalyzer::literal() // Rule 10-12
{
	int errors = 0;
	if (token == NUMLIT_T || token == STRLIT_T)
	{
		if (token == NUMLIT_T)
		{
			rules[10] = 1;
		}
		else
		{
			rules[11] = 1;
		}
		// Apply rule 10 or 11
		token = lex->GetToken();
	}
	else if (token == SQUOTE_T)
	{
		rules[12] = 1;
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
// Author of Function Below: Jeff Hultman
int SyntacticalAnalyzer::quotedLit() // Rule 13
{
	int errors = 0;
	if (token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == CONS_T || token == IF_T || token == DISPLAY_T || token == NEWLINE_T ||
		token == LISTOP_T || token == AND_T || token == OR_T || token == NOT_T || token == DEFINE_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T ||
		token == NULLP_T || token == STRINGP_T || token == PLUS_T || token == MINUS_T || token == DIV_T || token == MULT_T || token == MODULO_T || token == ROUND_T ||
		token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T || token == LTE_T || token == COND_T || token == ELSE_T)
	{
		rules[13] = 1;
		// Rule 13 Used
		errors += anyOtherToken();
	}
	else
	{
		errors++;
		lex->ReportError("Terminal: QuotedLit token expected; '" + lex->GetTokenName(token) + "' found.");
	}
}
// Author of Function Below: Jeff Hultman
int SyntacticalAnalyzer::moreTokens() // Rule 14-15
{
	int errors = 0;
	if (token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == CONS_T || token == IF_T || token == DISPLAY_T || token == NEWLINE_T ||
		token == LISTOP_T || token == AND_T || token == OR_T || token == NOT_T || token == DEFINE_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T ||
		token == NULLP_T || token == STRINGP_T || token == PLUS_T || token == MINUS_T || token == DIV_T || token == MULT_T || token == MODULO_T || token == ROUND_T ||
		token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T || token == LTE_T || token == COND_T || token == ELSE_T)
	{
		rules[14]=1;
		// Rule 14 Used
		errors += anyOtherToken();
		errors += moreTokens();
	}
	else if (token == RPAREN_T)
	{
		rules[15]=1;
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
// Author of Function Below: Jeff Hultman
int SyntacticalAnalyzer::paramList() // Rule 16-17
{
	int errors = 0;
	if (token == IDENT_T)
	{
		rules[16]=1;
		errors += paramList();
	}
	else if (token == RPAREN_T)
	{
		rules[17]=1;
	}
	else
	{
		errors++;
		lex->ReportError("Terminal: ParamList token expected; '" + lex->GetTokenName(token) + "' found.");
	}
}
// Author of Function Below: Jeff Hultman
int SyntacticalAnalyzer::elsePart() // Rule 18-19
{
	int errors = 0;
	if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		rules[18]=1;
		errors += stmt();
	}
	else if (token == RPAREN_T)
	{
		rules[19]=1;
	}
	else
	{
		errors++;
		lex->ReportError("Terminal: ElsePart token expected; '" + lex->GetTokenName(token) + "' found.");
	}
}
// Author of Function Below: Sergio Hernandez
int SyntacticalAnalyzer::stmtPair() // Rule 20-21
{
	int errors = 0;

	if (token == LPAREN_T)
	{
		rules[20]=1;
		token = lex->GetToken();
		errors += stmtPairBody();
	}
	else if (token == RPAREN_T)
	{
		rules[21]=1;
	}
	else
	{
		lex->ReportError("Terminal: stmt_pair expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
}
// Author of Function Below: Sergio Hernandez
int SyntacticalAnalyzer::stmtPairBody() // Rule 22-23
{
	int errors = 0;
	token = lex->GetToken();

	if (token == ELSE_T)
	{
		rules[23]=1;
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
		rules[22]=1;
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
// Author of Function Below: Sergio Hernandez
int SyntacticalAnalyzer::action() // Rule 24-49
{ //questions
	int errors = 0;

	if (token == IF_T)
	{
		rules[24]=1;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
		errors += elsePart();
	}

	else if (token == COND_T)
	{
		rules[25]=1;
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
	else if (token == LISTOP_T || token == NOT_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T || token == NULLP_T || token == STRINGP_T ||
			 token == ROUND_T || token == DISPLAY_T)
	{
		if (token == LISTOP_T)
		{
			rules[26]=1;
		}
		else if (token == NOT_T)
		{
			rules[30]=1;
		}
		else if (token == NUMBERP_T)
		{
			rules[31] = 1;
		}
		else if (token == LISTP_T)
		{
			rules[32]=1;
		}
		else if (token == ZEROP_T)
		{
			rules[33]=1;
		}
		else if (token == NULLP_T)
		{
			rules[34]=1;
		}
		else if (token == STRINGP_T)
		{
			rules[35]=1;
		}
		else if (token == ROUND_T)
		{
			rules[36]=1;
		}
		else
		{
			rules[48]=1;
		}
		token = lex->GetToken();
		errors += stmt();
	}
	//<stmt> <stmt>
	else if (token == CONS_T || token == MODULO_T)
	{
		if (token == CONS_T)
		{
			rules[27]=1;
		}
		else
		{
			rules[40] = 1;
		}
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
	}
	//<stmt_list>
	else if (token == AND_T || token == OR_T || token == PLUS_T || token == MULT_T || token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T ||
			 token == LTE_T || token == IDENT_T)
	{
		if (token == AND_T)
		{
			rules[28]=1;
		}
		else if (token == OR_T)
		{
			rules[29]=1;
		}
		else if (token == PLUS_T)
		{
			rules[36]=1;
		}
		else if (token == MULT_T)
		{
			rules[39]=1;
		}
		else if (token == EQUALTO_T)
		{
			rules[42]=1;
		}
		else if (token == GT_T)
		{
			rules[43]=1;
		}
		else if (token == LT_T)
		{
			rules[44] = 1;
		}
		else if (token == GTE_T)
		{
			rules[45] = 1;
		}
		else if (token == LTE_T)
		{
			rules[46] = 1;
		}
		else if (token == IDENT_T)
		{
			rules[47]=1;
		}

		token = lex->GetToken();
		errors += stmtList();
	}

	//<stmt> <stmt_list>
	else if (token == MINUS_T || token == DIV_T)
	{
		if (token == MINUS_T)
		{
			rules[37] = 1;
		}
		else
		{
			rules[38] = 1;
		}
		token = lex->GetToken();
		errors += stmt();
		errors += stmtList();
	}

	//NEWLINE_T
	else if (token == NEWLINE_T)
	{
		rules[49]=1;
		token = lex->GetToken();
	}

	else
	{
		lex->ReportError("Terminal: action expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
}
// Author of Function Below: Sergio Hernandez
int SyntacticalAnalyzer::anyOtherToken() // Rule 50-81
{
	int errors = 0;
	if (token == LPAREN_T)
	{
		rules[50]=1;
		token = lex->GetToken();
		errors += moreTokens();
		if (token == RPAREN_T)
		{
			token = lex->GetToken();
		}
		else
		{
			lex->ReportError("Terminal: AnyOtherToken expetokened; '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
	}

	else if (token == SQUOTE_T)
	{
		rules[79]=1;
		token = lex->GetToken();
		errors += anyOtherToken();
	}

	else if (token != IDENT_T && token != NUMLIT_T && token != STRLIT_T && token != CONS_T && token != IF_T && token != DISPLAY_T && token != NEWLINE_T &&
			 token != LISTOP_T && token != AND_T && token != OR_T && token != NOT_T && token != DEFINE_T && token != NUMBERP_T && token != LISTP_T && token != ZEROP_T &&
			 token != NULLP_T && token != STRINGP_T && token != PLUS_T && token != MINUS_T && token != DIV_T && token != MULT_T && token != MODULO_T && token != ROUND_T &&
			 token != EQUALTO_T && token != GT_T && token != LT_T && token != GTE_T && token != LTE_T && token != COND_T && token != ELSE_T)
	{
		lex->ReportError("anyOtherToken expetokened; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	else
	{
		if (token == IDENT_T)
		{
			rules[51] = 1;
		}
		else if (token == NUMLIT_T)
		{
			rules[52]=1;
		}
		else if (token == STRLIT_T)
		{
			rules[53] = 1;
		}
		else if (token == CONS_T)
		{
			rules[54]=1;
		}
		else if (token == IF_T)
		{
			rules[55]=1;
		}
		else if (token == DISPLAY_T)
		{
			rules[56]=1;
		}
		else if (token == NEWLINE_T)
		{
			rules[57]=1;
		}
		else if (token == LISTOP_T)
		{
			rules[58]=1;
		}
		else if (token == AND_T)
		{
			rules[59]=1;
		}
		else if (token == OR_T)
		{
			rules[60]=1;
		}
		else if (token == NOT_T)
		{
			rules[61]=1;
		}
		else if (token == DEFINE_T)
		{
			rules[62]=1;
		}
		else if (token == NUMBERP_T)
		{
			rules[63]=1;
		}
		else if (token == LISTP_T)
		{
			rules[64]=1;
		}
		else if (token == ZEROP_T)
		{
			rules[65]=1;
		}
		else if (token == NULLP_T)
		{
			rules[66] = 1;
		}
		else if (token == STRINGP_T)
		{
			rules[67]=1;
		}
		else if (token == PLUS_T)
		{
			rules[68]=1;
		}
		else if (token == MINUS_T)
		{
			rules[69]=1;
		}
		else if (token == DIV_T)
		{
			rules[70]=1;
		}
		else if (token == MULT_T)
		{
			rules[71]=1;
		}
		else if (token == MODULO_T)
		{
			rules[72]=1;
		}
		else if (token == ROUND_T)
		{
			rules[73]=1;
		}
		else if (token == EQUALTO_T)
		{
			rules[74]=1;
		}
		else if (token == GT_T)
		{
			rules[75]=1;
		}
		else if (token == LT_T)
		{
			rules[76]=1;
		}
		else if (token == GTE_T)
		{
			rules[77]=1;
		}
		else if (token == LTE_T)
		{
			rules[78]=1;
		}
		else if (token == COND_T)
		{
			rules[80]=1;
		}
		else if (token == ELSE_T)
		{
			rules[81]=1;
		}
		token = lex->GetToken();
	}
	return errors;
}
