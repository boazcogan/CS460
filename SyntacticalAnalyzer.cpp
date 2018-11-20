#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	token = lex->GetToken();
	int errors = 0;
	errors += program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}

int SyntacticalAnalyzer::program()
{
	int errors = 0;
	if ( token == LPAREN_T )
	{
		token = lex->GetToken();
		errors += define();
		if ( token == LPAREN_T )
		{
			token = lex->GetToken();
			errors += moreDefines();
			if ( token == EOF_T )
			{
				return errors;
			}
			else
			{
				errors++;
			}
		}
		else
		{
			errors++;
		}
	}
	else
	{
		errors++;
	}
	return errors;
}

int SyntacticalAnalyzer::moreDefines()
{
	int errors = 0;
	if ( token == IDENT_T )
	{
		token = lex->GetToken();
		errors += stmtList();
		if ( token == RPAREN_T )
		{
			token = lex->GetToken();
		}
		else
		{
			errors++;
		}
	}
	else if ( token == DEFINE_T )
	{
		errors += define();
		if ( token == LPAREN_T )
		{
			token = lex->GetToken();
			errors += moreDefines();
		}
	}
	else
	{
		errors++;
	}
	return errors;
}

int SyntacticalAnalyzer::define()
{
	int errors = 0;
	if ( token == DEFINE_T )
	{
		token = lex->GetToken();
		if ( token == LPAREN_T )
		{
			token = lex->GetToken();
			if ( token == IDENT_T )
			{
				token = lex->GetToken();
				errors += paramList();
				if ( token == RPAREN_T )
				{
					token = lex->GetToken();
					errors += stmt();
					errors += stmtList();
					if ( token == RPAREN_T )
					{
						token = lex->GetToken();
					}
					else
					{
						errors++;
					}
				}
				else
				{
					errors++;
				}
				
			}
			else
			{
				errors++;
			}
			
		}
		else
		{
			errors++;
		}

	}
	else
	{
		errors++;
	}
	return errors;

}

int SyntacticalAnalyzer::stmtList()
{
	int errors = 0;
	if ( token == IDENT_T || token == LPAREN_T )
	{
		errors += stmt();
		error += stmtList();
	}
	else if ( token == RPAREN_T )
	{
		//lambda
	}
	else
	{
		errors++;
	}
	return errors;

}

int SyntacticalAnalyzer::stmt()
{
	int errors = 0;
	if ( token == IDENT_T )
	{
		token = lex->GetToken();
	}
	else if ( token == LPAREN_T )
	{
		token = lex->GetToken();
		errors+=action();
		if ( token == RPAREN_T )
		{
			token = lex->GetToken();
		}
		else
		{
			errors++;
		}
	}
	else
	{
		errors+=literal();
	}
	return errors;
}

