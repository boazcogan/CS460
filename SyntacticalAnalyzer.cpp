#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

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

int SyntacticalAnalyzer::any_other_token ()
{
        int errors = 0;

	if(token == LPAREN_T)
	  {
	    token = lex->NextTerminal();
	    errors += more_tokens();
	    if(token == RPAREN_T)
	      {
		token = lex->NextTerminal();
	      }
	    else
	      {
		lex->ReportError("Terminal: RPAREN_T expetokened; '" + token + "' found.");
		errors++;
	      }
	  }

	else if(token == SQUOTE_t)
          {
            token = lex->NextTerminal();
            errors += any_other_token();
	    
          }
	
	else if (token != IDENT_T && token != NUMLIT_T && token != STRLIT_T && token != CONS_T && token != IF_T && token != DISPLAY_T && token != NEWLINE_T
		 && token != LISTOP_T && token != AND_T && token != OR_T && token != NOT_T && token != DEFINE_T && token != NUMBERP_T && token != LISTP_T
		 && token != ZEROP_T && token != NULLP_T && token != STRINGP_T && token != PLUS_T && token != MINUS_T && token != DIV_T && token != MULT_T
		 && token != MODULO_T && token != ROUND_T && token != EQUALTO_T && token != GT_T && token != LT_T && token != GTE_T && token != LTE_T
		 && token != COND_T  && token != ELSE_T )
	  {
	    lex->ReportError ("any_other_token expetokened; '" + token + "' found.");
	    errors++;
	  }
        else
	  {
	    token = lex->NextTerminal();
	  }
        return errors;
}

int SyntacticalAnalyzer::action ()
{//questions
  int errors = 0;

  if(token == IF_T)
    {
      token = lex->NextTerminal();
      errors += stmt();
      errors += stmt();
      errors += else_part();
    }

  else if(token == COND_T)
    {
      token = lex->NextTerminal();
      if(token == LPAREN_T)
	{
	  token = lex->NextTerminal();
	  errors += stmt_pair_body();
	}
      else
	{
	  lex->ReportError("Terminal: (LPAREN_T) expected; '" + ct + "' found.");
	  errors++;
	}
    }
  //<stmt>
  else if(token == LISTOP_T || token == NOT_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T || token == NULLP_T || token == STRINGP_T || token == ROUND_T || token == DISPLAY_T )
    {
      token = lex->NextTerminal();
      errors += stmt();
    }
  //<stmt> <stmt>
  else if(token == CONS_T || token == MODULO_T)
    {
      token = lex->NextTerminal();
      errors += stmt();
      errors += stmt();
    }
  //<stmt_list>
  else if(token == AND_T || token == OR_T || token == PLUS_T || token == MULT_T || token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T || token == LTE_T ||  token == IDENT_T )
    {
      token = lex->NextTerminal();
      errors += stmt_list();
    }

  //<stmt> <stmt_list>
  else if(token == MINUS_T || token == DIV_T)
    {
      token = lex->NextTerminal();
      errors += stmt();
      errors += stmt_list();
    }

  //NEWLINE_T
  else if(token == NEWLINE_T)
    {
      token = lex->NextTerminal();
    }
  
  else
    {
      lex->ReportError("Terminal: action expected; '" + ct + "' found.");
      errors++;
    }
}


int SyntacticalAnalyzer::stmt_pair()
{
  int errors = 0;	 
  
  if(token == LPAREN_T)
    {
      token = lex->NextTerminal();
      errors += stmt_pair_body();
    }
  else if(token == RPAREN_T)
    {
      
    }
  else
    {
      lex->ReportError("Terminal: stmt_pair expected; '" + ct + "' found.");
      errors++;
    }

}

int SyntacticalAnalyzer::stmt_pair_body()
{
    int errors = 0;
    token = lex->NextTerminal();

    if(token == ELSE_T)
      {
	token = lex->NextTerminal();
	errors += stmt();

	if(token == RPAREN_T)
	  {
	    token = lex->NextTerminal();
	    errors += stmt_pair();
	  }
	else
	  {
	    lex->ReportError("Terminal: RPAREN_T expetokened; '" + token + "' found.");
	    errors++;
	  }
      }
    else if(token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
      {
	errors += stmt();
	errors += stmt();
	
	if(token == RPAREN_T)
	  {
	    token = lex->NextTerminal();
	    errors += stmt_pair();
	  }
	else
	  {
	    lex->ReportError("Terminal: RPAREN_T expetokened; '" + token + "' found.");
	    errors++;
	  }
	errors += stmt_pair();
      }

}
