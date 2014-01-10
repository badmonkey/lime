#ifndef LIME_AST_TOKENIZER
#define LIME_AST_TOKENIZER

#include <list>
#include <map>
#include <string>
#include <smart_ptr>
#include "ast/pattern.h"


namespace lime
{
namespace ast
{


class tokenizer
{
public:
  typedef std::shared_ptr<tokenizer>  pointer;
  typedef std::list<pointer>  list_type;


private:
  std::string         name_;
  pattern::list_type  patterns_;
  
}; // class tokenizer


} // namespace ast
} // namespace lime

#endif // LIME_AST_TOKENIZER

