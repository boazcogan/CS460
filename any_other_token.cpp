/*******************************************************************************    
* Assignment: P2 Language Syntatokenical Analyzer                                 *  
* Author: Sergio Hernandez Mata                                                *  
* Date: Fall 2019                                                              *                    
* Description: This file contains the Syntatokenial Analyzer funtokenion for the     *  
               non-terminal symbol "novel".                                    *
*******************************************************************************/

#include "Syn.h"

int Syn::any_other_token ()
{
        int errors = 0;

	if(token == "LPAREN_T")
	  {
	    token = lex->NextTerminal();
	    errors += more_tokens();
	    if(token == "RPAREN_T")
	      {
		token = lex->NextTerminal();
	      }
	    else
	      {
		lex->ReportError("Terminal: RPAREN_T expetokened; '" + token + "' found.");
		errors++;
	      }
	  }

	else if(token == "SQUOTE_t")
          {
            token = lex->NextTerminal();
            errors += any_other_token();
	    
          }
	
	else if (token != "IDENT_T" && token != "NUMLIT_T" && token != "STRLIT_T" && token != "CONS_T" && token != "IF_T" && token != "DISPLAY_T" && token != "NEWLINE_T"
		 && token != "LISTOP_T" && token != "AND_T" && token != "OR_T" && token != "NOT_T" && token != "DEFINE_T" && token != "NUMBERP_T" && token != "LISTP_T"
		 && token != "ZEROP_T" && token != "NULLP_T" && token != "STRINGP_T" && token != "PLUS_T" && token != "MINUS_T" && token != "DIV_T" && token != "MULT_T"
		 && token != "MODULO_T" && token != "ROUND_T" && token != "EQUALTO_T" && token != "GT_T" && token != "LT_T" && token != "GTE_T" && token != "LTE_T"
		 && token != "COND_T"  && token != "ELSE_T" )
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
