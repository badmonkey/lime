#ifndef LIME_AST_PATTERN
#define LIME_AST_PATTERN

#include "ast/types.h"


namespace lime
{
namespace ast
{


class pattern
{
public:
  typedef std::shared_ptr<pattern>  pointer;
  typedef std::list<pointer>  list_type;


private:
  int          idx_;
  std::string  regex_;
  std::string  machine_name_;
  int          token_id_;
  std::string  token_value_;
  std::string  custom_code_;
  
}; // class pattern


} // namespace ast
} // namespace lime

#endif // LIME_AST_PATTERN

