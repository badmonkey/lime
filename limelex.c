

#include "lime.h"


namespace lime
{
  

/*
tokenizer curlyblock
  ignore "{" then curlyblock
  ignore "[^}]+"
  halt "}"
end
*/
struct curlyblock : public lime::tokenizer
{
	
    curlyblock()
    : tokenizer()
    {
	add_pattern(0, "{");
	add_pattern(1, "[^}]+");
	add_pattern(2, "}");
	add_pattern(3, "\r?\n");
	
	regex_set_.Compile();
	
    }
    
    
    lime::Token next_token(lime::StringPiece& text)
    {
	lime::StringPiece	START = text;
	lime::Token		TOKEN;
	
	
	int idx = match_token(text, TOKEN);
	
	switch( idx )
	{
	    case 0:
	    {
		lime::Token t = lime::process_machine<curlyblock>(*this, text);
		if ( t.id == lime::ERROR )
		    return t;
		
		TOKEN.text = span_pieces( START, t.text );
		TOKEN.id = lime::IGNORE;
	    }
	    break;
	    
	    case 1:
		TOKEN.id = lime::IGNORE;
	    break;
	    
	    case 2:
		TOKEN.id = lime::HALT;
	    break;
	    
	    case 3:
		new_line();
		TOKEN.id = lime::IGNORE;
	    break;

	    default:
		/* pass on HALTS from match_token */
	    break;
	}
	
	return TOKEN;
    } // next_token()

}; // struct curlyblock


/*
tokenizer regex_quote
  ignore /[^\\"]+/
  ignore /\\./
  halt /"/
end
*/
struct regex_quote : public lime::tokenizer
{
	
    regex_quote()
    : tokenizer()
    {
	add_pattern(0, "[^\\\"]+");
	add_pattern(1, "\\.");
	add_pattern(2, "\"");
	add_pattern(3, "\r?\n");
	
	regex_set_.Compile();
	
    }
    
    
    lime::Token next_token(lime::StringPiece& text)
    {
	lime::StringPiece	START = text;
	lime::Token		TOKEN;
	
	
	int idx = match_token(text, TOKEN);
	
	switch( idx )
	{
	    case 0:
		TOKEN.id = lime::IGNORE;
	    break;
	    
	    case 1:
		TOKEN.id = lime::IGNORE;
	    break;
	    
	    case 2:
		TOKEN.id = lime::HALT;
	    break;
	    
	    case 3: /* handle new line */
		counters_->new_line();
		TOKEN.id = lime::IGNORE;
	    break;

	    default:
		/* pass on HALTS from match_token */
	    break;
	}
	
	return TOKEN;
    } // next_token()

}; // struct regex_quote



/*
tokenizer regex_slash
  ignore "[^\\/]+"
  ignore "\\."
  halt "/"
end
*/
struct regex_slash : public lime::tokenizer
{
	
    regex_slash()
    : tokenizer()
    {
	add_pattern(0, "[^\\/]+");
	add_pattern(1, "\\.");
	add_pattern(2, "/");
	add_pattern(3, "\r?\n");
	
	regex_set_.Compile();
	
    }
    
    
    lime::Token next_token(lime::StringPiece& text)
    {
	lime::StringPiece	START = text;
	lime::Token		TOKEN;
	
	
	int idx = match_token(text, TOKEN);
	
	switch( idx )
	{
	    case 0:
		TOKEN.id = lime::IGNORE;
	    break;
	    
	    case 1:
		TOKEN.id = lime::IGNORE;
	    break;
	    
	    case 2:
		TOKEN.id = lime::HALT;
	    break;
	    
	    case 3: /* handle new line */
		counters_->new_line();
		TOKEN.id = lime::IGNORE;
	    break;

	    default:
		/* pass on HALTS from match_token */
	    break;
	}
	
	return TOKEN;
    } // next_token()

}; // struct regex_slash



} // namespace lime