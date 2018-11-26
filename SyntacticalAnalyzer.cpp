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
	for (int i = 0; i < 82; i++)
	{
		rules[i] = 0;
	}
	lex = new LexicalAnalyzer(filename);
	token = lex->GetToken();
	int errors = 0;
	string name = filename;
	string p2name = name.substr(0, name.length() - 3) + ".p2";
	p2file.open(p2name.c_str());
	errors += program();
	/*
	p2file << "Rules used: ";
	for (int i = 0; i<82; i++)
	{
		if (rules[i] == 1 && i < 81)
		{
			p2file<<i<<",";
		}
		else if(rules[i]==1)
		{
			p2file<<i<<"\n";
		}
	}*/
}

SyntacticalAnalyzer::~SyntacticalAnalyzer()
{
	delete lex;
}
// Author of Function Below: Boaz Cogan
int SyntacticalAnalyzer::program() // Rule 1
{
	p2file << "Entering Program function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	p2file << "Using Rule 1\n";
	rules[1] = 1;
	int errors = 0;
	if (token == LPAREN_T) // Check if we are looking at the correct starting token
	{
		token = lex->GetToken(); // Walk over LPAREN_T
		errors += define();		 // Apply rule 4 and collect errors
		if (token == LPAREN_T)
		{
			token = lex->GetToken();
			errors += moreDefines(); // Apply rule 2 or 3, moreDefines() will determine the correct rule used
			if (token == EOF_T)
			{
				p2file << "Exiting Program function; current token is: " << lex->GetTokenName(token) << std::endl;
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
	p2file << "Exiting Program function; current token is: " << lex->GetTokenName(token) << std::endl;
	return errors;
}
// Author of Function Below: Boaz Cogan
int SyntacticalAnalyzer::moreDefines() // Rule 2-3
{
	p2file << "Entering More_Defines function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;
	if (token == IDENT_T) // Check if rule 3 is used as IDENT_T will be the first expected token in rule 3
	{
		p2file << "Using Rule 3\n";
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
		p2file << "Using Rule 2\n";
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
	p2file << "Exiting More_Defines function; current token is: " << lex->GetTokenName(token) << std::endl;

	return errors;
}
// Author of Function Below: Boaz Cogan
int SyntacticalAnalyzer::define() // Rule 4
{
	p2file << "Entering Define function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	p2file << "Using Rule 4\n";
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
	p2file << "Exiting Define function; current token is: " << lex->GetTokenName(token) << std::endl;

	return errors;
}
// Author of Function Below: Boaz Cogan
int SyntacticalAnalyzer::stmtList() // Rule 5-6
{
	p2file << "Entering Stmt_List function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;
	if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T) // Check firsts of the current rule we are lloking at to determine the correct rule to apply
	{
		p2file << "Using Rule 5\n";
		rules[5] = 1;
		errors += stmt();
		errors += stmtList();
	}
	else if (token == RPAREN_T)
	{
		p2file << "Using Rule 6\n";
		rules[6] = 1;
		//lambda
	}
	else
	{
		lex->ReportError("Terminal: PAREN_T or IDENT_T token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	p2file << "Exiting Stmt_List function; current token is: " << lex->GetTokenName(token) << std::endl;

	return errors;
}
// Author of Function Below: Boaz Cogan
int SyntacticalAnalyzer::stmt() // Rule 7-9
{
	p2file << "Entering Stmt function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;
	if (token == IDENT_T)
	{
		p2file << "Using Rule 8\n";
		rules[8] = 1;
		token = lex->GetToken();
	}
	else if (token == LPAREN_T)
	{
		p2file << "Using Rule 9\n";
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
		p2file << "Using Rule 7\n";
		rules[7] = 1;
		errors += literal();
	}
	else
	{
		lex->ReportError("Terminal: IDENT_T, LPAREN, or Literal token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	p2file << "Exiting Stmt function; current token is: " << lex->GetTokenName(token) << std::endl;

	return errors;
}
// Author of Function Below: Jeff Hultman
int SyntacticalAnalyzer::literal() // Rule 10-12
{
	p2file << "Entering Literal function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;
	if (token == NUMLIT_T || token == STRLIT_T)
	{
		if (token == NUMLIT_T)
		{
			p2file << "Using Rule 10\n";
			rules[10] = 1;
		}
		else
		{
			p2file << "Using Rule 11\n";
			rules[11] = 1;
		}
		// Apply rule 10 or 11
		token = lex->GetToken();
	}
	else if (token == SQUOTE_T)
	{
		p2file << "Using Rule 12\n";
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
	p2file << "Exiting Literal function; current token is: " << lex->GetTokenName(token) << std::endl;
	return errors;
}
// Author of Function Below: Jeff Hultman
int SyntacticalAnalyzer::quotedLit() // Rule 13
{
	p2file << "Entering Quoted_Lit function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;
	if (token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == CONS_T || token == IF_T || token == DISPLAY_T || token == NEWLINE_T ||
		token == LISTOP_T || token == AND_T || token == OR_T || token == NOT_T || token == DEFINE_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T ||
		token == NULLP_T || token == STRINGP_T || token == PLUS_T || token == MINUS_T || token == DIV_T || token == MULT_T || token == MODULO_T || token == ROUND_T ||
		token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T || token == LTE_T || token == COND_T || token == ELSE_T)
	{
		p2file << "Using Rule 13\n";
		rules[13] = 1;
		// Rule 13 Used
		errors += anyOtherToken();
	}
	else
	{
		errors++;
		lex->ReportError("Terminal: QuotedLit token expected; '" + lex->GetTokenName(token) + "' found.");
	}
	p2file << "Exiting Quoted_Lit function; current token is: " << lex->GetTokenName(token) << std::endl;
	return errors;
}
// Author of Function Below: Jeff Hultman
int SyntacticalAnalyzer::moreTokens() // Rule 14-15
{
	p2file << "Entering More_tokens function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;
	if (token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == CONS_T || token == IF_T || token == DISPLAY_T || token == NEWLINE_T ||
		token == LISTOP_T || token == AND_T || token == OR_T || token == NOT_T || token == DEFINE_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T ||
		token == NULLP_T || token == STRINGP_T || token == PLUS_T || token == MINUS_T || token == DIV_T || token == MULT_T || token == MODULO_T || token == ROUND_T ||
		token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T || token == LTE_T || token == COND_T || token == ELSE_T)
	{
		p2file << "Using Rule 14\n";
		rules[14] = 1;
		// Rule 14 Used
		errors += anyOtherToken();
		errors += moreTokens();
	}
	else if (token == RPAREN_T)
	{
		p2file << "Using Rule 15\n";
		rules[15] = 1;
		// Rule 15 used
		// Lambda
	}
	else
	{
		errors++;
		lex->ReportError("Terminal: AnyOtherToken token expected; '" + lex->GetTokenName(token) + "' found.");
	}
	p2file << "Exiting More_Tokens function; current token is: " << lex->GetTokenName(token) << std::endl;

	return errors;
}
// Author of Function Below: Jeff Hultman
int SyntacticalAnalyzer::paramList() // Rule 16-17
{
	p2file << "Entering Param_List function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;
	if (token == IDENT_T)
	{
		p2file << "Using Rule 16\n";
		rules[16] = 1;
		token = lex->GetToken();
		errors += paramList();
	}
	else if (token == RPAREN_T)
	{
		p2file << "Using Rule 17\n";
		rules[17] = 1;
	}
	else
	{
		errors++;
		lex->ReportError("Terminal: ParamList token expected; '" + lex->GetTokenName(token) + "' found.");
	}
	p2file << "Exiting Param_List function; current token is: " << lex->GetTokenName(token) << std::endl;
	return errors;
}
// Author of Function Below: Jeff Hultman
int SyntacticalAnalyzer::elsePart() // Rule 18-19
{
	p2file << "Entering else_part function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;
	if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		p2file << "Using Rule 18\n";
		rules[18] = 1;
		errors += stmt();
	}
	else if (token == RPAREN_T)
	{
		p2file << "Using Rule 19\n";
		rules[19] = 1;
	}
	else
	{
		errors++;
		lex->ReportError("Terminal: ElsePart token expected; '" + lex->GetTokenName(token) + "' found.");
	}
	p2file << "Exiting Else_Part function; current token is: " << lex->GetTokenName(token) << std::endl;
	return errors;
}
// Author of Function Below: Sergio Hernandez
int SyntacticalAnalyzer::stmtPair() // Rule 20-21
{
	p2file << "Entering Stmt_Pair function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;

	if (token == LPAREN_T)
	{
		p2file << "Using Rule 20\n";
		rules[20] = 1;
		token = lex->GetToken();
		errors += stmtPairBody();
	}
	else if (token == RPAREN_T)
	{
		p2file << "Using Rule 21\n";
		rules[21] = 1;
	}
	else
	{
		lex->ReportError("Terminal: stmt_pair expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	p2file << "Exiting Stmt_Pair function; current token is: " << lex->GetTokenName(token) << std::endl;
return errors;
}
// Author of Function Below: Sergio Hernandez
int SyntacticalAnalyzer::stmtPairBody() // Rule 22-23
{
	p2file << "Entering Stmt_Pair_Body function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;
	// token = lex->GetToken();

	if (token == ELSE_T)
	{
		p2file << "Using Rule 23\n";
		rules[23] = 1;
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
	else if (token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T || token == LPAREN_T || token == IDENT_T)
	{
		p2file << "Using Rule 22\n";
		rules[22] = 1;
		// token = lex->GetToken();
		errors += stmt();
		errors += stmt();

		if (token == RPAREN_T)
		{
			token = lex->GetToken();
			errors += stmtPair();
		}
		else
		{
			lex->ReportError("Terminal: RPAREN_T token expected; '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
		errors += stmtPair();
	}
	p2file << "Exiting Stmt_Pair_Body function; current token is: " << lex->GetTokenName(token) << std::endl;
return errors;
}
// Author of Function Below: Sergio Hernandez
int SyntacticalAnalyzer::action() // Rule 24-49
{								  //questions
	p2file << "Entering Action function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;

	if (token == IF_T)
	{
		p2file << "Using Rule 24\n";
		rules[24] = 1;
		token = lex->GetToken();
		errors += stmt();
		errors += stmt();
		errors += elsePart();
	}

	else if (token == COND_T)
	{
		p2file << "Using Rule 25\n";
		rules[25] = 1;
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
			p2file << "Using Rule 26\n";
			rules[26] = 1;
		}
		else if (token == NOT_T)
		{
			p2file << "Using Rule 30\n";
			rules[30] = 1;
		}
		else if (token == NUMBERP_T)
		{
			p2file << "Using Rule 31\n";
			rules[31] = 1;
		}
		else if (token == LISTP_T)
		{
			p2file << "Using Rule 32\n";
			rules[32] = 1;
		}
		else if (token == ZEROP_T)
		{
			p2file << "Using Rule 33\n";
			rules[33] = 1;
		}
		else if (token == NULLP_T)
		{
			p2file << "Using Rule 34\n";
			rules[34] = 1;
		}
		else if (token == STRINGP_T)
		{
			p2file << "Using Rule 35\n";
			rules[35] = 1;
		}
		else if (token == ROUND_T)
		{
			p2file << "Using Rule 36\n";
			rules[36] = 1;
		}
		else
		{
			p2file << "Using Rule 48\n";
			rules[48] = 1;
		}
		token = lex->GetToken();
		errors += stmt();
	}
	//<stmt> <stmt>
	else if (token == CONS_T || token == MODULO_T)
	{
		if (token == CONS_T)
		{
			p2file << "Using Rule 27\n";
			rules[27] = 1;
		}
		else
		{
			p2file << "Using Rule 40\n";
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
			p2file << "Using Rule 28\n";
			rules[28] = 1;
		}
		else if (token == OR_T)
		{
			p2file << "Using Rule 29\n";
			rules[29] = 1;
		}
		else if (token == PLUS_T)
		{
			p2file << "Using Rule 36\n";
			rules[36] = 1;
		}
		else if (token == MULT_T)
		{
			p2file << "Using Rule 39\n";
			rules[39] = 1;
		}
		else if (token == EQUALTO_T)
		{
			p2file << "Using Rule 42\n";
			rules[42] = 1;
		}
		else if (token == GT_T)
		{
			p2file << "Using Rule 43\n";
			rules[43] = 1;
		}
		else if (token == LT_T)
		{
			p2file << "Using Rule 44\n";
			rules[44] = 1;
		}
		else if (token == GTE_T)
		{
			p2file << "Using Rule 45\n";
			rules[45] = 1;
		}
		else if (token == LTE_T)
		{
			p2file << "Using Rule 46\n";
			rules[46] = 1;
		}
		else if (token == IDENT_T)
		{
			p2file << "Using Rule 47\n";
			rules[47] = 1;
		}

		token = lex->GetToken();
		errors += stmtList();
	}

	//<stmt> <stmt_list>
	else if (token == MINUS_T || token == DIV_T)
	{
		if (token == MINUS_T)
		{
			p2file << "Using Rule 37\n";
			rules[37] = 1;
		}
		else
		{
			p2file << "Using Rule 38\n";
			rules[38] = 1;
		}
		token = lex->GetToken();
		errors += stmt();
		errors += stmtList();
	}

	//NEWLINE_T
	else if (token == NEWLINE_T)
	{
		p2file << "Using Rule 49\n";
		rules[49] = 1;
		token = lex->GetToken();
	}

	else
	{
		lex->ReportError("Terminal: action expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	p2file << "Exiting Action function; current token is: " << lex->GetTokenName(token) << std::endl;
return errors;
}
// Author of Function Below: Sergio Hernandez
int SyntacticalAnalyzer::anyOtherToken() // Rule 50-81
{
	p2file << "Entering Any_Other_Token function; current token is: " << lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << "\n";
	int errors = 0;
	if (token == LPAREN_T)
	{
		p2file << "Using Rule 50\n";
		rules[50] = 1;
		token = lex->GetToken();
		errors += moreTokens();
		if (token == RPAREN_T)
		{
			token = lex->GetToken();
		}
		else
		{
			lex->ReportError("Terminal: AnyOtherToken token expected; '" + lex->GetTokenName(token) + "' found.");
			errors++;
		}
	}

	else if (token == SQUOTE_T)
	{
		p2file << "Using Rule 79\n";
		rules[79] = 1;
		token = lex->GetToken();
		errors += anyOtherToken();
	}

	else if (token != IDENT_T && token != NUMLIT_T && token != STRLIT_T && token != CONS_T && token != IF_T && token != DISPLAY_T && token != NEWLINE_T &&
			 token != LISTOP_T && token != AND_T && token != OR_T && token != NOT_T && token != DEFINE_T && token != NUMBERP_T && token != LISTP_T && token != ZEROP_T &&
			 token != NULLP_T && token != STRINGP_T && token != PLUS_T && token != MINUS_T && token != DIV_T && token != MULT_T && token != MODULO_T && token != ROUND_T &&
			 token != EQUALTO_T && token != GT_T && token != LT_T && token != GTE_T && token != LTE_T && token != COND_T && token != ELSE_T)
	{
		lex->ReportError("anyOtherToken token expected; '" + lex->GetTokenName(token) + "' found.");
		errors++;
	}
	else
	{
		if (token == IDENT_T)
		{
			p2file << "Using Rule 51\n";
			rules[51] = 1;
		}
		else if (token == NUMLIT_T)
		{
			p2file << "Using Rule 52\n";
			rules[52] = 1;
		}
		else if (token == STRLIT_T)
		{
			p2file << "Using Rule 53\n";
			rules[53] = 1;
		}
		else if (token == CONS_T)
		{
			p2file << "Using Rule 54\n";
			rules[54] = 1;
		}
		else if (token == IF_T)
		{
			p2file << "Using Rule 55\n";
			rules[55] = 1;
		}
		else if (token == DISPLAY_T)
		{
			p2file << "Using Rule 56\n";
			rules[56] = 1;
		}
		else if (token == NEWLINE_T)
		{
			p2file << "Using Rule 57\n";
			rules[57] = 1;
		}
		else if (token == LISTOP_T)
		{
			p2file << "Using Rule 58\n";
			rules[58] = 1;
		}
		else if (token == AND_T)
		{
			p2file << "Using Rule 59\n";
			rules[59] = 1;
		}
		else if (token == OR_T)
		{
			p2file << "Using Rule 60\n";
			rules[60] = 1;
		}
		else if (token == NOT_T)
		{
			p2file << "Using Rule 61\n";
			rules[61] = 1;
		}
		else if (token == DEFINE_T)
		{
			p2file << "Using Rule 62\n";
			rules[62] = 1;
		}
		else if (token == NUMBERP_T)
		{
			p2file << "Using Rule 63\n";
			rules[63] = 1;
		}
		else if (token == LISTP_T)
		{
			p2file << "Using Rule 64\n";
			rules[64] = 1;
		}
		else if (token == ZEROP_T)
		{
			p2file << "Using Rule 65\n";
			rules[65] = 1;
		}
		else if (token == NULLP_T)
		{
			p2file << "Using Rule 66\n";
			rules[66] = 1;
		}
		else if (token == STRINGP_T)
		{
			p2file << "Using Rule 67\n";
			rules[67] = 1;
		}
		else if (token == PLUS_T)
		{
			p2file << "Using Rule 68\n";
			rules[68] = 1;
		}
		else if (token == MINUS_T)
		{
			p2file << "Using Rule 69\n";
			rules[69] = 1;
		}
		else if (token == DIV_T)
		{
			p2file << "Using Rule 70\n";
			rules[70] = 1;
		}
		else if (token == MULT_T)
		{
			p2file << "Using Rule 71\n";
			rules[71] = 1;
		}
		else if (token == MODULO_T)
		{
			p2file << "Using Rule 72\n";
			rules[72] = 1;
		}
		else if (token == ROUND_T)
		{
			p2file << "Using Rule 73\n";
			rules[73] = 1;
		}
		else if (token == EQUALTO_T)
		{
			p2file << "Using Rule 74\n";
			rules[74] = 1;
		}
		else if (token == GT_T)
		{
			p2file << "Using Rule 75\n";
			rules[75] = 1;
		}
		else if (token == LT_T)
		{
			p2file << "Using Rule 76\n";
			rules[76] = 1;
		}
		else if (token == GTE_T)
		{
			p2file << "Using Rule 77\n";
			rules[77] = 1;
		}
		else if (token == LTE_T)
		{
			p2file << "Using Rule 78\n";
			rules[78] = 1;
		}
		else if (token == COND_T)
		{
			p2file << "Using Rule 80\n";
			rules[80] = 1;
		}
		else if (token == ELSE_T)
		{
			p2file << "Using Rule 81\n";
			rules[81] = 1;
		}
		token = lex->GetToken();
	}
	p2file << "Exiting Any_Other_Token function; current token is: " << lex->GetTokenName(token) << std::endl;

	return errors;
}
