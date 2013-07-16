

#include "lime.h"


namespace lime
{
  

/*
tokenizer curlyblock
  ignore "{" using curlyblock
  ignore "[^}]+"
  halt "}"
end
*/
struct curlyblock : public lime::tokenizer
{
	
    curlyblock()
    : tokenizer()
    {
	add(0, "{");
	add(1, "[^}]+");
	add(2, "}");
	ass(3, "\n");
	
	regex_set_.Compile();
	
    }
    
    
    lime::Token next_token(lime::StringPiece& text)
    {
	std::vector<int>	matches;
	lime::StringPiece	capture;
	lime::Token		TOKEN;
	
	
	if ( !regex_set_.Match(text, &matches, &capture) )
	{
	    // error
	}
	
	int idx = *std::min_element( matches.begin(), matches.end() );
	TOKEN.text.set( capture.data(), capture.length() );
	
	text.remove_prefix( capture.length() );
	
	
	switch( idx )
	{
	    case 0:
	    {
		curlyblock  block;
		lime::Token t;
		
		do
		{
		    t = block.next_token(text);
		    if ( t.id == lime::ERROR )
			return t;
		}
		while( t.id != lime::HALT );
	    }
	    break;
	    
	    case 1:
	    break;
	    
	    case 2:
		TOKEN.id = lime::HALT;
	    break;
	    
	    case 3:
	    {
		new_line();
		TOKEN.id = lime::IGNORE;
	    }
	    break;

	    default:
	    {
	    }
	    break;
	}
	
	return TOKEN;
    } // next_token()

}; // struct curlyblock


}