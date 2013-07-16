#ifndef LIME_LIME_H
#define LIME_LIME_H

#include <stdexcept>
#include <algorithm>
#include <vector>
#include "re2/stringpiece.H"
#include "re2/set.h"


namespace lime
{
  
using re2::StringPiece;
using re2::RE2::Set;
using re2::RE2::Options;

  
  
struct Token
{
    int		id;
    StringPiece	text;
    int		line;
    int		offset;
    void*	user_data;
  
}; // struct Token


enum TOKENID
{
      HALT = 0
    , IGNORE = -1
    , ERROR = -2
};


inline Options standard_options()
{
    Options  opts;
    
    opts.set_longest_match(true);
    opts.set_never_capture(true);
    
    return opts;
} // standard_options()


struct tokenizer
{
      tokenizer()
    : regex_set_( lime::standard_options(), re2::RE2::ANCHOR_START)
    , linenum_(0)
    , offset_(0)
    {
    } // tokenizer()
    
    
    void add(int idx, const char* pattern)
    {
	std::string  error;
	
	if ( regex_set_.Add( lime::StringPiece(pattern), &error ) != idx )
	{
	    throw std::runtime_error(error);
	}
    } // add()
    
    
    void new_line()
    {
	linenum_++;
	offset_ = 0;
	
    } // new_line()
    
    
    lime::Set		regex_set_;
    int			linenum_;
    int			offset_;
}; // struct tokenizer


} // namespace lime


#endif // LIME_LIME_H