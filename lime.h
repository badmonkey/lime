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


class Counters
{
public:
    typedef std::shared_ptr<Counters>  ptr;
    
    virtual int line() const;
    virtual int offset() const;
    
    virtual void add_offset(int amt);
    virtual void new_line();
    
    static ptr make_counters();
    static ptr make_proxy(ptr c);
};


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
    , counters_( Counters::make_counters() );
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
	
	
	tok.line = counters_->line();
	tok.offset = counters_->offset();
	
	
	if ( text.empty() )
	{
	    tok.text = text;
	    tok.id = lime::HALT;
	    
	    return -1;
	}
	
	if (  !regex_set_.Match(text, &matches, &capture)
	    || matches.empty() )
	{
	    // error
	}
	
	tok.text.set( capture.data(), capture.length() );
	tok.extra_data = nullptr;
	
// 	offset_ += capture.length();
	counters_->add_offset( capture.length() );
	text.remove_prefix( capture.length() );
	
	return *std::min_element( matches.begin(), matches.end() );
    } // match_token()
    
    
//     void new_line()
//     {
// 	linenum_++;
// 	offset_ = 0;
// 	
//     } // new_line()

    void set_proxy_counters(Counters::ptr c)
    {
	counters_ = Counters::make_proxy(c);
    } // set_proxy_counters()
    
    
    StringPiece span_pieces(const StringPiece& head, const StringPiece& tail)
    {
	return StringPiece( head.data()
			  , tail.data() - head.data() + tail.length() );
    } // span_pieces()
    
    
    lime::Set		regex_set_;
    Counters::ptr	counters_;
}; // struct tokenizer



template<typename T>
Token process_machine(lime::StringPiece& text, Counters::ptr parent)
{
    T  machine;
    lime::Token t;
    
    machine.set_proxy_counters(parent);
		
    do
    {
	t = machine.next_token(text);
	if ( t.id == lime::ERROR )
	    return t;
    }
    while( t.id != lime::HALT );
    
		
    return t;
} // ignore_machine()


} // namespace lime


#endif // LIME_LIME_H