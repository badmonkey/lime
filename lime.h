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
    void*	extra_data;
  
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
    typedef std::function< int& () >	accessor_type;
    
    
      tokenizer()
    : regex_set_( lime::standard_options(), re2::RE2::ANCHOR_START)
    , linenum_data_(1), offset_data_(0)
    , linenum_( []() -> int& { return linenum_data_; } )
    , offset_( []() -> int& { return offset_data_; } )
    {
    } // tokenizer()
    
    
    void add_pattern(int idx, const char* pattern)
    {
	std::string  error;
	
	if ( regex_set_.Add( lime::StringPiece(pattern), &error ) != idx )
	{
	    throw std::runtime_error(error);
	}
    } // add_pattern()
    
    
    int match_token(StringPiece& text, Token& tok)
    {
	std::vector<int>	matches;
	lime::StringPiece	capture;
	
	
	tok.line = linenum_();
	tok.offset = offset_();
	
	
	if ( text.empty() )
	{
	    tok.text = text;
	    tok.id = lime::ERROR;
	    
	    return -1;
	}
	
	if (  !regex_set_.Match(text, &matches, &capture)
	    || matches.empty() )
	{
	    // error
	    tok.id = lime::ERROR;
	    return -1;
	}
	
	tok.text.set( capture.data(), capture.length() );
	tok.extra_data = nullptr;
	
	offset_() += capture.length();
	text.remove_prefix( capture.length() );
	
	return *std::min_element( matches.begin(), matches.end() );
    } // match_token()
    
    
    void new_line()
    {
	++( linenum_() );
	offset_() = 0;
    } // new_line()
    
    
    void use_parent_counters(const tokenizer& parent)
    {
	linenum_ = [&parent] () -> int& { return parent.linenum_(); }
	offset_ = [&parent] () -> int& { return parent.offset_(); }
    } // set_counters()

    
    
    StringPiece span_pieces(const StringPiece& head, const StringPiece& tail)
    {
	return StringPiece( head.data()
			  , tail.data() - head.data() + tail.length() );
    } // span_pieces()
    
    
    
    lime::Set		regex_set_;
    int			linenum_data_;
    int			offset_data_;
    accessor_type	linenum_;
    accessor_type	offset_;
}; // struct tokenizer



template<typename T>
Token process_machine(const tokenizer& parent, lime::StringPiece& text)
{
    T  machine;
    lime::Token t;
    
    machine.use_parent_counters(parent);
		
    do
    {
	t = machine.next_token(text);
	if ( t.id == lime::ERROR )
	    return t;
    }
    while( t.id != lime::HALT );
    
		
    return t;
} // ignore_machine()


#if !defined(SAFE_TOKEN_ID)
#define SAFE_TOKEN_ID(X)     (X)
#endif


} // namespace lime


#endif // LIME_LIME_H